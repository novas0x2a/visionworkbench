#include "FrameStore.h"

#include "vw/Core/Exception.h"
#include "vw/Math/EulerAngles.h"

#include <algorithm>
#include <memory>

namespace vw
{
namespace geometry
{
  using namespace std;
  using namespace vw::math;

  FrameHandle const FrameStore::NULL_HANDLE = FrameHandle(NULL);

  FrameStore::~FrameStore() throw() 
  {
    // delete all frames
    FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
    for (first = m_root_nodes.begin(); first != last; ++first) {
      (*first)->recursive_delete();
    }
  }

  void
  FrameStore::clone_tree(FrameTree & tree, FrameHandle rootFrame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    tree.clear();
    if (rootFrame.node == NULL) {
      int num_nodes = 0;
      FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
      for (first = m_root_nodes.begin(); first != last; ++first) {
	num_nodes += (*first)->num_offsprings() + 1;
      }
      tree.reserve(num_nodes);
    }
    else {
      tree.reserve(rootFrame.node->num_offsprings() + 1);
    }

    if (rootFrame.node == NULL) {
      FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
      for (first = m_root_nodes.begin(); first != last; ++first) {
	(*first)->clone_vec(NULL, tree);
      }
    }
    else {
      rootFrame.node->clone_vec(NULL, tree);
    }
  }

  FrameTreeNode *
  FrameStore::clone_tree(FrameHandle rootFrame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (rootFrame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    return rootFrame.node->clone();
  }

  std::string const& 
  FrameStore::name(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    return frame.node->data().name();
  }

  std::string
  FrameStore::full_name(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    string name;
    FrameTreeNodeVector const& ancestry = frame.node->ancestry();
    FrameTreeNodeVector::const_iterator first, last = ancestry.end();
    for (first = ancestry.begin(); first != last; ++first) {
      name += "/" + (*first)->data().name();
    }

    return name;
  }

  FrameHandle 
  FrameStore::lookup(std::string const& name, FrameHandle scope) const
  {
    
    if (scope == NULL &&
	!name.empty() && name[0] != '/') {
      // try to explicitly resolve the root frames
      string searchName = "/" + name;
      FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
      for (first = m_root_nodes.begin(); first != last; ++first) {
	FrameTreeNode * node = ::vw::geometry::lookup(*first, searchName);

	if (node != NULL)
	  return FrameHandle(node);
      }
    }

    string searchName = name;

    // if not explicitly state otherwise, we search for .../name 
    if (!name.empty() && name[0] != '/') {
      if (name.length() < 4 || name.substr(0, 4) != ".../") {
	searchName = ".../" + name;
      }
    }
  
    if (scope == NULL || 
	(!searchName.empty() && searchName[0] == '/')) {
      FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
      for (first = m_root_nodes.begin(); first != last; ++first) {
	FrameTreeNode * node = ::vw::geometry::lookup(*first, searchName);

	if (node != NULL)
	  return FrameHandle(node);
      }
      return NULL;
    }

    return ::vw::geometry::lookup(scope.node, searchName);
  }


  namespace 
  {
  class Append
  {
    vector<string>& s_;
  public:
    Append(vector<string>& s) :
      s_(s) 
    {}
    void operator() (FrameTreeNode const * node) 
    {
      string full_name;
      vector<FrameTreeNode *> const& ancestry = node->ancestry();
      vector<FrameTreeNode *>::const_iterator first, last = ancestry.end();
      for (first = ancestry.begin(); first != last; ++first) {
	full_name += "/" + (*first)->data().name();
	}
      
      s_.push_back(full_name);
    }
  };
    
  class Nothing
  {
  public:
    void operator() (FrameTreeNode const *) {}
  };
  }

  std::vector<std::string>
  FrameStore::frame_names() const
  {
    RecursiveMutex::Lock lock(m_mutex);

    vector<string> names;
   
    Nothing n;
    Append app(names);
    FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
    for (first = m_root_nodes.begin(); first != last; ++first) {
      (*first)->pre_order_traverse(app, n, n);
    }

    return names;
  }


  FrameHandle
  FrameStore::parent(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    return frame.node->parent();
  }

  FrameHandle
  FrameStore::root(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    return frame.node->root();
  }

  /** Get the list of direct children of a frame.
   * 
   * @param frame
   */
  FrameStore::FrameHandleVector
  FrameStore::children(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    if (frame.node == NULL) {
      return FrameHandleVector(m_root_nodes.begin(), m_root_nodes.end());
    }
    
    FrameTreeNodeVector c = frame.node->children();
    return FrameHandleVector(c.begin(), c.end());
  }



  FrameHandle
  FrameStore::add(std::string const& name, FrameHandle parent, Location const& p)
  {
    // create new node
    FrameTreeNode * node = new FrameTreeNode(NULL, Frame(name, p));
    // add node
    add(node, parent);

    return node;
  }
  
  void
  FrameStore::add(FrameTreeNode * node, FrameHandle parent)
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (node != NULL,
		 vw::LogicErr("NULL pointer not allowed as node parameter."));
    VW_ASSERT (!is_member(node),
		 vw::LogicErr("Node already member of FrameStore instance."));

    auto_ptr<FrameTreeNode> n(node);

    VW_ASSERT (node->data().name().length() != 0,
	       vw::LogicErr("None empty frame name required."));
    VW_ASSERT (parent.node == NULL || is_member(parent),
	       vw::LogicErr("None member node not allowed as parent."));
    
    // check frame name uniqueness
    assert_unique(node->data().name(), parent.node);
      
    if (parent.node == NULL)
      m_root_nodes.push_back(node);
      
    node->set_parent(parent.node);

    n.release();
  }
 
  void
  FrameStore::del(FrameHandle frame, bool recursive)
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));
    
    // erase the node from root node vector, if root node
    FrameTreeNodeVector::iterator elem =
      std::find(m_root_nodes.begin(), m_root_nodes.end(), frame.node);
    if (elem != m_root_nodes.end())
      m_root_nodes.erase(elem);

    if (frame.node != NULL) {
      if (recursive) {
	// delete subtree
	frame.node->recursive_delete();
	// decrease global node count
      }
      else {
	// safe children
	FrameTreeNodeVector children = frame.node->children();
	// insert child nodes into root-node vector
	m_root_nodes.insert(m_root_nodes.end(), children.begin(), children.end());
	// delete frame
	delete frame.node;
	// we deleted only one node
      }
    }
  }
  
  void
  FrameStore::set_parent(FrameHandle frame, FrameHandle parent) 
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as frame parameter."));
    VW_ASSERT (parent.node == NULL || is_member(parent),
	       vw::LogicErr("None member node not allowed as parent."));

    if (frame.node->parent() == parent.node)
      return;

    assert_unique(frame.node->data().name(), parent.node);

    frame.node->set_parent(parent.node);
  }
  
  bool
  FrameStore::is_root(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));
    
    return frame.node->is_root();
  }
  
  bool
  FrameStore::is_leaf(FrameHandle frame) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));
    
    return frame.node->is_leaf();
  }
  
  bool
  FrameStore::is_ancestor_of(FrameHandle frame, FrameHandle pop) const
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL && pop.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    return frame.node->is_ancestor_of(pop.node);
  }

  Frame::Location
  FrameStore::get_location_of(FrameHandle frame, FrameHandle source, Location const& loc)
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL && source.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));
    
    return vw::geometry::get_location_of(frame.node, source.node, loc);
  }
  
  Frame::Location
  FrameStore::get_location(FrameHandle frame, FrameHandle source)
  {
    RecursiveMutex::Lock lock(m_mutex);
    
    VW_ASSERT (source.node != NULL, 
	       vw::LogicErr("NULL handle not allowed as frame parameter."));
       
    return vw::geometry::get_location(frame.node, source.node);
  }
  
  void 
  FrameStore::set_location(FrameHandle frame, FrameHandle wrt_frame, Location const& update)
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    vw::geometry::set_location(frame.node, wrt_frame.node, update);
  }

  void 
  FrameStore::set_location_rel(FrameHandle frame, Location const& update)
  {
    RecursiveMutex::Lock lock(m_mutex);

    VW_ASSERT (frame.node != NULL,
	       vw::LogicErr("NULL handle not allowed as parameter."));

    vw::geometry::set_location(frame.node, NULL, update);
  }

  bool
  FrameStore::is_member(FrameHandle frame) const throw() 
  {
    RecursiveMutex::Lock lock(m_mutex);
    return is_member(frame.node);
  }

  void
  FrameStore::merge_tree(FrameTreeNode * tree, FrameHandle start_frame)
  {
    // if a start node is given for mergin
    if (start_frame != NULL_HANDLE) {
      vw::geometry::merge_frame_trees(start_frame.node, tree);
      tree->recursive_delete();
      return;
    }

    // try to match one of the root nodes and merge with it
    FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
    for (first = m_root_nodes.begin(); first != last; ++first) {
      if ((*first)->data().name() == tree->data().name()) {
	vw::geometry::merge_frame_trees(*first, tree);
	tree->recursive_delete();
	return;
      }
    }
    
    // just add the tree to the forest
    m_root_nodes.push_back(tree);
  }

  void
  FrameStore::update_frames(UpdateVector const& updates) 
  {
    UpdateVector::const_iterator first, last = updates.end();
    for (first = updates.begin(); first != last; ++first) {
      unsigned index = first->axis;
      vw::ATrans3& atrans = first->handle.node->data().location();
      switch (first->axis) {
      case FrameUpdate::X:
      case FrameUpdate::Y:
      case FrameUpdate::Z:
	atrans.translation()[index] = first->value;
	break;
      case FrameUpdate::Roll:
      case FrameUpdate::Pitch:
      case FrameUpdate::Yaw:
	{
	  index -= 3;
	  Vector3 rpy = 
	    rotation_matrix_to_euler_xyz(atrans.rotation());
	  rpy[index] = first->value;
	  atrans.rotation() =
	    euler_to_rotation_matrix(rpy[0], rpy[1], rpy[2], "XYZ");
	}
	break;
      }
    }
  }

  bool
  FrameStore::is_member(FrameTreeNode * node) const throw() {
    if (node != NULL) {
      FrameTreeNodeVector::const_iterator first, last = m_root_nodes.end();
      for (first = m_root_nodes.begin(); first != last; ++first) {
	if ((*first) == node ||
	    (*first)->is_ancestor_of(node))
	  return true;
      }
    }
    return false;
  }

  // check name uniqueness
  void
  FrameStore::assert_unique(std::string const& name, FrameTreeNode * parent) const
  {
    char const * const childError = "Name not unique as child node.";
    char const * const rootError = "Name not unique as root node.";
    char const * err = NULL;

    FrameTreeNodeVector children;
    FrameTreeNodeVector::const_iterator first, last;

    if (parent == NULL) {
      first = m_root_nodes.begin();
      last = m_root_nodes.end();
      err = rootError;
    }
    else {
      children = parent->children();
      first = children.begin();
      last = children.end();
      err = childError;
    }
    
    for (; first != last; ++ first) {
      if ((*first)->data().name() == name)
	vw_throw(vw::LogicErr(err));
    }
  }

}
}
