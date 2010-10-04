vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

#-----------------------------
if( PROTOBUF_FOUND )
  set( PROTOCOL_HEADERS
	TsaiFile.pb.h
  )
  set( PROTOCOL_SOURCES
	TsaiFile.pb.cc
  )
endif( PROTOBUF_FOUND )

#-----------------------------
if( LAPACK_FOUND )
  set( LSM_HEADERS
    Extrinsics.h 
    LensDistortion.h
    LinearPushbroomModel.h
    LinescanModel.h 
    OrbitingPushbroomModel.h 
    PinholeModelCalibrate.h 
    CameraGeometry.h 
  )
  set( LSM_SOURCES
    PinholeModel.cc 
    LensDistortion.cc 
  )
  
  
  add_definitions( ${LAPACK_DEFINITIONS} )
  link_libraries(  ${LAPACK_LIBRARIES} )
  
endif( LAPACK_FOUND )

#-----------------------------
set( HEADERS
  ${EXPORT_H}
  ${LSM_HEADERS} 
  ${PROTOCOL_HEADERS}
  BayerFilter.h
  CAHVModel.h 
  CAHVORModel.h
  CameraModel.h 
  CameraTransform.h 
  ExifData.h 
  Exif.h 
  PinholeModel.h
)

set( SOURCES 
  ${LSM_SOURCES}
  ${PROTOCOL_SOURCES}
  CAHVModel.cc 
  CAHVORModel.cc
  CameraModel.cc
  Exif.cc 
  ExifData.cc 
  LensDistortion.cc
  PinholeModel.cc
)

link_libraries( 
  ${Boost_IOSTREAMS_LIBRARY} 
  vwFileIO 
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
