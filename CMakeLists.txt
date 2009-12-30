project(vw)
cmake_minimum_required(VERSION 2.6)

## Many of these are set in configure.ac in AC_INIT
## We duplicate them here because it's easier
## than trying to parse that file
##################################################
set( VW_VERSION_MAJOR      2         )
set( VW_VERSION_MINOR      0         )
set( VW_VERSION_PATCH      beta5_pre )
set( VW_VERSION            "${VW_VERSION_MAJOR}.${VW_VERSION_MINOR}.${VW_VERSION_PATCH}")
set( VW_PACKAGE            "VisionWorkbench"                  )
set( VW_PACKAGE_BUGREPORT  "vision-workbench@lists.nasa.gov"  )
set( VW_PACKAGE_NAME       "NASA ${VW_PACKAGE}"               )
set( VW_PACKAGE_STRING     "${VW_PACKAGE_NAME} ${VW_VERSION}" )
set( VW_PACKAGE_TARNAME    "${VW_PACKAGE}"                    )
set( VW_PACKAGE_VERSION    "${VW_VERSION}"                    )
set( VW_PACKAGE_URL        "http://ti.arc.nasa.gov/project/nasa-vision-workbench/")
set( VW_SOVERSION_MAJOR    4 ) # epoch
set( VW_SOVERSION_MINOR    0 ) # revision
set( VW_SOVERSION_PATCH    0 ) # age
set( VW_SOVERSION          "${VW_SOVERSION_MAJOR}.${VW_SOVERSION_MINOR}.${VW_SOVERSION_PATCH}" ) 

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
set( VW_BOOST_COMPONENTS 
        system 
        filesystem 
        program_options 
        thread 
        iostreams 
        date_time
   )
# boost is a required dep; bail if it's not found
find_package( Boost REQUIRED COMPONENTS ${VW_BOOST_COMPONENTS} )
message(STATUS "VW will be built using Boost version ${Boost_VERSION}")

find_package( JPEG )
find_package( ZLIB )
find_package( PNG )
find_package( TIFF )
find_package( OpenSceneGraph3rdParty ) # for jpeg, zlib, png & tiff on win32
find_package( OpenEXR )
find_package( LAPACK ) 
find_package( GDAL )
find_package( Proj4 )
find_package( Protobuf ) 
find_package( OpenGL )
find_package( Qt4 )

## Add the in-source and out-of-source 
## include paths by default 
##################################################
include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}/src"
)

## Boost setup
##################################################
include_directories( ${Boost_INCLUDE_DIR} )
if( NOT Boost_USE_STATIC_LIBS )
  # dynamic linking boost on windows will not
  # work at all without this magic define
  add_definitions( -DBOOST_ALL_DYN_LINK )
endif( NOT Boost_USE_STATIC_LIBS )


## Add some other project defaults
##################################################
set( VW_DEFAULT_LIB_TYPE SHARED )
if( WIN32 )
  #set( VW_DEFAULT_LIB_TYPE STATIC )
  
  set( VW_DEFAULT_LIB_TYPE SHARED )
  add_definitions( -DVW_ALL_DYN_LINK ) # add this if building shared libs
  
  link_directories( ${Boost_LIBRARY_DIRS} )
endif( WIN32 )

## Directories
##################################################
add_subdirectory( src )

## install the export file
##################################################
install_export()

## test what CPack can do
##################################################
include( CMakePack.txt )

## print the build switches
##################################################
print_build_switches()



#------------------------------------
include( GatherDlls )
gather_dlls( 
  ${Boost_LIBRARIES} 
  ${JPEG_LIBRARIES} 
  ${ZLIB_LIBRARIES}
  ${PNG_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${OPENEXR_LIBRARIES}
  ${LAPACK_LIBRARIES}
  ${GDAL_LIBRARIES}
  ${PROJ4_LIBRARIES}
)

