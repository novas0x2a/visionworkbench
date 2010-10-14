#ifndef __VW_CARTOGRAPHY_EXPORT_H__
#define __VW_CARTOGRAPHY_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CARTOGRAPHY_DYN_LINK)
#    ifdef VW_CARTOGRAPHY_BUILD_DLL
#      define VW_CARTOGRAPHY_DECL __declspec(dllexport)
#    else
#      define VW_CARTOGRAPHY_DECL __declspec(dllimport)
#    endif //VW_CARTOGRAPHY_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_CARTOGRAPHY_DECL
#  define VW_CARTOGRAPHY_DECL
#endif 

#endif //__VW_CARTOGRAPHY_EXPORT_H__

