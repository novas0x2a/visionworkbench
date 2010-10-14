vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

#-----------------------------
if( LAPACK_FOUND )
  set( LSM_HEADERS
    CameraGeometry.h
    Extrinsics.h
    LensDistortion.h
    LinearPushbroomModel.h
    LinescanModel.h
    OrbitingPushbroomModel.h
    PinholeModelCalibrate.h
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
  BayerFilter.h
  CAHVModel.h
  CAHVORModel.h
  CameraModel.h
  CameraTransform.h
  Exif.h
  ExifData.h
  PinholeModel.h
)

set( SOURCES
  ${LSM_SOURCES}
  CAHVModel.cc
  CAHVORModel.cc
  CameraModel.cc
  Exif.cc
  ExifData.cc
)

if( PROTOBUF_FOUND )
  PROTOBUF_GENERATE_CPP( PROTO_SOURCES PROTO_HEADERS TsaiFile.proto )
  set( HEADERS ${HEADERS} ${PROTO_HEADERS} )
  set( SOURCES ${SOURCES} ${PROTO_SOURCES} )
  include_directories( ${PROTOBUF_INCLUDE_DIRS} )
  link_libraries( ${PROTOBUF_LIBRARIES} )
endif (PROTOBUF_FOUND )


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
