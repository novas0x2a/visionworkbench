#ifndef __VW_MATH_EXPORT_H__
#define __VW_MATH_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_MATH_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_MATH_BUILD_DLL
#        define VW_MATH_DECL   __declspec(dllexport)
#        define VW_MATH_EXTERN
#      else
#        define VW_MATH_DECL   __declspec(dllimport)
#        define VW_MATH_EXTERN extern
#      endif //VW_MATH_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_MATH_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_MATH_DECL
#  define VW_MATH_DECL
#endif

#ifndef VW_MATH_EXTERN
#  define VW_MATH_EXTERN
#endif

#endif //__VW_MATH_EXPORT_H__

