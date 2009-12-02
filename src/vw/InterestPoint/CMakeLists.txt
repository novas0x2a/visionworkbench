set( MODULE_NAME InterestPoint )
set( LIB_NAME ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
  Descriptor.h
  Detector.h
  Extrema.h
  ImageOctave.h
  ImageOctaveHistory.h
  IntegralImage.h
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
  InterestData.cc
)

link_libraries( ${LAPACK_LIBRARIES} )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

# for windows, *.lib goes in lib, *.dll goes in bin 
#-----------------------------------------------------------
install_library( ${LIB_NAME} )
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
