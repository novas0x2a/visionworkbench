vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

PROTOBUF_GENERATE_CPP( PROTO_SOURCES PROTO_HEADERS
    IndexService.proto
    Rpc.proto
)

set( HEADERS
  ${EXPORT_H}
  ${PROTO_HEADERS}
  AmqpConnection.h
  Blob.h
  BlobManager.h
  common.h
  Exception.h
  HTTPUtils.h
  Index.h
  IndexPage.h
  IndexService.h
  LocalIndex.h
  PagedIndex.h
  PlateCarreePlateManager.h
  PlateFile.h
  PlateManager.h
  PlateView.h
  PolarStereoPlateManager.h
  RemoteIndex.h
  RpcServices.h
  SnapshotManager.h
  TileManipulation.h
  ToastDem.h
  ToastPlateManager.h
)

set( SOURCES
  ${PROTO_SOURCES}
  AmqpConnection.cc
  Blob.cc
  BlobManager.cc
  HTTPUtils.cc
  Index.cc
  IndexPage.cc
  IndexService.cc
  LocalIndex.cc
  PagedIndex.cc
  PlateCarreePlateManager.cc
  PlateFile.cc
  PlateManager.cc
  PolarStereoPlateManager.cc
  RemoteIndex.cc
  RpcServices.cc
  SnapshotManager.cc
  TileManipulation.cc
  ToastDem.cc
  ToastPlateManager.cc
)

include_directories(
  ${PROTOBUF_INCLUDE_DIRS}
)

link_libraries(
  ${PROTOBUF_LIBRARIES}
  vwCore
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} )
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )

