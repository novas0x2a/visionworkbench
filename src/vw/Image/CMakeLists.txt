vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

if( OPENCV_FOUND )
  set( OPENCV_HEADERS 
	ImageResourceOpenCV.h
  )
  set( OPENCV_SOURCES
    ImageResourceOpenCV.cc
  )
endif( OPENCV_FOUND )

set( HEADERS 
  ${EXPORT_H}
  ${OPENCV_HEADERS}
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
  ImageViewBase.h
  ImageView.h
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
  ${OPENCV_SOURCES}
  Filter.cc
  ImageResource.cc
  Interpolation.cc
  PixelTypeInfo.cc
)

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
