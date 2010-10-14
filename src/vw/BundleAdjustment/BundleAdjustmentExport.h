#ifndef __VW_BUNDLEADJUSTMENT_EXPORT_H__
#define __VW_BUNDLEADJUSTMENT_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_BUNDLEADJUSTMENT_DYN_LINK)
#    ifdef VW_BUNDLEADJUSTMENT_BUILD_DLL
#      define VW_BUNDLEADJUSTMENT_DECL __declspec(dllexport)
#    else
#      define VW_BUNDLEADJUSTMENT_DECL __declspec(dllimport)
#    endif //VW_BUNDLEADJUSTMENT_BUILD_DLL
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_BUNDLEADJUSTMENT_DECL
#  define VW_BUNDLEADJUSTMENT_DECL
#endif

#endif //__VW_BUNDLEADJUSTMENT_EXPORT_H__

