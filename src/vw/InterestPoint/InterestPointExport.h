#ifndef __VW_INTERESTPOINT_EXPORT_H__
#define __VW_INTERESTPOINT_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_INTERESTPOINT_DYN_LINK)
#    ifdef VW_INTERESTPOINT_BUILD_DLL
#      define VW_INTERESTPOINT_DECL __declspec(dllexport)
#    else
#      define VW_INTERESTPOINT_DECL __declspec(dllimport)
#    endif //VW_INTERESTPOINT_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_INTERESTPOINT_DECL
#  define VW_INTERESTPOINT_DECL
#endif 

#endif //__VW_INTERESTPOINT_EXPORT_H__

