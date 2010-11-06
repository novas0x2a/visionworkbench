#ifndef __VW_HDR_EXPORT_H__
#define __VW_HDR_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_HDR_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_HDR_BUILD_DLL
#        define VW_HDR_DECL   __declspec(dllexport)
#        define VW_HDR_EXTERN 
#      else
#        define VW_HDR_DECL   __declspec(dllimport)
#        define VW_HDR_EXTERN extern
#      endif //VW_HDR_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_HDR_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_HDR_DECL
#  define VW_HDR_DECL
#endif

#ifndef VW_HDR_EXTERN
#  define VW_HDR_EXTERN
#endif

#endif //__VW_HDR_EXPORT_H__

