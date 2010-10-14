vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

if( LAPACK_FOUND )
  set( LAPACK_HEADERS
    Geometry.h
    LapackExports.h
    LevenbergMarquardt.h
    LinearAlgebra.h
    PoseEstimation.h
  )
  add_definitions( ${LAPACK_DEFINITIONS} )
  link_libraries(  ${LAPACK_LIBRARIES} )
endif( LAPACK_FOUND )


set( HEADERS
  ${EXPORT_H}
  ${LAPACK_HEADERS}
  BBox.h
  ConjugateGradient.h
  DisjointSet.h
  EulerAngles.h
  Functions.h
  Functors.h
  KDTree.h
  Matrix.h
  MatrixSparseSkyline.h
  MinimumSpanningTree.h
  NelderMead.h
  ParticleSwarmOptimization.h
  Quaternion.h
  RANSAC.h
  Statistics.h
  Vector.h
)


set( SOURCES
  MinimumSpanningTree.cc
)

link_libraries( vwCore )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )


















