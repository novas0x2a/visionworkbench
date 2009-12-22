## create a macro to reduce boiler plate 
##-------------------------------------------------
macro( vw_setup_lib_vars MODULE_PREFIX MODULE_NAME )
  set( LIB_NAME  ${MODULE_PREFIX}${MODULE_NAME} )
  set( EXPORT_H  ${MODULE_NAME}Export.h )
  string( TOUPPER  "VW_${MODULE_NAME}_BUILD_DLL" EXPORT_DEFINE )
  if( ${VW_DEFAULT_LIB_TYPE} MATCHES "SHARED" )
    add_definitions(-D${EXPORT_DEFINE})
  endif( ${VW_DEFAULT_LIB_TYPE} MATCHES "SHARED" )
endmacro( vw_setup_lib_vars )

## add all enabled modules and install module header
##-------------------------------------------------
foreach( MODULE_NAME ${MODULE_NAMES} )
  if( VW_BUILD_${MODULE_NAME} )
    add_subdirectory( ${MODULE_NAME} )
    install_files( /include/${MODULE_PREFIX} FILES ${MODULE_NAME}.h )
  endif( VW_BUILD_${MODULE_NAME} )
endforeach( MODULE_NAME )

## install vw.h header
##--------------------------------------------------
install_files( /include/${MODULE_PREFIX} FILES vw.h )

## add tools and gui dirs
##--------------------------------------------------
add_build_switch( tools  BUILD_DEFAULT_TRUE )
add_build_switch( gui    QT4_FOUND AND )

