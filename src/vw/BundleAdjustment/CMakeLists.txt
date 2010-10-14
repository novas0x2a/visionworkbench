vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

#-----------------------------
set( HEADERS
  ${EXPORT_H}
  AdjustBase.h
  AdjustRef.h
  AdjustRobustRef.h
  AdjustRobustSparse.h
  AdjustRobustSparseKGCP.h
  AdjustSparse.h
  BundleAdjustReport.h
  CameraRelation.h
  ControlNetwork.h
  ControlNetworkLoader.h
  ModelBase.h
)

set( SOURCES
  ${LSM_SOURCES}
  BundleAdjustReport.cc
  ControlNetwork.cc
  CameraRelation.cc
  ControlNetworkLoader.cc
)

link_libraries(
  vwInterestPoint
  vwCartography
  vwStereo
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
