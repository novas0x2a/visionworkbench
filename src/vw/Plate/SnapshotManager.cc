// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <vw/Plate/PlateManager.h>    // for bbox_tiles()...
#include <vw/Plate/SnapshotManager.h>
#include <vw/Mosaic/ImageComposite.h>
#include <vw/Image/ImageView.h>
#include <set>

namespace vw {
namespace platefile {

  bool is_leaf(boost::shared_ptr<PlateFile> platefile, TileHeader const& tile_header) {

    int current_col = tile_header.col();
    int current_row = tile_header.row();

    for (int new_row = current_row*2; new_row < current_row*2+2; ++new_row) {
      for (int new_col = current_col*2; new_col < current_col*2+2; ++new_col) {

        try {
          std::list<TileHeader> tile_records;
          tile_records = platefile->search_by_location(new_col, new_row, 
                                                       tile_header.level() + 1,
                                                       tile_header.transaction_id(),
                                                       tile_header.transaction_id(),
                                                       false); // fetch_one_additional_entry
          
          // If this node has child tiles, then it is not a leaf node.
          if (tile_records.size() > 0) 
            return false;
        } catch (TileNotFoundErr &e) { /* do nothing */ }
      }
    }

    // If not child tiles were found, then this is a leaf node.
    return true;
  }

  template <class PixelT> 
  int vw::platefile::SnapshotManager<PixelT>::snapshot_helper(int current_col, 
                       int current_row, 
                       int current_level, 
                       std::map<int32, TileHeader> composite_tiles,
                       vw::BBox2i const& target_region,
                       int target_level, 
                       int start_transaction_id, 
                       int end_transaction_id, 
                       int write_transaction_id) const {

    // Check to see if there are any tiles at this level that need to be
    // snapshotted.  We fetch one additional tile outside of the
    // specified range so that we get any tiles that were not actually
    // composited during the last snapshot.
    std::list<TileHeader> tile_records;
    try {
      tile_records = m_platefile->search_by_location(current_col, current_row, current_level,
                                                   start_transaction_id, end_transaction_id,
                                                   true); // fetch_one_additional_entry
    } catch (TileNotFoundErr &e) { /* do nothing */ }

    // If there are no valid tiles at this level, then there is nothing
    // further for us to do here on this branch of the recursion.  We
    // return immediately.
    if (tile_records.size() == 0)
      return 0;

    // Insert the TileHeaders for this level into the composite_tiles
    // map<>.  Elements will be inserted in order, and any duplicate
    // transaction_ids will be overwritten by new tile records.  In
    // this fashion, we build up a list of the highest resolution
    // valid tiles in order of transaction id.
    for (std::list<TileHeader>::iterator iter = tile_records.begin();
         iter != tile_records.end(); ++iter) {

      if (current_level == target_level || is_leaf(m_platefile, *iter) ) {

        // We always add any valid tiles at the target_level, since
        // they should always be included in the composite.
        //
        // When adding tiles that may need to be supersampled, we are
        // only interested in leaf nodes.  All others will have higher
        // resolution tiles available as their children that we should
        // use instead.
        composite_tiles[iter->transaction_id()] = *iter;

      }
      
    }

    // If we have reached the target level, then we use the
    // accumulated tiles to generate a snapshot for this tile.
    if (current_level == target_level) {

      // Check to see if the second entry is the start_transaction_id.
      // If this is the case, then we can safely skip the first tile
      // (which is then assumed to be the "additional_entry" that
      // falls earlier than the given transaction range) since it
      // will have already been incorporated into the previous
      // snapshot.
      if (composite_tiles.size() >= 2) {
        std::map<int32, TileHeader>::iterator lowest_tid_iter = composite_tiles.begin();
        std::map<int32, TileHeader>::iterator second_tid_iter = lowest_tid_iter;
        ++second_tid_iter;
        if (int(second_tid_iter->first) == start_transaction_id) {
          composite_tiles.erase(lowest_tid_iter);
        } 
      }
      
      // If we arrive at the target level and find that we have fewer
      // than two tiles to composite, then there's no actual
      // compositing that needs to be done.  We can safely bail on
      // this tile.
      if (composite_tiles.size() < 2)
        return 0;

      // Create a tile into which we will accumulate composited data.
      ImageView<PixelT> composite_tile(m_platefile->default_tile_size(),
                                       m_platefile->default_tile_size());

      int num_composited = 0;
      for (std::map<int32, TileHeader>::reverse_iterator iter = composite_tiles.rbegin(); 
           iter != composite_tiles.rend(); ++iter) {

        TileHeader &current_hdr = iter->second;

        // std::cout << "\t--> [ " << current_hdr.transaction_id() << " ]  " 
        //           << current_hdr.col() << " " << current_hdr.row()
        //           << " @ " << current_hdr.level() << "\n";
        
        // Tile access is highly localized during snapshotting, so it
        // speeds things up considerably to cache them here.
        ImageView<PixelT> new_tile;
        if ( !(this->restore_tile(new_tile, current_hdr.col(), current_hdr.row(), 
                                  current_hdr.level(), current_hdr.transaction_id())) ) {
          try {
            // If the tile isn't in our cache, then we take the
            // performance hit and read the tile from the platefile.
            m_platefile->read(new_tile, 
                            current_hdr.col(), current_hdr.row(), 
                            current_hdr.level(), current_hdr.transaction_id(),
                            true); // exact_transaction_match
            this->save_tile(new_tile, current_hdr.col(), current_hdr.row(), 
                            current_hdr.level(), current_hdr.transaction_id());
          } catch (BlobIoErr &e) {
            // If we get a BlobIO error, that's bad news, but not worth
            // killing the snapshot for.  Instead we log the error here and move
            // onto the next location.
            std::ostringstream ostr;
            ostr << "WARNING: error reading tile from blob: " << e.what();
            m_platefile->log(ostr.str());
            continue;
          }

        }
        
        // If this is the first tile in this location, it's already at
        // the target_level, and it's opaque (which is a very common
        // case), then we don't need to save it as part of the
        // snapshot since its already the top tile in the snapshot.
        // This optimization saves us both space and time!
        if ( current_hdr.level() == target_level && 
             iter == composite_tiles.rbegin() &&
             is_opaque(new_tile) ) {
          return 0;
        }

        // If we are compositing a tile from a lower resolution in the
        // pyramid, it needs to be supersampled and cropped before we
        // can use it here.
        if (current_hdr.level() != target_level) {

          new_tile = resample_img_from_level(
                      new_tile, current_hdr.col(), current_hdr.row(), current_hdr.level(),
                      current_col, current_row, target_level);

        }

        // Add the new tile UNDERNEATH the tiles we have already composited.
        vw::mosaic::ImageComposite<PixelT> composite;
        composite.insert(new_tile, 0, 0);
        composite.insert(composite_tile, 0, 0);
        composite.set_draft_mode( true );
        composite.prepare();

        // Overwrite the composite_tile
        composite_tile = composite;
        ++num_composited;

        // Check to see if the composite tile is opaque.  If it is,
        // then there's no point in compositing any more tiles because
        // they will end up hidden anyway.
        if ( is_opaque(composite_tile) ) {
          break;
        }
      }

      if (num_composited > 0) {
        // -- debug
        vw_out(DebugMessage, "plate::snapshot")  << "\t    Compositing " << current_col << " " 
                                                 << current_row << " @ " << current_level 
                                                 << "  [ " << num_composited << " ]\n";
        //---

        // for (unsigned j = 0; j < composite_tile.rows(); ++j) {
        //   for (unsigned i = 0; i < composite_tile.cols(); ++i) {
        //     composite_tile(i,j)[0] *= -1;
        //   }
        // }

        m_platefile->write_update(composite_tile, 
                                current_col, current_row, current_level,
                                write_transaction_id);
        return 1;
      } else {
        return 0;
      }

    } else {
      
      // If we haven't reached the target level, then are four
      // possible children that we can explore for more snapshotting
      // information.
      int num_tiles_updated = 0;
      for (int new_row = current_row*2; new_row < current_row*2+2; ++new_row) {
        for (int new_col = current_col*2; new_col < current_col*2+2; ++new_col) {

          // We should only explore those child tiles that overlap
          // with the target_region at the target level.  To test
          // this, we create a bounding box for the child tile we are
          // testing, and scale that bounding box up to see how big it
          // is when projected into the target_level of the pyramid.
          BBox2i parent_region(new_col, new_row, 1, 1);
          parent_region.min() *= pow(2,target_level-(current_level+1));
          parent_region.max() *= pow(2,target_level-(current_level+1));


          // With the scaled up parent region and the target region in
          // hand, we can do a simple intersection test to assess
          // whether this child tile is worth exploring.
          if (parent_region.intersects(target_region)) {
            num_tiles_updated += snapshot_helper(new_col, new_row, current_level+1,
                                                 composite_tiles, target_region, target_level, 
                                                 start_transaction_id, end_transaction_id, 
                                                 write_transaction_id);
          }
        }
      }
      return num_tiles_updated;
    }
  }
}} // namespace vw::platefile


template <class PixelT>
void vw::platefile::SnapshotManager<PixelT>::snapshot(int level, BBox2i const& tile_region, 
                                                      int start_transaction_id, 
                                                      int end_transaction_id, 
                                                      int write_transaction_id) const {
  
  // Subdivide the bbox into smaller workunits if necessary.
  // This helps to keep operations efficient.
  std::list<BBox2i> tile_workunits = bbox_tiles(tile_region, 1024, 1024);
  for ( std::list<BBox2i>::iterator region_iter = tile_workunits.begin(); 
        region_iter != tile_workunits.end(); ++region_iter) {

    // Create an empty list of composite tiles and then kick off the
    // recursive snapshotting process.
    std::map<int32, TileHeader> composite_tiles;
    int num_tiles_updated = snapshot_helper(0, 0, 0, composite_tiles, *region_iter, level, 
                                            start_transaction_id, end_transaction_id, 
                                            write_transaction_id);

    if (num_tiles_updated > 0)
      vw_out() << "\t--> Snapshot " << *region_iter << " @ level " << level 
               << " (" << num_tiles_updated << " tiles updated).\n";

  }
}

// Create a full snapshot of every level and every region in the mosaic.
template <class PixelT>
void vw::platefile::SnapshotManager<PixelT>::full_snapshot(int start_transaction_id, 
                                                           int end_transaction_id, 
                                                           int write_transaction_id) const {

  for (int level = 0; level < m_platefile->num_levels(); ++level) {    

    // For debugging:
    //    for (int level = 11; level < 12; ++level) {    

    // Snapshot the entire region at each level.  These region will be
    // broken down into smaller work units in snapshot().
    int region_size = pow(2,level);
    int subdivided_region_size = region_size / 16;
    if (subdivided_region_size < 1024) subdivided_region_size = 1024;
    BBox2i full_region(0,0,region_size,region_size);
    std::list<BBox2i> workunits = bbox_tiles(full_region, 
                                             subdivided_region_size, 
                                             subdivided_region_size);
    for ( std::list<BBox2i>::iterator region_iter = workunits.begin(); 
          region_iter != workunits.end(); ++region_iter) {
      snapshot(level, *region_iter, start_transaction_id, 
               end_transaction_id, write_transaction_id);
    }
  }
}

// Explicit template instatiation
namespace vw { 
namespace platefile {
  
  template 
  void vw::platefile::SnapshotManager<PixelGrayA<uint8> >::snapshot(int level, 
                                                                    BBox2i const& bbox, 
                                                                    int start_transaction_id, 
                                                                    int end_transaction_id, 
                                                                    int write_transaction_id) const;
  template 
  void vw::platefile::SnapshotManager<PixelGrayA<int16> >::snapshot(int level, 
                                                                    BBox2i const& bbox, 
                                                                    int start_transaction_id, 
                                                                    int end_transaction_id, 
                                                                    int write_transaction_id) const;
  template
  void vw::platefile::SnapshotManager<PixelRGBA<uint8> >::snapshot(int level, 
                                                                   BBox2i const& bbox, 
                                                                   int start_transaction_id, 
                                                                   int end_transaction_id, 
                                                                   int write_transaction_id) const;

  template
  void vw::platefile::SnapshotManager<PixelGrayA<uint8> >::full_snapshot(int start_transaction_id, 
                                                                         int end_transaction_id, 
                                                                         int write_transaction_id) const;
  template
  void vw::platefile::SnapshotManager<PixelGrayA<int16> >::full_snapshot(int start_transaction_id, 
                                                                         int end_transaction_id, 
                                                                         int write_transaction_id) const;
  template
  void vw::platefile::SnapshotManager<PixelRGBA<uint8> >::full_snapshot(int start_transaction_id, 
                                                                         int end_transaction_id, 
                                                                         int write_transaction_id) const;



}}
