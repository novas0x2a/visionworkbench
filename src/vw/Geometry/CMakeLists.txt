set( MODULE    Geometry )
set( LIB_NAME  ${MODULE_PREFIX}${MODULE_NAME} )


set( HEADERS 
  ATrans.h
  Box.h
  Frame.h
  FrameTreeNode.h
  PointListIO.h
  Shape.h
  #SpatialTree.h
  Sphere.h
  TreeNode.h
)                       
                        
set( SOURCES
  #SpatialTree.cc
  FrameTreeNode.cpp
)

link_libraries( vwCore )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
