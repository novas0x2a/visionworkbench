vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS 
  ${EXPORT_H}
  AffineMixtureComponent.h 
  Correlate.h 
  CorrelateResearch.h
  CorrelatorView.h	
  DisparityMap.h 
  EMSubpixelCorrelatorView.h 		
  EMSubpixelCorrelatorView.hpp
  GammaMixtureComponent.h 		
  GaussianMixtureComponent.h				
  MixtureComponent.h 
  OptimizedCorrelator.h 
  PyramidCorrelator.h 
  ReferenceCorrelator.h		
  StereoModel.h	
  StereoView.h 
  SubpixelView.h 
  UniformMixtureComponent.h	
)

set( SOURCES 
  Correlate.cc
  CorrelateResearch.cc
  EMSubpixelCorrelatorView.cc
  OptimizedCorrelator.cc
  PyramidCorrelator.cc 
  StereoModel.cc 
)

add_definitions( ${LAPACK_DEFINITIONS} )

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
