project(vw)
cmake_minimum_required(VERSION 2.6)

## if ../cmake/Modules exists, then use that and 
## set default install dir to ../${ARCHITECTURE}
## (used when pulled in as part of roversw)
##################################################
set(ROOTDIR ${PROJECT_SOURCE_DIR} ) 
if( EXISTS ${PROJECT_SOURCE_DIR}/../cmake/Modules )
  set(ROOTDIR ${PROJECT_SOURCE_DIR}/.. )
endif( EXISTS ${PROJECT_SOURCE_DIR}/../cmake/Modules )

## add the irg_cmake modules to the head
## of our cmake module search path 
##################################################
set( CMAKE_MODULE_PATH ${ROOTDIR}/cmake/Modules ${CMAKE_MODULE_PATH} )

include( UserOptions )
include( SetArchitecture )
include( SetupWarnLevel )
include( SetupRPATH )
include( SetupInstall )
include( AddBuildSwitch )
string( TOUPPER ${PROJECT_NAME} PROJECT_UPPER )
build_switch_init( ${PROJECT_UPPER} )

## we have to play some tricks to get install
## path to "stick" in Windows.
##################################################
if( NOT VW_INSTALL_PREFIX )
  set( VW_INSTALL_PREFIX "${ROOTDIR}/${ARCHITECTURE}" CACHE PATH "VisionWorkbench install path")
endif( NOT VW_INSTALL_PREFIX )
set( CMAKE_INSTALL_PREFIX ${VW_INSTALL_PREFIX} CACHE INTERNAL "" )

## Call RPATH macro after CMAKE_INSTALL_PREFIX has been set
##################################################
setup_rpath()

# If user specified VW_BUILD_TYPE in options file
# force that value into the cache
######################################################
if(VW_BUILD_TYPE)
  set( CMAKE_BUILD_TYPE ${VW_BUILD_TYPE} CACHE STRING
      "Overridden by VW_BUILD_TYPE" FORCE )
endif(VW_BUILD_TYPE)


## find the packages we depend on 
##################################################
set( VW_BOOST_COMPONENTS system filesystem program_options thread iostreams)
if( WIN32 )
  # boost wants to pull these components in as well on win32
  set( VW_BOOST_COMPONENTS ${VW_BOOST_COMPONENTS} date_time )
endif( WIN32 ) 
# boost is a required dep; bail if it's not found
find_package( Boost REQUIRED COMPONENTS ${VW_BOOST_COMPONENTS} )
message(STATUS "VW will be built using Boost version ${Boost_VERSION}")

if(WIN32) 
  find_package( OpenSceneGraph3rdParty )
  find_package( win32LAPACK )
else(WIN32)
  find_package( JPEG )
  find_package( ZLIB )
  find_package( PNG )
  find_package( TIFF )
  find_package( LAPACK ) 
endif(WIN32)

find_package( OpenEXR )
find_package( GDAL )
find_package( OpenGL )
find_package( Proj4 )
#find_package( Protobuf ) # part of CMake 2.8, not in CMake 2.6
find_package( Qt4 )

## Add the in-source and out-of-source 
## include paths by default 
##################################################
include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}/src"
)


## Add some other project defaults
##################################################
include_directories( ${Boost_INCLUDE_DIR} )
if( NOT Boost_USE_STATIC_LIBS )
  # dynamic linking boost on windows will not
  # work at all without this magic define
  add_definitions( -DBOOST_ALL_DYN_LINK )
endif( NOT Boost_USE_STATIC_LIBS )

set( VW_DEFAULT_LIB_TYPE SHARED )
if( WIN32 )
  # build static under win32 until proper __declspec
  # macros are created and added to codebase
  set( VW_DEFAULT_LIB_TYPE STATIC )
  
  ## test shared libraries
  #set( VW_DEFAULT_LIB_TYPE SHARED )
  #add_definitions( -DVW_ALL_DYN_LINK )

  link_directories( ${Boost_LIBRARY_DIRS} )

  # not sure why LIBCMT is being pulled in... this
  # cures the symptom, not the cause. Warrants 
  # further investigation
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT" )
endif( WIN32 )

## Directories
##################################################
add_subdirectory( src )

## install the export file
##################################################
install_export()

## print the build switches
##################################################
print_build_switches()

