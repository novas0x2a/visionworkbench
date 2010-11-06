#ifndef __VW_GEOMETRY_EXPORT_H__
#define __VW_GEOMETRY_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_GEOMETRY_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_GEOMETRY_BUILD_DLL
#        define VW_GEOMETRY_DECL   __declspec(dllexport)
#        define VW_GEOMETRY_EXTERN 
#      else
#        define VW_GEOMETRY_DECL   __declspec(dllimport)
#        define VW_GEOMETRY_EXTERN extern
#      endif //VW_GEOMETRY_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_GEOMETRY_DECL __attribute__ ((visibility("default")))
#    endif
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_GEOMETRY_DECL
#  define VW_GEOMETRY_DECL
#endif 

#ifndef VW_GEOMETRY_EXTERN
#  define VW_GEOMETRY_EXTERN
#endif 

#endif //__VW_GEOMETRY_EXPORT_H__

