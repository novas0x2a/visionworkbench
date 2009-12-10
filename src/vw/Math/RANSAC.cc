#include <vw/Math/RANSAC.h>
namespace vw {
namespace math {
  void exception_symbol() {
    vw_throw(RANSACErr() << "bogus" );
  }
}}
