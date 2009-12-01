
//// Vision Workbench
#include <vw/Plate/IndexService.h>
#include <vw/Plate/IndexManagerService.h>
#include <vw/Plate/RpcServices.h>
#include <vw/Plate/common.h>

#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace vw;
using namespace vw::platefile;

static void null_closure() {}

#if 0
const boost::shared_ptr<Blob> PlateModule::get_blob(const std::string& plate_filename, uint32 blob_id) const {
  std::ostringstream ostr;
  ostr << plate_filename << "/plate_" << blob_id << ".blob";
  const std::string& filename = ostr.str();

  BlobCache::const_iterator blob = blob_cache.find(filename);
  if (blob != blob_cache.end())
    return blob->second;

  boost::shared_ptr<Blob> ret( new Blob(filename, true) );
  blob_cache[filename] = ret;
  return ret;
}
#endif


#define VW_DEFINE_SINGLETON(name, klass) \
  namespace { \
    vw::RunOnce name ## _once = VW_RUNONCE_INIT; \
    boost::shared_ptr<klass> name ## _ptr; \
    void init_ ## name() { \
      name ## _ptr = boost::shared_ptr<klass>(new klass()); \
    } \
    void kill_ ## name() { \
      init_ ## name(); \
    } \
  } \
  const klass& name() { \
    name ## _once.run( init_ ## name ); \
    return *name ## _ptr; \
  } \
  klass& name ## _mutable() { \
    name ## _once.run( init_ ## name ); \
    return *name ## _ptr; \
  }

#define VW_DEFINE_SINGLETON_FUNC(name, klass, func) \
  namespace { \
    vw::RunOnce name ## _once = VW_RUNONCE_INIT; \
    boost::shared_ptr<klass> name ## _ptr; \
    void init_ ## name() { \
      name ## _ptr = func(); \
    } \
    void kill_ ## name() { \
      init_ ## name(); \
    } \
  } \
  const klass& name() { \
    name ## _once.run( init_ ## name ); \
    return *name ## _ptr; \
  } \
  klass& name ## _mutable() { \
    name ## _once.run( init_ ## name ); \
    return *name ## _ptr; \
  }

std::string queue_name() {
  return AmqpRpcClient::UniqueQueueName("index_client");
}

boost::shared_ptr<IndexService> create_idx() {
  return boost::shared_ptr<IndexService>(
      new IndexService::Stub(
        new AmqpRpcChannel(INDEX_EXCHANGE, "index", queue_name()),
        google::protobuf::Service::STUB_OWNS_CHANNEL) );
}

boost::shared_ptr<IndexManagerService> create_mgr() {
  return boost::shared_ptr<IndexManagerService>(
      new IndexManagerService::Stub(
        new AmqpRpcChannel(INDEX_MGR_EXCHANGE, "index_mgr", queue_name()),
        google::protobuf::Service::STUB_OWNS_CHANNEL) );
}



VW_DEFINE_SINGLETON(client, AmqpRpcClient);
VW_DEFINE_SINGLETON_FUNC(index, IndexService,        create_idx);
VW_DEFINE_SINGLETON_FUNC(mgr,   IndexManagerService, create_mgr);


void PlateInfo(const std::string& name) {

  boost::shared_ptr<Index> index = Index::construct_open(std::string("pf://index/") + name);
  const IndexHeader& hdr = index->index_header();

  vw_out(0) << "Platefile: "
            << "ID["          << hdr.platefile_id()      << "] "
            << "Name["        << fs::path(name).leaf()   << "] "
            << "Filename["    << index->platefile_name() << "] "
            << "Description[" << (hdr.has_description() ? hdr.description() : "No Description") << "]"
            << std::endl;
}

void ListPlates() {

  IndexListRequest request;
  IndexListReply   reply;

  mgr_mutable().ListRequest(&client_mutable(), &request, &reply, google::protobuf::NewCallback(&null_closure));

  vw_out(0) << "Got Plates:" << std::endl;
  std::copy(reply.platefile_names().begin(), reply.platefile_names().end(), std::ostream_iterator<std::string>(vw_out(0), " "));
  vw_out(0) << std::endl;

  std::for_each(reply.platefile_names().begin(), reply.platefile_names().end(), boost::bind(&PlateInfo, _1));
}

int main(int argc, char** argv) {

  std::string name;

  po::options_description general_options("Runs a query against the index manager, or a specified platefile id");
  general_options.add_options()
    ("platefile,p", po::value(&name), "Run an info request against this platefile id.")
    ("help", "Display this help message");

  po::options_description options("Allowed Options");
  options.add(general_options);

  po::variables_map vm;
  po::store( po::command_line_parser( argc, argv ).options(options).run(), vm );
  po::notify( vm );

  std::ostringstream usage;
  usage << "Usage: " << argv[0] << " [-p <platefile_name>]" << std::endl;
  usage << general_options << std::endl;

  if( vm.count("help") ) {
    std::cout << usage.str();
    return 0;
  }

  if (vm.count("platefile")) {
    // Run IndexInfoRequest
    PlateInfo(name);
  } else {
    // Run IndexListRequest
    ListPlates();
  }

  return 0;
}

