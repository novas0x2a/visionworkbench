// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#ifndef __VW_MOSAIC_IMAGE_COMPOSITE_SIMPLE_H__
#define __VW_MOSAIC_IMAGE_COMPOSITE_SIMPLE_H__

#include <vw/Image/ImageViewBase.h>
#include <vw/Image/ImageViewRef.h>
#include <vw/Image/EdgeExtension.h>

namespace vw {
namespace mosaic {

  template <class PixelT>
  class CompositeView : public ImageViewBase<CompositeView<PixelT> > {

    std::vector<ImageViewRef<PixelT> > m_images;
    std::vector<BBox2i> m_bboxes;
    BBox2i m_view_bbox, m_data_bbox;

    ImageView<PixelT> generate_patch( BBox2i const& patch_bbox ) const;

  public:
    typedef PixelT pixel_type;
    typedef PixelT result_type;
    typedef ProceduralPixelAccessor<CompositeView> pixel_accessor;

    CompositeView() {}
    CompositeView(const std::vector<ImageViewRef<PixelT> >& images, const std::vector<BBox2i>& boxes)
      : m_images(images), m_bboxes(boxes)
    {
      VW_ASSERT(images.size() == boxes.size(), LogicErr() << "Need a bbox for each image");
      if (images.size() == 0)
        return;
      m_view_bbox = m_data_bbox = boxes[0];
      for (size_t i = 1; i < m_images.size(); ++i) {
        m_view_bbox.grow( m_bboxes[i] );
        m_data_bbox.grow( m_bboxes[i] );
      }
    }

    void insert( ImageViewRef<PixelT> const& image, int32 x, int32 y );

    void prepare( const ProgressCallback &progress_callback = ProgressCallback::dummy_instance() );
    void prepare( BBox2i const& view_bbox, const ProgressCallback &progress_callback = ProgressCallback::dummy_instance() );

    /// Returns the number of images that intersect with the supplied
    /// bbox.  Useful if you are deciding whether it is worthwhile to
    /// rasterize this bounding box.
    uint32 intersects(const BBox2i& bbox) const {
      uint32 num_intersections = 0;
      for (size_t i = 0; i < m_bboxes.size(); ++i) {
        if(bbox.intersects(m_bboxes[i]))
          ++num_intersections;
      }
      return num_intersections;
    }

    inline int32 cols() const { return m_data_bbox.width(); }
    inline int32 rows() const { return m_data_bbox.height(); }
    inline int32 planes() const { return 1; }

    BBox2i const& bbox() const { return m_data_bbox; }

    inline pixel_accessor origin() const { return pixel_accessor(*this); }

    inline result_type operator()( int32 i, int32 j, int32 /*p*/ = 0 ) const {
      for (ssize_t p = m_images.size()-1; p >= 0; --p) {
        if( m_bboxes[p].contains( Vector2i(i,j) ) ) {
          int32 ii = i - m_bboxes[p].min().x();
          int32 jj = j - m_bboxes[p].min().y();
          result_type px = m_images[p](ii,jj);
          if ( ! is_transparent(px) )
            return px;
        }
      }
      return result_type();
    }

    typedef CompositeView<PixelT> prerasterize_type;
    inline prerasterize_type prerasterize( BBox2i const& bbox ) const {
      std::vector<ImageViewRef<PixelT> > images;
      std::vector<BBox2i> bboxes;
      vw_out(DebugMessage, "mosaic.composite") << "Prerastersize to " << bbox << std::endl;
      for (uint32 i = 0; i < m_images.size(); ++i) {
        const ImageViewRef<PixelT>& img = m_images[i];
        BBox2i new_box(m_bboxes[i]);
        vw_out(DebugMessage, "mosaic.composite") << "Checking " << new_box << std::endl;
        new_box.crop(bbox);
        vw_out(DebugMessage, "mosaic.composite") << "Cropped to " << new_box << std::endl;
        if (new_box.width() > 0 && new_box.height() > 0) {
          vw_out(DebugMessage, "mosaic.composite") << "In zone. Cropping " << img.planes() << "x" << img.rows() << "x" << img.cols() << " image." << std::endl;
          ImageView<PixelT> patch = crop( img, new_box - m_bboxes[i].min());
          images.push_back(patch);
          bboxes.push_back(new_box);
        }
      }
      return prerasterize_type(images, bboxes);
    }
    template <class DestT> inline void rasterize( DestT const& dest, BBox2i bbox ) const {
      vw::rasterize( prerasterize(bbox), dest, bbox );
    }
  };

}}


template <class PixelT>
void vw::mosaic::CompositeView<PixelT>::insert( ImageViewRef<PixelT> const& image, int32 x, int32 y ) {
  m_images.push_back( image );
  int32 cols = image.cols(), rows = image.rows();
  BBox2i image_bbox( Vector2i(x, y), Vector2i(x+cols, y+rows) );
  m_bboxes.push_back( image_bbox );
  if( m_bboxes.size() == 1 ) {
    m_view_bbox = m_bboxes.back();
    m_data_bbox = m_bboxes.back();
  }
  else {
    m_view_bbox.grow( image_bbox );
    m_data_bbox.grow( image_bbox );
  }
}


template <class PixelT>
void vw::mosaic::CompositeView<PixelT>::prepare( vw::ProgressCallback const& /*progress_callback*/ ) {
  // Translate bboxes to origin
  for( unsigned i=0; i<m_images.size(); ++i )
    m_bboxes[i] -= m_view_bbox.min();
  m_data_bbox -= m_view_bbox.min();
}

template <class PixelT>
void vw::mosaic::CompositeView<PixelT>::prepare( BBox2i const& total_bbox, vw::ProgressCallback const& progress_callback ) {
  m_view_bbox = total_bbox;
  prepare( progress_callback );
}

#endif
