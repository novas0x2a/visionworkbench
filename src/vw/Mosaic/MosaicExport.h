#ifndef __VW_MOSAIC_EXPORT_H__
#define __VW_MOSAIC_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_MOSAIC_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_MOSAIC_BUILD_DLL
#        define VW_MOSAIC_DECL   __declspec(dllexport)
#        define VW_MOSAIC_EXTERN
#      else
#        define VW_MOSAIC_DECL   __declspec(dllimport)
#        define VW_MOSAIC_EXTERN extern
#      endif //VW_MOSAIC_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_MOSAIC_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_MOSAIC_DECL
#  define VW_MOSAIC_DECL
#endif

#ifndef VW_MOSAIC_EXTERN
#  define VW_MOSAIC_EXTERN
#endif

#endif //__VW_MOSAIC_EXPORT_H__

