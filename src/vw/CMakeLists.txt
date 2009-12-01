foreach( MODULE_NAME ${MODULE_NAMES} )
  if( VW_BUILD_${MODULE_NAME} )
    add_subdirectory( ${MODULE_NAME} )
    install_files( /include/${MODULE_PREFIX} FILES ${MODULE_NAME}.h )
  endif( VW_BUILD_${MODULE_NAME} )
endforeach( MODULE_NAME )

install_files( /include/${MODULE_PREFIX} FILES vw.h )
add_build_switch( tools BUILD_DEFAULT_TRUE )
