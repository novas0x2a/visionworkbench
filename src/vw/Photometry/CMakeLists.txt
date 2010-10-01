vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS 
  ${EXPORT_H}
  Albedo.h
  Camres.h
  Exposure.h
  Index.h
  Misc.h
  Outlier.h
  Reconstruct.h
  ReconstructError.h
  Reflectance.h
  Shadow.h
  ShapeFromShading.h
  Shape.h
  Weights.h
)

set( SOURCES 
  Albedo.cc
  Camres.cc
  Exposure.cc
  Index.cc
  Misc.cc
  Outlier.cc
  Reflectance.cc
  Shadow.cc
  Shape.cc
  ShapeFromShading.cc
  Weights.cc
  Reconstruct.cc
  ReconstructError.cc
)

link_libraries(
  ${LAPACK_LIBRARIES}
  vwCamera
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
