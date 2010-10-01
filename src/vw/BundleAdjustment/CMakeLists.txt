vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

if( MAKE_MODULE_INTERESTPOINT AND MAKE_MODULE_STEREO )
  set( RELATION_HEADERS
    CameraRelation.h
    ControlNetworkLoader.h
  )
  set( RELATION_SOURCES
    CameraRelation.cc
    ControlNetworkLoader.cc
  )
endif( MAKE_MODULE_INTERESTPOINT AND MAKE_MODULE_STEREO )



set( HEADERS 
  ${EXPORT_H}
  ${RELATION_HEADERS}
  AdjustBase.h
  AdjustRef.h
  AdjustRobustRef.h
  AdjustRobustSparse.h
  AdjustRobustSparseKGCP.h  
  AdjustSparse.h
  BundleAdjustReport.h
  ControlNetwork.h
  ModelBase.h
)

set( SOURCES
  ${RELATION_SOURCES}
  BundleAdjustReport.cc
  ControlNetwork.cc
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
