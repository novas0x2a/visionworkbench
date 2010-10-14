vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS
  ${EXPORT_H}
  Datum.h
  GeoReference.h
  GeoReferenceBase.h
  GeoReferenceResourcePDS.h
  GeoTransform.h
  OrthoImageView.h
  PointImageManipulation.h
  Projection.h
  SimplePointImageManipulation.h
  ToastTransform.h
)

set( SOURCES
  Datum.cc
  GeoReference.cc
  GeoTransform.cc
  GeoReferenceResourcePDS.cc
  ToastTransform.cc
)

if( VW_HAVE_PKG_GDAL )
  set( HEADERS ${HEADERS} GeoReferenceResourceGDAL.h )
  set( SOURCES ${SOURCES} GeoReferenceResourceGDAL.cc )
  include_directories( ${GDAL_INCLUDE_DIR} )
  link_libraries( ${GDAL_LIBRARIES} )
endif( VW_HAVE_PKG_GDAL )

if( VW_HAVE_PKG_CAMERA )
  set( HEADERS ${HEADERS} CameraBBox.h )
  set( SOURCES ${SOURCES} CameraBBox.cc )
  link_libraries( vwCamera )
endif( VW_HAVE_PKG_CAMERA )

include_directories( ${PROJ4_INCLUDE_DIR} )

link_libraries( ${PROJ4_LIBRARIES} vwFileIO )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
