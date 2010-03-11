// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


/// \file Descriptor.cc
///
/// Basic classes and structures for storing image interest points.
///
#include <vw/InterestPoint/Descriptor.h>

const uint vw::ip::SGradDescriptorGenerator::box_strt[5] = {17,15,9,6,0};
const uint vw::ip::SGradDescriptorGenerator::box_size[5] = {2,4,8,10,14};
const uint vw::ip::SGradDescriptorGenerator::box_half[5] = {1,2,4,5,7};
