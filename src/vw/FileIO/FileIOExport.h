#ifndef __VW_FILEIO_EXPORT_H__
#define __VW_FILEIO_EXPORT_H__

#include <vw/config.h>

#ifdef VW_HAS_DECLSPEC
#  if defined(VW_ALL_DYN_LINK) || defined(VW_FILEIO_DYN_LINK)
#    ifdef VW_FILEIO_BUILD_DLL
#      define VW_FILEIO_DECL __declspec(dllexport)
#    else
#      define VW_FILEIO_DECL __declspec(dllimport)
#    endif //VW_FILEIO_BUILD_DLL
#  endif // 
#endif // VW_HAS_DECLSPEC

#ifndef VW_FILEIO_DECL
#  define VW_FILEIO_DECL
#endif 

#endif //__VW_FILEIO_EXPORT_H__

