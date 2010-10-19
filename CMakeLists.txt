project(vw)
cmake_minimum_required(VERSION 2.6)
#cmake_minimum_required(VERSION 2.6.3)
# relax version enforcement until cmake on RHWS5 machines is updated
# (currently at 2.6.0, version 2.8.0 update is pending)
# cmake < 2.6.3 is broken on OSX

## Many of these are set in configure.ac in AC_INIT
## We duplicate them here because it's easier
## than trying to parse that file
##################################################
set( VW_VERSION_MAJOR      2         )
set( VW_VERSION_MINOR      1         )
set( VW_VERSION_PATCH      0_post    )
set( VW_VERSION            "${VW_VERSION_MAJOR}.${VW_VERSION_MINOR}.${VW_VERSION_PATCH}")
set( VW_PACKAGE            "VisionWorkbench"                  )
set( VW_PACKAGE_BUGREPORT  "vision-workbench@lists.nasa.gov"  )
set( VW_PACKAGE_NAME       "NASA ${VW_PACKAGE}"               )
set( VW_PACKAGE_STRING     "${VW_PACKAGE_NAME} ${VW_VERSION}" )
set( VW_PACKAGE_TARNAME    "${VW_PACKAGE}"                    )
set( VW_PACKAGE_VERSION    "${VW_VERSION}"                    )
set( VW_PACKAGE_URL        "http://ti.arc.nasa.gov/project/nasa-vision-workbench/")
set( VW_SOVERSION_MAJOR    2 ) # epoch
set( VW_SOVERSION_MINOR    1 ) # revision
set( VW_SOVERSION_PATCH    99) # age
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

## disable strict aliasing
if( CMAKE_COMPILER_IS_GNUCXX )
  set( WARN_FLAG_ADDITIONS "-fno-strict-aliasing -Wextra" )
endif( CMAKE_COMPILER_IS_GNUCXX )

include( UserOptions )
include( SetArchitecture )
include( SetupBuildSwitch )
include( SetupInstall )
include( SetupRPATH )
include( SetupUtilities )
include( SetupWarnLevel )

## we have to play some tricks to get install
## path to "stick" in Windows.
##################################################
if( NOT VW_INSTALL_PREFIX )
  set( VW_INSTALL_PREFIX "${ROOTDIR}/${ARCHITECTURE}" CACHE PATH "VisionWorkbench install path")
endif( NOT VW_INSTALL_PREFIX )
set( CMAKE_INSTALL_PREFIX ${VW_INSTALL_PREFIX} CACHE INTERNAL "" )

## Call RPATH macro after CMAKE_INSTALL_PREFIX has 
## been set. This sets up install_name on OSX, too
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
        date_time
        filesystem
        graph
        iostreams
        program_options
        regex
        system
        thread
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

minimum_requirements( Boost_FOUND )

## modify and extend find results where necessary
##################################################
# if gdal has tiff support, disable native tiff
if( GDAL_FOUND AND GDAL_HAS_TIFF )
  set( TIFF_FOUND FALSE )
  set( TIFF_LIBRARIES "")
  set( VW_HAS_BIGTIFF ${GDAL_HAS_BIGTIFF} )
else( GDAL_FOUND AND GDAL_HAS_TIFF )
  if( TIFF_FOUND )
    include( CheckLibraryExists )
    check_library_exists( ${TIFF_LIBRARY} TIFFScanlineSize64 "" VW_HAS_BIGTIFF )
  endif( TIFF_FOUND )
endif( GDAL_FOUND AND GDAL_HAS_TIFF )

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
set( DO_USE_DECLSPEC FALSE )

if( WIN32 )
  #set( VW_DEFAULT_LIB_TYPE STATIC )

  set( VW_DEFAULT_LIB_TYPE SHARED )
  add_definitions( -DVW_ALL_DYN_LINK ) # add this if building shared libs
  set( DO_USE_DECLSPEC TRUE )

  link_directories( ${Boost_LIBRARY_DIRS} )
endif( WIN32 )

## Directories
##################################################
add_subdirectory( src )

## install the export file
##################################################
install_export()

## print the build switches
##################################################
message( STATUS "----------------------------------------------")
message( STATUS "   ${VW_PACKAGE_STRING}" )
print_build_switches()

## test what CPack can do
##################################################
include( CPackLists.txt )

