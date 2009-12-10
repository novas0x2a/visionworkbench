#ifndef __VW_PLATE_EXPORT_H__
#define __VW_PLATE_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_PLATE_DYN_LINK)
#    ifdef VW_PLATE_BUILD_DLL
#      define VW_PLATE_DECL __declspec(dllexport)
#    else
#      define VW_PLATE_DECL __declspec(dllimport)
#    endif //VW_PLATE_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_PLATE_DECL
#  define VW_PLATE_DECL
#endif 

#endif //__VW_PLATE_EXPORT_H__

