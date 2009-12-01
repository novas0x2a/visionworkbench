// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


/// \file DiskImageResource.cc
///
/// An abstract base class referring to an image on disk.
///

#ifdef _MSC_VER
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#endif

#include <vw/config.h>

#include <iostream>
#include <set>
#include <map>
#include <boost/algorithm/string.hpp>

// For RunOnce
#include <vw/Core/Thread.h>

#include <vw/FileIO/DiskImageResource.h>
#include <vw/FileIO/DiskImageResourcePDS.h>
#include <vw/FileIO/DiskImageResourcePBM.h>

#if defined(VW_HAVE_PKG_PNG) && VW_HAVE_PKG_PNG==1
#include <vw/FileIO/DiskImageResourcePNG.h>
#endif

#if defined(VW_HAVE_PKG_JPEG) && VW_HAVE_PKG_JPEG==1
#include <vw/FileIO/DiskImageResourceJPEG.h>
#endif

#if defined(VW_HAVE_PKG_JPEG2K) && VW_HAVE_PKG_JPEG2K==1
#include <vw/FileIO/DiskImageResourceJP2.h>
#endif

#if defined(VW_HAVE_PKG_TIFF) && VW_HAVE_PKG_TIFF==1
#include <vw/FileIO/DiskImageResourceTIFF.h>
#endif

#if defined(VW_HAVE_PKG_OPENEXR) && VW_HAVE_PKG_OPENEXR==1
#if defined(VW_ENABLE_EXCEPTIONS) && VW_ENABLE_EXCEPTIONS==1
#include <vw/FileIO/DiskImageResourceOpenEXR.h>
#endif
#endif

#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
#include <vw/FileIO/DiskImageResourceGDAL.h>
#endif

#include <vw/FileIO/DiskImageResource_internal.h>

namespace {
  typedef std::map<std::string,vw::DiskImageResource::construct_open_func> OpenMapType;
  typedef std::map<std::string,vw::DiskImageResource::construct_create_func> CreateMapType;
  OpenMapType *open_map = 0;
  CreateMapType *create_map = 0;
}

bool vw::DiskImageResource::default_rescale = true;

void vw::DiskImageResource::set_rescale(bool rescale) {
  m_rescale = rescale;
}

void vw::DiskImageResource::set_default_rescale(bool rescale) {
  default_rescale = rescale;
}

namespace vw {
  namespace internal {

void foreach_ext(std::string const& prefix, ExtTestFunction const& callback, std::set<std::string> const& exclude)
{
  OpenMapType::const_iterator oi;
  vw::DiskImageResource::register_default_file_types();

  for (oi = open_map->begin(); oi != open_map->end(); ++oi)
  {
    if (exclude.find(oi->first.substr(1)) == exclude.end())
      callback(prefix + oi->first);
  }
}

}}

void vw::DiskImageResource::register_file_type( std::string const& extension,
                                                std::string const& disk_image_resource_type,
                                                vw::DiskImageResource::construct_open_func open_func,
                                                vw::DiskImageResource::construct_create_func create_func )
{
  if( ! open_map ) open_map = new OpenMapType();
  if( ! create_map ) create_map = new CreateMapType();
  //  std::cout << "REGISTERING DiskImageResource " << disk_image_resource_type << " for extension " << extension << std::endl;

  OpenMapType::iterator om_iter = open_map->find( extension );
  if ( om_iter != open_map->end() )
    om_iter->second = open_func;
  else
    open_map->insert( std::make_pair( extension, open_func ) );

  CreateMapType::iterator cm_iter = create_map->find( extension );
  if ( cm_iter != create_map->end() )
    cm_iter->second = create_func;
  else
    create_map->insert( std::make_pair( extension, create_func ) );

}

static std::string file_extension( std::string const& filename ) {
  std::string::size_type dot = filename.find_last_of('.');
  if (dot == std::string::npos)
    vw_throw( vw::IOErr() << "DiskImageResource: Cannot infer file format from filename with no file extension." );
  std::string extension = filename.substr( dot );
  boost::to_lower( extension );
  return extension;
}

static vw::RunOnce rdft_once = VW_RUNONCE_INIT;

static void register_default_file_types_impl() {

  // Give GDAL precedence in reading PDS images when this is supported.
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support(".img") &&
      vw::DiskImageResourceGDAL::gdal_has_support(".pds") &&
      vw::DiskImageResourceGDAL::gdal_has_support(".lbl")) {
    vw::DiskImageResource::register_file_type( ".img", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
    vw::DiskImageResource::register_file_type( ".pds", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
    vw::DiskImageResource::register_file_type( ".lbl", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
  } else {
#endif
  vw::DiskImageResource::register_file_type( ".img", vw::DiskImageResourcePDS::type_static(), &vw::DiskImageResourcePDS::construct_open, &vw::DiskImageResourcePDS::construct_create );
  vw::DiskImageResource::register_file_type( ".pds", vw::DiskImageResourcePDS::type_static(), &vw::DiskImageResourcePDS::construct_open, &vw::DiskImageResourcePDS::construct_create );
  vw::DiskImageResource::register_file_type( ".lbl", vw::DiskImageResourcePDS::type_static(), &vw::DiskImageResourcePDS::construct_open, &vw::DiskImageResourcePDS::construct_create );
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  }
#endif

#if defined(VW_HAVE_PKG_PNG) && VW_HAVE_PKG_PNG==1
  vw::DiskImageResource::register_file_type( ".png", vw::DiskImageResourcePNG::type_static(), &vw::DiskImageResourcePNG::construct_open, &vw::DiskImageResourcePNG::construct_create );
#elif defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support(".png"))
    vw::DiskImageResource::register_file_type( ".png", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
  else
    vw::vw_throw(vw::IOErr() << "GDAL does not have PNG support.");
#endif

#if defined(VW_HAVE_PKG_JPEG) && VW_HAVE_PKG_JPEG==1
  vw::DiskImageResource::register_file_type( ".jpg", vw::DiskImageResourceJPEG::type_static(), &vw::DiskImageResourceJPEG::construct_open, &vw::DiskImageResourceJPEG::construct_create );
  vw::DiskImageResource::register_file_type( ".jpeg", vw::DiskImageResourceJPEG::type_static(), &vw::DiskImageResourceJPEG::construct_open, &vw::DiskImageResourceJPEG::construct_create );
#elif defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support(".jpg"))
    vw::DiskImageResource::register_file_type( ".jpg", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
  if (vw::DiskImageResourceGDAL::gdal_has_support(".jpeg"))
    vw::DiskImageResource::register_file_type( ".jpeg", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
#endif

#if defined(VW_HAVE_PKG_JPEG2K) && VW_HAVE_PKG_JPEG2K==1 && 0
  // A file with a .jp2 extension is a full fledged JPEG2000 image
  // with acquisition metadata. A file with a .j2k extension has only
  // the "raw" encoded image, with image encoding and size specified
  // in a small header. A file with a .jpf extension is a full fledged
  // JPEG2000 image with acquisition and (possibly) GML metadata.
  vw::DiskImageResource::register_file_type(".jp2", vw::DiskImageResourceJP2::type_static(), &vw::DiskImageResourceJP2::construct_open, &vw::DiskImageResourceJP2::construct_create );
  vw::DiskImageResource::register_file_type(".j2k", vw::DiskImageResourceJP2::type_static(), &vw::DiskImageResourceJP2::construct_open, &vw::DiskImageResourceJP2::construct_create );
  vw::DiskImageResource::register_file_type(".jpf", vw::DiskImageResourceJP2::type_static(), &vw::DiskImageResourceJP2::construct_open, &vw::DiskImageResourceJP2::construct_create );
#elif defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support(".jp2"))
    vw::DiskImageResource::register_file_type(".jp2", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
  if (vw::DiskImageResourceGDAL::gdal_has_support(".j2k"))
    vw::DiskImageResource::register_file_type(".j2k", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
#endif

// This is a little hackish but it makes it so libtiff acts as a proper fallback
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support(".tif") && vw::DiskImageResourceGDAL::gdal_has_support(".tiff")) {
    vw::DiskImageResource::register_file_type( ".tif", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
    vw::DiskImageResource::register_file_type( ".tiff", vw::DiskImageResourceGDAL::type_static(), &vw::DiskImageResourceGDAL::construct_open, &vw::DiskImageResourceGDAL::construct_create );
  } else {
#endif
#if defined(VW_HAVE_PKG_TIFF) && VW_HAVE_PKG_TIFF==1
    vw::DiskImageResource::register_file_type( ".tif", vw::DiskImageResourceTIFF::type_static(), &vw::DiskImageResourceTIFF::construct_open, &vw::DiskImageResourceTIFF::construct_create );
    vw::DiskImageResource::register_file_type( ".tiff", vw::DiskImageResourceTIFF::type_static(), &vw::DiskImageResourceTIFF::construct_open, &vw::DiskImageResourceTIFF::construct_create );
#endif
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  }
#endif

#if defined(VW_HAVE_PKG_OPENEXR) && VW_HAVE_PKG_OPENEXR==1
  vw::DiskImageResource::register_file_type( ".exr", vw::DiskImageResourceOpenEXR::type_static(), &vw::DiskImageResourceOpenEXR::construct_open, &vw::DiskImageResourceOpenEXR::construct_create );
#endif

  // Filetypes that are always supported
  vw::DiskImageResource::register_file_type( ".pbm", vw::DiskImageResourcePBM::type_static(), &vw::DiskImageResourcePBM::construct_open, &vw::DiskImageResourcePBM::construct_create );
  vw::DiskImageResource::register_file_type( ".pgm", vw::DiskImageResourcePBM::type_static(), &vw::DiskImageResourcePBM::construct_open, &vw::DiskImageResourcePBM::construct_create );
  vw::DiskImageResource::register_file_type( ".ppm", vw::DiskImageResourcePBM::type_static(), &vw::DiskImageResourcePBM::construct_open, &vw::DiskImageResourcePBM::construct_create );

}

// This extra class helps to ensure that register_file_type() is only run once.
void vw::DiskImageResource::register_default_file_types() {
  rdft_once.run( register_default_file_types_impl );
}

vw::DiskImageResource* vw::DiskImageResource::open( std::string const& filename ) {
  register_default_file_types();
  if( open_map ) {
    OpenMapType::iterator i = open_map->find( file_extension( filename ) );
    if( i != open_map->end() ) {
      DiskImageResource* rsrc = i->second( filename );
      vw_out(DebugMessage,"fileio") << "Produce DiskImageResource of type: " << rsrc->type() << "\n";
      return rsrc;
    }
  }

  // GDAL has support for many useful file formats, and we fall back
  // on it here in case none of the registered file handlers know how
  // to do the job.
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
  if (vw::DiskImageResourceGDAL::gdal_has_support( file_extension(filename) ))
    return vw::DiskImageResourceGDAL::construct_open(filename);
#endif

  // If all attempts to find a suitable file driver fails, we throw an
  // exception.
  vw_throw( NoImplErr() << "Unsupported file format: " << filename );
  return 0; // never reached
}

/// Returns a disk image resource with the given filename.  The file
/// type is determined by the value in 'type'.
vw::DiskImageResource* vw::DiskImageResource::create( std::string const& filename, ImageFormat const& format, std::string const& type ) {
  register_default_file_types();
  if( create_map ) {
    CreateMapType::iterator i = create_map->find( type );
    if( i != create_map->end() )
      return i->second( filename, format );
  }
  vw_throw( NoImplErr() << "Unsupported file type \"" << type << "\" for filename: " << filename );
  return 0; // never reached
}

/// Returns a disk image resource with the given filename.  The file
/// type is determined by the extension of the filename.
vw::DiskImageResource* vw::DiskImageResource::create( std::string const& filename, ImageFormat const& format ) {
  register_default_file_types();
  if( create_map ) {
    CreateMapType::iterator i = create_map->find( file_extension( filename ) );
    if( i != create_map->end() )
      return i->second( filename, format );
  }
  vw_throw( NoImplErr() << "Unsupported file format: " << filename );
  return 0; // never reached
}
