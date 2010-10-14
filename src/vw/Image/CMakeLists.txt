vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS
  ${EXPORT_H}
  Algorithms.h
  BlockProcessor.h
  BlockRasterize.h
  Convolution.h
  EdgeExtend.h
  EdgeExtension.h
  Filter.h
  Filter.tcc
  ImageIO.h
  ImageMath.h
  ImageResource.h
  ImageResourceImpl.h
  ImageResourceView.h
  ImageView.h
  ImageViewBase.h
  ImageViewRef.h
  Interpolation.h
  Manipulation.h
  MaskViews.h
  Palette.h
  PerPixelAccessorViews.h
  PerPixelViews.h
  PixelAccessors.h
  PixelIterator.h
  PixelMask.h
  PixelMath.h
  PixelTypeInfo.h
  PixelTypes.h
  SparseImageCheck.h
  Statistics.h
  Transform.h
  UtilityViews.h
  ViewImageResource.h
)

set( SOURCES
  Filter.cc
  ImageResource.cc
  Interpolation.cc
  PixelTypeInfo.cc
)

if (OPENCV_FOUND)
  set( HEADERS ${HEADERS} ImageResourceOpenCV.h )
  set( SOURCES ${SOURCES} ImageResourceOpenCV.cc )
  include_directories( ${OPENCV_INCLUDE_DIR} )
  link_libraries( ${OPENCV_LIBRARIES} )
endif (OPENCV_FOUND)

link_libraries(
  ${Boost_SYSTEM_LIBRARY}
  vwMath
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
