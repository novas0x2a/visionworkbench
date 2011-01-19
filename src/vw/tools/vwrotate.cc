// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <iostream>
#include <vw/Image.h>
#include <vw/FileIO.h>

using namespace vw;
typedef PixelRGB<uint8> Px;

int main( int argc, char *argv[] ) {
  try {
    VW_ASSERT( argc==3, ArgumentErr() << "Invalid command-line args." );
    ImageView<Px> image;

    read_image( image, argv[1] );
    write_image( argv[2],
        crop(
          rotate(
            translate(image, -image.cols()/2, -image.rows()/2),
            M_PI),
          -image.cols(), -image.rows(), 2*image.cols(), 2*image.rows())
        );
  }
  catch( Exception& e ) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Usage: vwconvert <source> <destination>" << std::endl;
    return 1;
  }
  return 0;
}
