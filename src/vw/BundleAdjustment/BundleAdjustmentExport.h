#ifndef __VW_BUNDLEADJUSTMENT_EXPORT_H__
#define __VW_BUNDLEADJUSTMENT_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_BUNDLEADJUSTMENT_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_BUNDLEADJUSTMENT_BUILD_DLL
#        define VW_BUNDLEADJUSTMENT_DECL   __declspec(dllexport)
#        define VW_BUNDLEADJUSTMENT_EXTERN
#      else
#        define VW_BUNDLEADJUSTMENT_DECL   __declspec(dllimport)
#        define VW_BUNDLEADJUSTMENT_EXTERN extern
#      endif //VW_BUNDLEADJUSTMENT_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_BUNDLEADJUSTMENT_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_BUNDLEADJUSTMENT_DECL
#  define VW_BUNDLEADJUSTMENT_DECL
#endif

#ifndef VW_BUNDLEADJUSTMENT_EXTERN
#  define VW_BUNDLEADJUSTMENT_EXTERN
#endif

#endif //__VW_BUNDLEADJUSTMENT_EXPORT_H__

