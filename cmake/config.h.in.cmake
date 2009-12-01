#ifndef __VW__CONFIG_H__
#define __VW__CONFIG_H__

#define VW_BOOST_VERSION ${VW_BOOST_VERSION}

//-- External Packages
//------------------------------------------------
#cmakedefine VW_HAVE_PKG_APRON 1
#cmakedefine VW_HAVE_PKG_BOOST 1
#cmakedefine VW_HAVE_PKG_BOOST_FILESYSTEM 1
#cmakedefine VW_HAVE_PKG_BOOST_PROGRAM_OPTIONS 1
#cmakedefine VW_HAVE_PKG_BOOST_THREAD 1
#cmakedefine VW_HAVE_PKG_BOOST_IOSTREAMS 1
#cmakedefine VW_HAVE_PKG_CG 1
#cmakedefine VW_HAVE_PKG_CLAPACK 1
#cmakedefine VW_HAVE_PKG_FLAPACK 1
#cmakedefine VW_HAVE_PKG_GDAL 1
#cmakedefine VW_HAVE_PKG_GL 1
#cmakedefine VW_HAVE_PKG_GLEW 1
#cmakedefine VW_HAVE_PKG_GMP 1
#cmakedefine VW_HAVE_PKG_HDF 1
#cmakedefine VW_HAVE_PKG_HDR 1
#cmakedefine VW_HAVE_PKG_ILMBASE 1
#cmakedefine VW_HAVE_PKG_JPEG 1
#cmakedefine VW_HAVE_PKG_LAPACK 1
#cmakedefine VW_HAVE_PKG_M 1
#cmakedefine VW_HAVE_PKG_MPFR 1
#cmakedefine VW_HAVE_PKG_OPENEXR 1
#cmakedefine VW_HAVE_PKG_PNG 1
#cmakedefine VW_HAVE_PKG_PPL 1
#cmakedefine VW_HAVE_PKG_PROJ4 1
#cmakedefine VW_HAVE_PKG_PTHREADS 1
#cmakedefine VW_HAVE_PKG_QHULL 1
#cmakedefine VW_HAVE_PKG_SLAPACK 1
#cmakedefine VW_HAVE_PKG_STANDALONE_BLAS 1
#cmakedefine VW_HAVE_PKG_STANDALONE_F2C 1
#cmakedefine VW_HAVE_PKG_STANDALONE_FBLAS 1
#cmakedefine VW_HAVE_PKG_STANDALONE_FLAPACK 1
#cmakedefine VW_HAVE_PKG_STANDALONE_LAPACK 1
#cmakedefine VW_HAVE_PKG_STANDALONE_LAPACK_AND_BLAS 1
#cmakedefine VW_HAVE_PKG_TIFF 1
#cmakedefine VW_HAVE_PKG_X11 1
#cmakedefine VW_HAVE_PKG_Z 1

//-- External headers. Are these defines used at all?
//------------------------------------------------
#cmakedefine VW_HAVE_DLFCN_H  1
#cmakedefine VW_HAVE_INTTYPES_H 1
#cmakedefine VW_HAVE_MEMORY_H 1
#cmakedefine VW_HAVE_STDINT_H 1
#cmakedefine VW_HAVE_STDLIB_H 1
#cmakedefine VW_HAVE_STRINGS_H 1
#cmakedefine VW_HAVE_STRING_H 1
#cmakedefine VW_HAVE_SYS_STAT_H 1
#cmakedefine VW_HAVE_SYS_TYPES_H 1
#cmakedefine VW_HAVE_UNISTD_H 1
#cmakedefine VW_STDC_HEADERS 1

//-- Internal Packages
//------------------------------------------------
#cmakedefine VW_HAVE_PKG_CAMERA 1
#cmakedefine VW_HAVE_PKG_CARTOGRAPHY 1
#cmakedefine VW_HAVE_PKG_CORE 1
#cmakedefine VW_HAVE_PKG_FILEIO 1
#cmakedefine VW_HAVE_PKG_GEOMETRY 1
#cmakedefine VW_HAVE_PKG_GPU 1
#cmakedefine VW_HAVE_PKG_IMAGE 1
#cmakedefine VW_HAVE_PKG_INTERESTPOINT 1
#cmakedefine VW_HAVE_PKG_MATH 1
#cmakedefine VW_HAVE_PKG_MOSAIC 1
#cmakedefine VW_HAVE_PKG_STEREO 1
#cmakedefine VW_HAVE_PKG_THREADS 1
#cmakedefine VW_HAVE_PKG_VW 1


//-- VW Configuration
//------------------------------------------------
#cmakedefine VW_ENABLE_EXCEPTIONS 1
#cmakedefine VW_ENABLE_BOUNDS_CHECK 1
#cmakedefine VW_ENABLE_CONFIG_FILE 1

#define VW_NUM_THREADS       ${VW_NUM_THREADS}

#define VW_PACKAGE           "${VW_PACKAGE}"
#define VW_PACKAGE_BUGREPORT "${VW_PACKAGE_BUGREPORT}"
#define VW_PACKAGE_NAME      "${VW_PACKAGE_NAME}"
#define VW_PACKAGE_STRING    "${VW_PACKAGE_STRING}"
#define VW_PACKAGE_TARNAME   "${VW_PACKAGE_TARNAME}"
#define VW_PACKAGE_VERSION   "${VW_PACKAGE_VERSION}"
#define VW_VERSION           "${VW_VERSION}"


#ifndef  WIN32
// Check to make sure the user is using the same version of Boost
// headers that the software was built with.
#  include <boost/version.hpp>
#  if BOOST_VERSION != VW_BOOST_VERSION
#    error You are using a different version of Boost than you used to build VW_!
#  endif
#else 
// include a helper header on win32
#  include "platform.h"
// XXX error preprocessor directive isn't understood by MSVC...
// XXX find a way to do the boost version check
#endif


//------------------------------------------------
#endif // __VW__CONFIG_H__
