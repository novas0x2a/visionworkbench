vw_setup_lib_vars( ${MODULE_PREFIX} ${MODULE_NAME} )

# ensure clean variable
set( TOOLS "" )

# First, the library
set( LIB_HEADERS Common.h )
set( LIB_SOURCES Common.cc )

add_library( ${LIB_NAME} STATIC
  ${LIB_HEADERS}
  ${LIB_SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} )
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${LIB_HEADERS} )


## common libs
link_libraries(
  ${LIB_NAME}
  vwCore
  vwFileIO
  vwImage
  vwMath
)
if( WIN32 )
  link_libraries(
    wsock32                    # needed for htonl
    ${Boost_DATE_TIME_LIBRARY} # win32 boost wants to pull this in
  )
endif( WIN32 )

## Camera tools
########################################
if( VW_HAVE_PKG_CAMERA AND VW_HAVE_PKG_LAPACK )

  set( PROGS print_exif bayer )

  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwCamera )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_CAMERA AND VW_HAVE_PKG_LAPACK )


## Cartography tools
########################################
if( VW_HAVE_PKG_CARTOGRAPHY )

  # XXX FIXME : slopemap has ambiguous 'algorithm' symbol (clashes with boost::algorithm)
  #set( PROGS colormap slopemap )
  set( PROGS colormap grassfirealpha )

  include_directories( ${PROJ4_INCLUDE_DIR} )

  if( VW_HAVE_PKG_GDAL )

    set( PROGS ${PROGS} georef hillshade)
    include_directories( ${GDAL_INCLUDE_DIR} )
  endif( VW_HAVE_PKG_GDAL )

  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwCartography )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_CARTOGRAPHY )


## HDR tools
########################################

## InterestPoint + LAPACK tools
########################################
if( VW_HAVE_PKG_INTERESTPOINT AND VW_HAVE_PKG_LAPACK )

  set( PROGS ipalign ipmatch ipfind learnpca )
  #set( PROGS ipalign ipmatch ipfind learnpca )
  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwInterestPoint ${LAPACK_LIBRARIES} )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_INTERESTPOINT AND VW_HAVE_PKG_LAPACK )


## Mosaic tools
########################################
if( VW_HAVE_PKG_MOSAIC )

  set( PROGS blend )
  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwMosaic  )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_MOSAIC )


## Mosaic + Cartography + GDAL tools
########################################
if( VW_HAVE_PKG_MOSAIC AND VW_HAVE_PKG_CARTOGRAPHY AND VW_HAVE_PKG_GDAL )

  set( PROGS image2qtree image2toast )

  if( VW_HAVE_PKG_LAPACK )
    set( PROGS ${PROGS} geoblend )
  endif( VW_HAVE_PKG_LAPACK )

  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwMosaic vwCartography )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_MOSAIC AND VW_HAVE_PKG_CARTOGRAPHY AND VW_HAVE_PKG_GDAL )



## Stereo tools
########################################
if( VW_HAVE_PKG_STEREO AND VW_HAVE_PKG_LAPACK )

  set( PROGS correlate )
  foreach( PROG ${PROGS} )
    add_executable( ${PROG} ${PROG}.cc )
    target_link_libraries( ${PROG} vwStereo vwInterestPoint ${LAPACK_LIBRARIES} )
    set( TOOLS ${TOOLS} ${PROG} )
  endforeach( PROG )
endif( VW_HAVE_PKG_STEREO AND VW_HAVE_PKG_LAPACK )


## install the lot
########################################
if( TOOLS )
  install_targets( /bin ${TOOLS} )
endif( TOOLS )

