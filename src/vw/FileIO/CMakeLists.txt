vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS 
  ${EXPORT_H}
  DiskImageResource.h 
  DiskImageResourcePBM.h
  DiskImageResourcePDS.h 
  DiskImageView.h 
  KML.h 
)

set( SOURCES 
  DiskImageResource.cc
  DiskImageResourcePBM.cc
  DiskImageResourcePDS.cc 
  KML.cc 
)

link_libraries( 
  ${Boost_FILESYSTEM_LIBRARY}
  vwImage
)

set( IO_IMPLS 
  PNG
  JPEG
  TIFF
  OpenEXR
  HDF
  GDAL
)

foreach( IO_IMPL ${IO_IMPLS} )
  string( TOUPPER ${IO_IMPL} IO_IMPL_UPPER )
  
  if( VW_HAVE_PKG_${IO_IMPL_UPPER} )
  
    set( HEADERS ${HEADERS} DiskImageResource${IO_IMPL}.h )
    set( SOURCES ${SOURCES} DiskImageResource${IO_IMPL}.cc )
    include_directories( ${${IO_IMPL_UPPER}_INCLUDE_DIR} )
    link_libraries( ${${IO_IMPL_UPPER}_LIBRARIES} )
    
  endif( VW_HAVE_PKG_${IO_IMPL_UPPER} )
  
endforeach( IO_IMPL )

if( ZLIB_FOUND )
  include_directories( ${ZLIB_INCLUDE_DIR} )
  link_libraries( ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

if( WIN32 )
  # ok, this might be sketchy, but jmorecfg.h redefines
  # INT32 and VC++ doesn't like that one bit. This works
  # around the compile error, but with unknown results...
  add_definitions(-DXMD_H)

  # add Ws2_32.lib for ntohl
  link_libraries(Ws2_32)
endif( WIN32 )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

# for windows, *.lib goes in lib, *.dll goes in bin 
#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
