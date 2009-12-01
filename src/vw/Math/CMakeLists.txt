set( MODULE    Math )
set( LIB_NAME  ${MODULE_PREFIX}${MODULE_NAME} )

if( LAPACK_FOUND )
  set( LAPACK_HEADERS 
    LinearAlgebra.h 
    LevenbergMarquardt.h 
    Geometry.h  
    PoseEstimation.h 
    LapackExports.h
  )
endif( LAPACK_FOUND )


set( HEADERS 
  ${LAPACK_HEADERS}
  BBox.h                     
  ConjugateGradient.h        
  DisjointSet.h              
  EulerAngles.h              
  Functions.h                
  Functors.h                 
  KDTree.h                   
  Matrix.h                        
  MinimumSpanningTree.h           
  NelderMead.h                    
  ParticleSwarmOptimization.h
  Quaternion.h               
  RANSAC.h               
  Statistics.h               
  SparseSkylineMatrix.h        
  Vector.h                   
)                       
                        
set( SOURCES
  MinimumSpanningTree.cc
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )


















