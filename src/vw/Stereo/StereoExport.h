#ifndef __VW_STEREO_EXPORT_H__
#define __VW_STEREO_EXPORT_H__

#include <vw/config.h>

#ifdef _WIN32
// for exporting explicit instantiations in msvc,
// see: http://support.microsoft.com/kb/168958
#  pragma warning(disable:4231)
#endif // _WIN32

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_STEREO_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_STEREO_BUILD_DLL
#        define VW_STEREO_DECL   __declspec(dllexport)
#        define VW_STEREO_EXTERN
#      else
#        define VW_STEREO_DECL   __declspec(dllimport)
#        define VW_STEREO_EXTERN extern
#      endif //VW_STEREO_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_STEREO_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_STEREO_DECL
#  define VW_STEREO_DECL
#endif

#ifndef VW_STEREO_EXTERN
#  define VW_STEREO_EXTERN
#endif

#endif //__VW_STEREO_EXPORT_H__

