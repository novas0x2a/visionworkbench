#ifndef __VW_CORE_EXPORT_H__
#define __VW_CORE_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CORE_DYN_LINK)
#    ifdef VW_CORE_BUILD_DLL
#      define VW_CORE_DECL __declspec(dllexport)
#    else
#      define VW_CORE_DECL __declspec(dllimport)
#    endif //VW_CORE_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_CORE_DECL
#  define VW_CORE_DECL
#endif 

#endif //__VW_CORE_EXPORT_H__

