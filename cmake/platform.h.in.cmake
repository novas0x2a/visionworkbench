#ifndef  __VW__PLATFORM_H__
#define  __VW__PLATFORM_H__

/*
 * Some quickie fixes to make life a little simpler under win32...
 */
#ifdef _WIN32
  #ifndef WIN32_WORKAROUNDS_INCLUDED
  #define WIN32_WORKAROUNDS_INCLUDED

  // Disable retarded win32 'min' and 'max' macros
  #ifndef NOMINMAX
  #  define NOMINMAX
  #endif
  // don't include gobs of win32 headers
  #ifndef WIN32_LEAN_AND_MEAN
  #  define WIN32_LEAN_AND_MEAN
  #endif 
  // prefer winsock2 
  #ifndef _WINSOCKAPI_
  #  define _WINSOCKAPI_
  #endif 
  // include useful things like M_PI
  #ifndef _USE_MATH_DEFINES
  #  define _USE_MATH_DEFINES
  #endif 
  
  #include <windows.h>
  #include <math.h>
  #include <stdlib.h>
  
  namespace win32_workarounds {
  
    inline long lround(double val) {
      return (long)(val > 0 ? floor(val+0.5) : ceil(val-0.5));
    }
    inline float nearbyintf(float val) {
      return (val > 0 ? floorf(val+0.5f) : ceilf(val-0.5f));
    }
    inline float roundf(float val) {
  	  return nearbyintf(val);
    }
    inline double nearbyint(double val) {
      return (val > 0 ? floor(val+0.5) : ceil(val-0.5));
    }
    inline double round(double val) {
      return nearbyint(val);
    }
    inline double log2(double x) {
      return log(x)/log(2.0); 
    }

    inline int random() {
      return rand();
    }
    inline void srandom( unsigned int seed ) {
      srand(seed);
    }

    //inline double hypot (double a, double b) { return _hypot(a,b); }
    inline float  hypotf(float a, float b)   { return _hypotf(a,b); }
    // XXX FIXME XXX 
    // XXX there does not appear to be a _hypotl, 
    // XXX so use _hypot as a poor workaround
    inline long double hypotl(double a, double b) {
      // is there a VC++ equivalent to #warning?
      return _hypot(a,b);
    }
  } // namespace win32_workarounds
  using namespace win32_workarounds;

  /*
   * use macros for calls with varargs
   */
  #if (_MSC_VER < 1500)  
  #ifndef snprintf
  #  define snprintf     _snprintf
  #endif
  #ifndef vsnprintf
  #  define vsnprintf    _vsnprintf
  #endif
  #ifndef strcasecmp
  #  define strcasecmp   _stricmp
  #endif
  #ifndef strncasecmp
  #  define strncasecmp  _strnicmp
  #endif
  #endif // _MSC_VER
  
  /*
   * we shouln't actually need these if 
   * _USE_MATH_DEFINES is defined before 
   * pulling in math.h
   */
  #ifndef M_PI
  #  define M_PI 3.14159265358979323846
  #endif//M_PI
  #ifndef M_PI_2
  #  define M_PI_2 1.570796326794896558
  #endif//M_PI_2
  
  #include <boost/cstdint.hpp>
  using namespace boost;

  #endif // WIN32_WORKAROUNDS_INCLUDED
#endif// _WIN32

#endif//__VW__PLATFORM_H__ 
