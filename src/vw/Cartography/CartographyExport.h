#ifndef __VW_CARTOGRAPHY_EXPORT_H__
#define __VW_CARTOGRAPHY_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CARTOGRAPHY_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_CARTOGRAPHY_BUILD_DLL
#        define VW_CARTOGRAPHY_DECL   __declspec(dllexport)
#        define VW_CARTOGRAPHY_EXTERN
#      else
#        define VW_CARTOGRAPHY_DECL   __declspec(dllimport)
#        define VW_CARTOGRAPHY_EXTERN extern
#      endif //VW_CARTOGRAPHY_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_CARTOGRAPHY_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_CARTOGRAPHY_DECL
#  define VW_CARTOGRAPHY_DECL
#endif

#ifndef VW_CARTOGRAPHY_EXTERN
#  define VW_CARTOGRAPHY_EXTERN
#endif 

#endif //__VW_CARTOGRAPHY_EXPORT_H__

