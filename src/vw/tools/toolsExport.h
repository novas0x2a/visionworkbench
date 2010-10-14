#ifndef __VW_TOOLS_EXPORT_H__
#define __VW_TOOLS_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_TOOLS_DYN_LINK)
#    ifdef VW_TOOLS_BUILD_DLL
#      define VW_TOOLS_DECL __declspec(dllexport)
#    else
#      define VW_TOOLS_DECL __declspec(dllimport)
#    endif //VW_TOOLS_BUILD_DLL
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_TOOLS_DECL
#  define VW_TOOLS_DECL
#endif

#endif //__VW_TOOLS_EXPORT_H__

