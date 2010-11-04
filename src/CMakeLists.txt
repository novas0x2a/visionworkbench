# Define the vw modules...
##################################################
set( MODULE_PREFIX "vw" )

#in rough dependency order
set( MODULE_NAMES
  Core
  Math
  Geometry
  Image
  FileIO
  InterestPoint
  Camera
  Cartography
  Mosaic
  Stereo
  BundleAdjustment
  HDR
  GPU
  Plate
  Python
  tools
)

## Temp variables for build flag defaults.
## These are used only to initialize the cache
## The initial values use the same syntax as the
## add_build_* macros so complete boolean expressions
## can be formed.
## So don't use "TRUE" use "BUILD_DEFAULT_TRUE"
## and don't dereference FOO_FOUND variables
##################################################
set( Core_DEFAULT          Boost_FOUND  )
set( Math_DEFAULT          VW_BUILD_Core )
set( Geometry_DEFAULT      VW_BUILD_Math )
set( Image_DEFAULT         VW_BUILD_Math  )
set( FileIO_DEFAULT        VW_BUILD_Image  )
set( InterestPoint_DEFAULT VW_BUILD_FileIO AND LAPACK_FOUND )
set( Camera_DEFAULT        VW_BUILD_FileIO  )
set( Cartography_DEFAULT   VW_BUILD_Camera AND PROJ4_FOUND )
set( Mosaic_DEFAULT        VW_BUILD_Cartography )
set( Stereo_DEFAULT        VW_BUILD_Camera AND LAPACK_FOUND )
set( BundleAdjustment_DEFAULT VW_BUILD_Stereo AND VW_BUILD_Cartography AND VW_BUILD_InterestPoint )
set( HDR_DEFAULT           VW_BUILD_Camera AND LAPACK_FOUND )
set( Plate_DEFAULT         PROTOBUF_FOUND AND BUILD_DEFAULT_FALSE ) # needs AMQP
set( tools_DEFAULT         BUILD_DEFAULT_TRUE )

## Create options (with roversw naming convention)
## to turn modules on/off
##################################################
foreach( MODULE_NAME ${MODULE_NAMES} )
  # initialize any modules we've missed with false
  if( NOT ${MODULE_NAME}_DEFAULT )
    set( ${MODULE_NAME}_DEFAULT BUILD_DEFAULT_FALSE )
  endif( NOT ${MODULE_NAME}_DEFAULT )

  add_build_var( ${MODULE_NAME} ${${MODULE_NAME}_DEFAULT} )

  # set vw variables based on cache variables
  string(TOUPPER ${MODULE_NAME} UPPER_NAME)
  set( VW_HAVE_PKG_${UPPER_NAME} ${VW_BUILD_${MODULE_NAME}} )
endforeach( MODULE_NAME )


## External Package defines for config.h creation
##################################################
set( VW_HAVE_PKG_BOOST                  ${Boost_FOUND}   )
set( VW_HAVE_PKG_BOOST_DATETIME         ${Boost_DATE_TIME_LIBRARY} )
set( VW_HAVE_PKG_BOOST_FILESYSTEM       ${Boost_FILESYSTEM_LIBRARY} )
set( VW_HAVE_PKG_BOOST_GRAPH            ${Boost_GRAPH_LIBRARY} )
set( VW_HAVE_PKG_BOOST_IOSTREAMS        ${Boost_IOSTREAMS_LIBRARY} )
set( VW_HAVE_PKG_BOOST_PROGRAM_OPTIONS  ${Boost_PROGRAM_OPTIONS_LIBRARY} )
set( VW_HAVE_PKG_BOOST_REGEX            ${Boost_REGEX_LIBRARY} )
set( VW_HAVE_PKG_BOOST_SYSTEM           ${Boost_SYSTEM_LIBRARY} )
set( VW_HAVE_PKG_BOOST_THREAD           ${Boost_THREAD_LIBRARY} )
set( VW_HAVE_PKG_GDAL                   ${GDAL_FOUND}    )

set( VW_HAVE_PKG_OPENCV                 ${OpenCV_FOUND}  )
set( VW_HAVE_PKG_GL                     ${OPENGL_FOUND}  )
set( VW_HAVE_PKG_JPEG                   ${JPEG_FOUND}    )
set( VW_HAVE_PKG_OPENEXR                ${OPENEXR_FOUND} )
set( VW_HAVE_PKG_ILMBASE                ${OPENEXR_FOUND} )
set( VW_HAVE_PKG_PNG                    ${PNG_FOUND}     )
set( VW_HAVE_PKG_PROJ4                  ${PROJ4_FOUND}   )
set( VW_HAVE_PKG_TIFF                   ${TIFF_FOUND}    )
set( VW_HAVE_PKG_Z                      ${ZLIB_FOUND}    )
set( VW_HAVE_PKG_LAPACK                 ${LAPACK_FOUND}  )

## check for some headers. Don't think this is necessary
## but do it anyway just for kicks and wiggles
##################################################
include(CheckIncludeFiles)
check_include_files( fenv.h      VW_HAVE_FENV_H      )
check_include_files( inttypes.h  VW_HAVE_INTTYPES_H  )
check_include_files( memory.h    VW_HAVE_MEMORY_H    )
check_include_files( pwd.h       VW_HAVE_PWD_H       )
check_include_files( stdint.h    VW_HAVE_STDINT_H    )
check_include_files( stdlib.h    VW_HAVE_STDLIB_H    )
check_include_files( string.h    VW_HAVE_STRING_H    )
check_include_files( strings.h   VW_HAVE_STRINGS_H   )
check_include_files( sys/stat.h  VW_HAVE_SYS_STAT_H  )
check_include_files( sys/types.h VW_HAVE_SYS_TYPES_H )
check_include_files( unistd.h    VW_HAVE_UNISTD_H    )

## type checks
##################################################
include(CheckDataTypes) 
set( VW_HAVE_SSIZE_T ${HAVE_SSIZE_T} )

## check for functions
##################################################
include(CheckFunctionExists)
check_function_exists( exp2           VW_HAVE_EXP2           )
check_function_exists( fabsl          VW_HAVE_FABSL          )
check_function_exists( feenableexcept VW_HAVE_FEENABLEEXCEPT )
check_function_exists( getpid         VW_HAVE_GETPID         )
check_function_exists( getpwuid       VW_HAVE_GETPWUID       )
check_function_exists( llabs          VW_HAVE_LLABS          )
check_function_exists( log2           VW_HAVE_LOG2           )
check_function_exists( tgamma         VW_HAVE_TGAMMA         )

## VW Configuration defines for config.h creation
##################################################
set( VW_BOOST_VERSION       ${Boost_VERSION} )
set( VW_HAS_DECLSPEC        ${DO_USE_DECLSPEC} CACHE BOOL "Enable dllexport/dllimport macros")
set( VW_ENABLE_EXCEPTIONS   1 CACHE STRING "Enable exceptions" )
set( VW_ENABLE_BOUNDS_CHECK 0 CACHE STRING "Enable image bounds checking (SLOW!)" )
set( VW_ENABLE_CONFIG_FILE  1 CACHE STRING "Enable ~/.vwrc support" )
set( VW_NUM_THREADS         4 CACHE STRING "Define to the default number of threads that the Vision Workbench should spawn for multi-threaded operations.")

## Directories
##################################################
add_subdirectory( vw )

set( VW_HAVE_PKG_TOOLS ${VW_BUILD_tools} )
set( VW_HAVE_PKG_GUI   ${VW_BUILD_gui}   )

## Create the config files in the out-of-source tree
##################################################
if( WIN32 )
  include( SetupConfigureFile )
  setup_win32_workarounds( vw )
endif( WIN32 )
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/config.h.in.cmake
  ${PROJECT_BINARY_DIR}/src/vw/config.h
)
install(FILES ${PROJECT_BINARY_DIR}/src/vw/config.h
        DESTINATION include/vw )

