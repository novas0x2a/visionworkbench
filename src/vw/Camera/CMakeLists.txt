set( MODULE_NAME Camera )
set( LIB_NAME ${MODULE_PREFIX}${MODULE_NAME} )

#-----------------------------
if( LAPACK_FOUND )
  set( LSM_HEADERS
    Extrinsics.h 
    LensDistortion.h
    LinearPushbroomModel.h
    LinescanModel.h 
    OrbitingPushbroomModel.h 
    PinholeModel.h 
    PinholeModelCalibrate.h 
  )
  
  set( BA_HEADERS
    BundleAdjust.h 
    BundleAdjustModelBase.h
    BundleAdjustReport.h 
    BundleAdjustmentBase.h
    BundleAdjustmentRef.h 
    BundleAdjustmentRobustRef.h
    BundleAdjustmentRobustSparse.h
    BundleAdjustmentSparse.h 
  )
  
  set( BA_SOURCES
    BundleAdjustReport.cc
  )
endif( LAPACK_FOUND )

#-----------------------------
set( HEADERS
  ${BA_HEADERS}
  ${LSM_HEADERS} 
  BayerFilter.h
  CAHVModel.h 
  CAHVORModel.h
  CameraModel.h 
  CameraTransform.h 
  ControlNetwork.h
  Exif.h 
  ExifData.h 
)

set( SOURCES 
  ${BA_SOURCES}
  PinholeModel.cc 
  CAHVModel.cc 
  CAHVORModel.cc
	Exif.cc 
  ExifData.cc 
  ControlNetwork.cc
)

link_libraries( ${Boost_IOSTREAMS_LIBRARY} )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
