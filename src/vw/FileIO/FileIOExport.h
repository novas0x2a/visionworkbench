#ifndef __VW_FILEIO_EXPORT_H__
#define __VW_FILEIO_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_FILEIO_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_FILEIO_BUILD_DLL
#        define VW_FILEIO_DECL   __declspec(dllexport)
#        define VW_FILEIO_EXTERN
#      else
#        define VW_FILEIO_DECL   __declspec(dllimport)
#        define VW_FILEIO_EXTERN extern
#      endif //VW_FILEIO_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_FILEIO_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_FILEIO_DECL
#  define VW_FILEIO_DECL
#endif

#ifndef VW_FILEIO_EXTERN
#  define VW_FILEIO_EXTERN
#endif

#endif //__VW_FILEIO_EXPORT_H__

