vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

#-----------------------------
if( LAPACK_FOUND )
  set( LSM_HEADERS
    Extrinsics.h 
    LensDistortion.h
    LinearPushbroomModel.h
    LinescanModel.h 
    OrbitingPushbroomModel.h 
    PinholeModelCalibrate.h 
    PinholeModel.h 
  )
  set( LSM_SOURCES
    PinholeModel.cc 
    LensDistortion.cc 
  )
  
  set( BA_HEADERS
    BundleAdjust.h 
    BundleAdjustmentBase.h
    BundleAdjustmentRef.h 
    BundleAdjustmentRobustRef.h
    BundleAdjustmentRobustSparse.h
    BundleAdjustmentSparse.h 
    BundleAdjustModelBase.h
    BundleAdjustReport.h 
  )
  set( BA_SOURCES
    BundleAdjustReport.cc
  )
  
  add_definitions( ${LAPACK_DEFINITIONS} )
  link_libraries(  ${LAPACK_LIBRARIES} )
  
endif( LAPACK_FOUND )

#-----------------------------
set( HEADERS
  ${EXPORT_H}
  ${BA_HEADERS}
  ${LSM_HEADERS} 
  BayerFilter.h
  CAHVModel.h 
  CAHVORModel.h
  CameraModel.h 
  CameraTransform.h 
  ControlNetwork.h
  ExifData.h 
  Exif.h 
)

set( SOURCES 
  ${BA_SOURCES}
  ${LSM_SOURCES}
  CAHVModel.cc 
  CAHVORModel.cc
  ControlNetwork.cc
  Exif.cc 
  ExifData.cc 
)

link_libraries( 
  ${Boost_IOSTREAMS_LIBRARY} 
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
