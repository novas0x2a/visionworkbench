#ifndef __VW_IMAGE_EXPORT_H__
#define __VW_IMAGE_EXPORT_H__

#include <vw/config.h>

#ifdef _WIN32
// for exporting explicit instantiations in msvc,
// see: http://support.microsoft.com/kb/168958
#  pragma warning(disable:4231)
#endif // _WIN32

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_IMAGE_DYN_LINK)
#    if defined(_WIN32) || defined (__CYGWIN__)
#      ifdef VW_IMAGE_BUILD_DLL
#        define VW_IMAGE_DECL   __declspec(dllexport)
#        define VW_IMAGE_EXTERN
#      else
#        define VW_IMAGE_DECL   __declspec(dllimport)
#        define VW_IMAGE_EXTERN extern
#      endif //VW_IMAGE_BUILD_DLL
#    elif defined(__GNUC__) && (__GNUC__ >= 4)
#      define VW_IMAGE_DECL __attribute__ ((visibility("default")))
#    endif
#  endif //
#endif // VW_HAS_DECLSPEC

#ifndef VW_IMAGE_DECL
#  define VW_IMAGE_DECL
#endif 

#ifndef VW_IMAGE_EXTERN
#  define VW_IMAGE_EXTERN
#endif 


#endif //__VW_IMAGE_EXPORT_H__

