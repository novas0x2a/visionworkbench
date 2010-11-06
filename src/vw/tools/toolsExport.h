#ifndef __VW_TOOLS_EXPORT_H__
#define __VW_TOOLS_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_TOOLS_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_TOOLS_BUILD_DLL
#        define VW_TOOLS_DECL   __declspec(dllexport)
#        define VW_TOOLS_EXTERN 
#      else
#        define VW_TOOLS_DECL   __declspec(dllimport)
#        define VW_TOOLS_EXTERN extern
#      endif //VW_TOOLS_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_TOOLS_DECL __attribute__ ((visibility("default")))
#    endif
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_TOOLS_DECL
#  define VW_TOOLS_DECL
#endif 

#ifndef VW_TOOLS_EXTERN
#  define VW_TOOLS_EXTERN
#endif 


#endif //__VW_TOOLS_EXPORT_H__

