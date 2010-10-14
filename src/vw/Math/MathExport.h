#ifndef __VW_MATH_EXPORT_H__
#define __VW_MATH_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_MATH_DYN_LINK)
#    ifdef VW_MATH_BUILD_DLL
#      define VW_MATH_DECL __declspec(dllexport)
#    else
#      define VW_MATH_DECL __declspec(dllimport)
#    endif //VW_MATH_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_MATH_DECL
#  define VW_MATH_DECL
#endif 

#endif //__VW_MATH_EXPORT_H__

