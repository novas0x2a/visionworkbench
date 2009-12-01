set( MODULE    Mosaic )
set( LIB_NAME  ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
  CelestiaQuadTreeConfig.h
  GMapQuadTreeConfig.h
  ImageComposite.h
  KMLQuadTreeConfig.h
  QuadTreeGenerator.h
  TMSQuadTreeConfig.h
  ToastQuadTreeConfig.h
  UniviewQuadTreeConfig.h
)
set( SOURCES
  CelestiaQuadTreeConfig.cc
  GMapQuadTreeConfig.cc
  KMLQuadTreeConfig.cc
  QuadTreeGenerator.cc
  TMSQuadTreeConfig.cc
  UniviewQuadTreeConfig.cc
)
  
add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )

