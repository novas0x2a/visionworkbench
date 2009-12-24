vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

set( HEADERS 
  ${EXPORT_H}
  CameraCurve.h
  GlobalToneMap.h
  HDRExport.h
  LDRtoHDR.h
  LocalToneMap.h
)                       
                        
set( SOURCES
  CameraCurve.cc
  LocalToneMap.cc
)

link_libraries( 
  vwCamera
)

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )
