#ifndef __VW_GEOMETRY_EXPORT_H__
#define __VW_GEOMETRY_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_GEOMETRY_DYN_LINK)
#    ifdef VW_GEOMETRY_BUILD_DLL
#      define VW_GEOMETRY_DECL __declspec(dllexport)
#    else
#      define VW_GEOMETRY_DECL __declspec(dllimport)
#    endif //VW_GEOMETRY_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_GEOMETRY_DECL
#  define VW_GEOMETRY_DECL
#endif 

#endif //__VW_GEOMETRY_EXPORT_H__

