#ifndef __VW_INTERESTPOINT_EXPORT_H__
#define __VW_INTERESTPOINT_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_INTERESTPOINT_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_INTERESTPOINT_BUILD_DLL
#        define VW_INTERESTPOINT_DECL   __declspec(dllexport)
#        define VW_INTERESTPOINT_EXTERN 
#      else
#        define VW_INTERESTPOINT_DECL   __declspec(dllimport)
#        define VW_INTERESTPOINT_EXTERN extern
#      endif //VW_INTERESTPOINT_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_INTERESTPOINT_DECL __attribute__ ((visibility("default")))
#    endif
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_INTERESTPOINT_DECL
#  define VW_INTERESTPOINT_DECL
#endif 

#ifndef VW_INTERESTPOINT_EXTERN
#  define VW_INTERESTPOINT_EXTERN
#endif 

#endif //__VW_INTERESTPOINT_EXPORT_H__

