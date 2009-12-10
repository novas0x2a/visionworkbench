#ifndef __VW_CAMERA_EXPORT_H__
#define __VW_CAMERA_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_CAMERA_DYN_LINK)
#    ifdef VW_CAMERA_BUILD_DLL
#      define VW_CAMERA_DECL __declspec(dllexport)
#    else
#      define VW_CAMERA_DECL __declspec(dllimport)
#    endif //VW_CAMERA_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_CAMERA_DECL
#  define VW_CAMERA_DECL
#endif 

#endif //__VW_CAMERA_EXPORT_H__

