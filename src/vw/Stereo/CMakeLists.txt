set( MODULE_NAME Stereo )
set( LIB_NAME ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
	OptimizedCorrelator.h 
	PyramidCorrelator.h 
  Correlate.h 
  CorrelatorView.h
  DisparityMap.h 
  ReferenceCorrelator.h	
  StereoModel.h	
  StereoView.h 
)

set( SOURCES 
  StereoModel.cc 
  OptimizedCorrelator.cc
	PyramidCorrelator.cc 
  Correlate.cc
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
