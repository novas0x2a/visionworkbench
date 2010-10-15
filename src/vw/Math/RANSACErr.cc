// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__

#include <vw/Math/RANSAC.h>

namespace vw {
namespace math {

/* in order for the RANSACErr exception to be exported 
 * from the vwMath library, we need it to appear in object 
 * code somewhere. Inclusion of this file is only necessary
 * when using declspec import/export
 */
void embody_ransac_exception() {
  VW_ASSERT( false, RANSACErr() << "embody_ransac_exception");
}

}}