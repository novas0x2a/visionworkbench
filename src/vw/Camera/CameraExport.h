#ifndef __VW_CAMERA_EXPORT_H__
#define __VW_CAMERA_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CAMERA_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_CAMERA_BUILD_DLL
#        define VW_CAMERA_DECL   __declspec(dllexport)
#        define VW_CAMERA_EXTERN
#      else
#        define VW_CAMERA_DECL   __declspec(dllimport)
#        define VW_CAMERA_EXTERN extern
#      endif //VW_CAMERA_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_CAMERA_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_CAMERA_DECL
#  define VW_CAMERA_DECL
#endif

#ifndef VW_CAMERA_EXTERN
#  define VW_CAMERA_EXTERN
#endif

#endif //__VW_CAMERA_EXPORT_H__

