set( MODULE_NAME Stereo )
set( LIB_NAME ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
  Correlate.h 
  CorrelatorView.h
  DisparityMap.h 
  OptimizedCorrelator.h 
  PyramidCorrelator.h 
  ReferenceCorrelator.h	
  StereoModel.h	
  StereoView.h 
)

set( SOURCES 
  Correlate.cc
  OptimizedCorrelator.cc
  PyramidCorrelator.cc 
  StereoModel.cc 
)

add_definitions( ${LAPACK_DEFINITIONS} )

link_libraries(
  ${LAPACK_LIBRARIES}
  vwCore 
  vwFileIO
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
