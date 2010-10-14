#ifndef __VW_MOSAIC_EXPORT_H__
#define __VW_MOSAIC_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_MOSAIC_DYN_LINK)
#    ifdef VW_MOSAIC_BUILD_DLL
#      define VW_MOSAIC_DECL __declspec(dllexport)
#    else
#      define VW_MOSAIC_DECL __declspec(dllimport)
#    endif //VW_MOSAIC_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_MOSAIC_DECL
#  define VW_MOSAIC_DECL
#endif 

#endif //__VW_MOSAIC_EXPORT_H__

