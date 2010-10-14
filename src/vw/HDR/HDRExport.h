#ifndef __VW_HDR_EXPORT_H__
#define __VW_HDR_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_HDR_DYN_LINK)
#    ifdef VW_HDR_BUILD_DLL
#      define VW_HDR_DECL __declspec(dllexport)
#    else
#      define VW_HDR_DECL __declspec(dllimport)
#    endif //VW_HDR_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_HDR_DECL
#  define VW_HDR_DECL
#endif 

#endif //__VW_HDR_EXPORT_H__

