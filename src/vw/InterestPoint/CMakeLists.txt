vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS
  ${EXPORT_H}
  BoxFilter.h
  Descriptor.h
  Detector.h
  Extrema.h
  ImageOctave.h
  ImageOctaveHistory.h
  IntegralDescriptor.h
  IntegralDetector.h
  IntegralImage.h
  IntegralInterestOperator.h
  InterestData.h
  InterestOperator.h
  InterestTraits.h
  LearnPCA.h
  Localize.h
  Matcher.h
  MatrixIO.h
  VectorIO.h
  WeightedHistogram.h
)


set( SOURCES
  Descriptor.cc
  IntegralDetector.cc
  IntegralInterestOperator.cc
  InterestData.cc
)

add_definitions( ${LAPACK_DEFINITIONS} )

link_libraries(
  ${LAPACK_LIBRARIES}
  vwFileIO
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

# for windows, *.lib goes in lib, *.dll goes in bin
#-----------------------------------------------------------
install_library( ${LIB_NAME} )
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
