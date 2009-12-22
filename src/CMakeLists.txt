## These are set in configure.ac in AC_INIT, parse that
## file and grab them from there
##################################################
set( VW_VERSION           "2.0_beta5_pre"                    )
set( VW_PACKAGE           "VisionWorkbench"                  )
set( VW_PACKAGE_BUGREPORT "vision-workbench@lists.nasa.gov"  )
set( VW_PACKAGE_NAME      "NASA ${VW_PACKAGE}"               )
set( VW_PACKAGE_STRING    "${VW_PACKAGE_NAME} ${VW_VERSION}" )
set( VW_PACKAGE_TARNAME   "${VW_PACKAGE}"                    )
set( VW_PACKAGE_VERSION   "${VW_VERSION}"                    )


# Define the vw modules...
##################################################
set( MODULE_PREFIX "vw" )
set( MODULE_NAMES
  Camera
  Cartography
  Core
  FileIO
  Geometry
  GPU
  HDR
  Image
  InterestPoint
  Math
  Mosaic
  Plate
  Python
  Stereo
)

## Temp variables for build flag defaults. 
## These are used only to initialize the cache
##################################################
set( Camera_DEFAULT        TRUE  )
set( Cartography_DEFAULT   ${PROJ4_FOUND} )
set( Core_DEFAULT          TRUE  )
set( FileIO_DEFAULT        TRUE  )
set( GPU_DEFAULT           FALSE  )
set( Geometry_DEFAULT      TRUE  )
set( HDR_DEFAULT           FALSE  )
set( Image_DEFAULT         TRUE  )
set( InterestPoint_DEFAULT ${LAPACK_FOUND} )
set( Math_DEFAULT          TRUE  )
set( Mosaic_DEFAULT        TRUE  )
set( Plate_DEFAULT         ${PROTOBUF_FOUND} )
set( Python_DEFAULT        FALSE )
set( Stereo_DEFAULT        ${LAPACK_FOUND} ) 

## Create options (with roversw naming convention)
## to turn modules on/off
##################################################
foreach( MODULE_NAME ${MODULE_NAMES} )
  # not necessary, but set "FALSE" instead of empty var
  if( NOT ${MODULE_NAME}_DEFAULT )
    set( ${MODULE_NAME}_DEFAULT FALSE ) 
  endif( NOT ${MODULE_NAME}_DEFAULT )
  
  add_build_var( ${MODULE_NAME} ${MODULE_NAME}_DEFAULT )

  # set vw variables based on cache variables
  string(TOUPPER ${MODULE_NAME} UPPER_NAME)
  set( VW_HAVE_PKG_${UPPER_NAME} ${VW_BUILD_${MODULE_NAME}} )
endforeach( MODULE_NAME )


## External Package defines for config.h creation
##################################################
set( VW_HAVE_PKG_BOOST                  ${Boost_FOUND}   )
set( VW_HAVE_PKG_BOOST_FILESYSTEM       ${Boost_FILESYSTEM_LIBRARY} )
set( VW_HAVE_PKG_BOOST_PROGRAM_OPTIONS  ${Boost_PROGRAM_OPTIONS_LIBRARY} )
set( VW_HAVE_PKG_BOOST_THREAD           ${Boost_THREAD_LIBRARY} )
set( VW_HAVE_PKG_BOOST_IOSTREAMS        ${Boost_IOSTREAMS_LIBRARY} )
set( VW_HAVE_PKG_GDAL                   ${GDAL_FOUND}    )
set( VW_HAVE_PKG_GL                     ${OPENGL_FOUND}  )
set( VW_HAVE_PKG_JPEG                   ${JPEG_FOUND}    )
set( VW_HAVE_PKG_OPENEXR                ${OPENEXR_FOUND} )
set( VW_HAVE_PKG_PNG                    ${PNG_FOUND}     )
set( VW_HAVE_PKG_PROJ4                  ${PROJ4_FOUND}   )
set( VW_HAVE_PKG_TIFF                   ${TIFF_FOUND}    )
set( VW_HAVE_PKG_Z                      ${ZLIB_FOUND}    )
set( VW_HAVE_PKG_LAPACK                 ${LAPACK_FOUND}  )

## check for some headers. Don't think this is necessary 
## but do it anyway just for kicks and wiggles
##################################################
include(CheckIncludeFiles)
check_include_files( dlfcn.h     VW_HAVE_DLFCN_H     )
check_include_files( inttypes.h  VW_HAVE_INTTYPES_H  )
check_include_files( memory.h    VW_HAVE_MEMORY_H    )
check_include_files( pwd.h       VW_HAVE_PWD_H       )
check_include_files( stdint.h    VW_HAVE_STDINT_H    )
check_include_files( stdlib.h    VW_HAVE_STDLIB_H    )
check_include_files( strings.h   VW_HAVE_STRINGS_H   )
check_include_files( string.h    VW_HAVE_STRING_H    )
check_include_files( sys/stat.h  VW_HAVE_SYS_STAT_H  )
check_include_files( sys/types.h VW_HAVE_SYS_TYPES_H )
check_include_files( unistd.h    VW_HAVE_UNISTD_H    )

## VW Configuration defines for config.h creation
##################################################
set( VW_BOOST_VERSION       ${Boost_VERSION} )
set( VW_HAS_DECLSPEC        ${WIN32} CACHE BOOL "Enable dllexport/dllimport macros")
set( VW_ENABLE_EXCEPTIONS   1 CACHE STRING "Enable exceptions" )
set( VW_ENABLE_BOUNDS_CHECK 0 CACHE STRING "Enable image bounds checking (SLOW!)" )
set( VW_ENABLE_CONFIG_FILE  1 CACHE STRING "Enable ~/.vwrc support" )
set( VW_NUM_THREADS         4 CACHE STRING "Define to the default number of threads that the Vision Workbench should spawn for multi-threaded operations.")

## Create the config files in the out-of-source tree
##################################################
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/platform.h.in.cmake
  ${PROJECT_BINARY_DIR}/src/vw/platform.h
)
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/config.h.in.cmake
  ${PROJECT_BINARY_DIR}/src/vw/config.h
)

## print out some info for the user
##################################################
message( STATUS "----------------------------------------------")
message( STATUS "   ${VW_PACKAGE_STRING}" )

## Directories
##################################################
add_subdirectory( vw )

install(FILES ${PROJECT_BINARY_DIR}/src/vw/config.h
			  ${PROJECT_BINARY_DIR}/src/vw/platform.h
        DESTINATION include/vw )

