#ifndef  __VW__PLATFORM_H__
#define  __VW__PLATFORM_H__

/*
 * Some quickie fixes to make life a little simpler under win32...
 */
#ifdef WIN32
  #define _USE_MATH_DEFINES
  #include <windows.h>
  #include <math.h>

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
  
  inline void srandom( unsigned int seed ) {
    srand(seed);
  }

  #define snprintf     _snprintf
  #define vsnprintf    _vsnprintf
  #define strcasecmp   _stricmp
  #define strncasecmp  _strnicmp
  
  inline double hypot (double a, double b) { return _hypot(a,b); }
  inline float  hypotf(float a, float b)   { return _hypotf(a,b); }
  // XXX FIXME XXX 
  // XXX there does not appear to be a _hypotl, 
  // XXX so use _hypot as a poor workaround
  inline long double hypotl(double a, double b) {
    return _hypot(a,b);
  }

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

#endif//WIN32

#endif//__VW__PLATFORM_H__ 
