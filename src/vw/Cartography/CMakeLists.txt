set( MODULE_NAME Cartography )
set( LIB_NAME ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
  CameraBBox.h
  Datum.h
  FileIO.h
  GeoReference.h
  GeoReferenceBase.h
  GeoReferenceResourcePDS.h
  GeoTransform.h
  OrthoImageView.h
  PointImageManipulation.h
  Projection.h
)

set( SOURCES 
  CameraBBox.cc
  Datum.cc
  GeoReference.cc
  GeoReferenceResourcePDS.cc
  GeoTransform.cc
)

if( VW_HAVE_PKG_GDAL )
  set( HEADERS ${HEADERS} GeoReferenceResourceGDAL.h )
  set( SOURCES ${SOURCES} GeoReferenceResourceGDAL.cc )
  include_directories( ${GDAL_INCLUDE_DIR} )
  link_libraries( ${GDAL_LIBRARIES} )
endif( VW_HAVE_PKG_GDAL )

include_directories( ${PROJ4_INCLUDE_DIR} )

link_libraries( ${PROJ4_LIBRARIES} )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
