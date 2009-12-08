set( MODULE_NAME Plate )
set( LIB_NAME    ${MODULE_PREFIX}${MODULE_NAME} )

PROTOBUF_GENERATE_CPP( PROTO_SOURCES PROTO_HEADERS ProtoBuffers.proto )

set( HEADERS 
  ${PROTO_HEADERS}
  Amqp.h
  Blob.h 
  BlobManager.h 
  Exception.h 
  Index.h
  IndexManagerService.h
  IndexService.h 
  KmlPlateManager.h
  LocalIndex.h 
  PlateFile.h 
  PlateManager.h 
  PlateView.h
  RemoteIndex.h 
  RpcServices.h 
  ToastPlateManager.h 
  Tree.h 
)

set( SOURCES
  ${PROTO_SOURCES}
  Amqp.cc 
  Blob.cc 
  BlobManager.cc 
  Index.cc 
  IndexManagerService.cc
  IndexService.cc 
  LocalIndex.cc	
  PlateFile.cc 
  RemoteIndex.cc 
  RpcServices.cc
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

