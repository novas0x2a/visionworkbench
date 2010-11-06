#ifndef __VW_PLATE_EXPORT_H__
#define __VW_PLATE_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_PLATE_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_PLATE_BUILD_DLL
#        define VW_PLATE_DECL   __declspec(dllexport)
#        define VW_PLATE_EXTERN 
#      else
#        define VW_PLATE_DECL   __declspec(dllimport)
#        define VW_PLATE_EXTERN extern
#      endif //VW_PLATE_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_PLATE_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_PLATE_DECL
#  define VW_PLATE_DECL
#endif

#ifndef VW_PLATE_EXTERN
#  define VW_PLATE_EXTERN
#endif

#endif //__VW_PLATE_EXPORT_H__

