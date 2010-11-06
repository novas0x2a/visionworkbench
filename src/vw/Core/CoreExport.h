#ifndef __VW_CORE_EXPORT_H__
#define __VW_CORE_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CORE_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_CORE_BUILD_DLL
#        define VW_CORE_DECL   __declspec(dllexport)
#        define VW_CORE_EXTERN
#      else
#        define VW_CORE_DECL   __declspec(dllimport)
#        define VW_CORE_EXTERN extern
#      endif //VW_CORE_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_CORE_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_CORE_DECL
#  define VW_CORE_DECL
#endif

#ifndef VW_CORE_EXTERN
#  define VW_CORE_EXTERN
#endif

#endif //__VW_CORE_EXPORT_H__

