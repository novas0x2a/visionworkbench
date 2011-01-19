// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <gtest/gtest.h>
#include <test/Helpers.h>

#include <vw/Image/ImageView.h>
#include <vw/Image/Transform.h>
#include <vw/Image/Algorithms.h>

using namespace vw;

template <template<class> class TraitT, class T>
static bool bool_trait( T const& /*arg*/ ) {
  return TraitT<T>::value;
}

TEST( Transform, BBoxComp ) { // wikka wikka
  ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
  TransformView<InterpolationView<EdgeExtensionView<ImageView<double>, ZeroEdgeExtension>, BilinearInterpolation>, TranslateTransform> im2 = transform(im, TranslateTransform(1,1));
}

TEST( Transform, Translate ) {
  ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
  ImageView<double> im3 = transform(im, TranslateTransform(1,1));
  TransformView<InterpolationView<EdgeExtensionView<ImageView<double>, ZeroEdgeExtension>, BilinearInterpolation>, TranslateTransform> im2 = transform(im, TranslateTransform(1,1));
  ASSERT_EQ( im2.cols(), 2 );
  ASSERT_EQ( im2.rows(), 3 );
  EXPECT_EQ( im2(1,1), 1 );
  EXPECT_EQ( im2(0,0), 0 );
  EXPECT_EQ( im2(1,2), 3 );
  EXPECT_EQ( im2(-1,-1), 0 );
  EXPECT_EQ( im2(-100,-100), 0 );

  // Test accessor
  EXPECT_EQ( *(im2.origin().advance(1,1)), 1 );
  EXPECT_EQ( *(im2.origin().advance(-1,-1)), 0 );

  // Test the traits
  ASSERT_TRUE( bool_trait<IsFloatingPointIndexable>( im2 ) );
  ASSERT_TRUE( bool_trait<IsFloatingPointIndexable>( transform(im, TranslateTransform(1,1)) ) );
  ASSERT_FALSE( bool_trait<IsMultiplyAccessible>( transform(im, TranslateTransform(1,1)) ) );
}

TEST( Transform, TranslateFunc ) {
  ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
  ImageView<double> im2;
  // FIXME This test exhibits a Heisenbug on gc.cs.cmu.edu when used
  // with the default BilinearInterpolation (Red Hat gcc 4.0.2-8).
  // Attempts at replicating the bug in other contexts fail, so we
  // just work around it here for the moment.
  im2 = translate(im,1.0,1.0,ZeroEdgeExtension(),NearestPixelInterpolation());
  ASSERT_EQ( im2.cols(), 2 );
  ASSERT_EQ( im2.rows(), 3 );
  EXPECT_EQ( im2(1,1), 1 );
  EXPECT_EQ( im2(0,0), 0 );
  EXPECT_EQ( im2(1,2), 3 );
  im2 = translate(im,1,1);
  ASSERT_EQ( im2.cols(), 2 );
  ASSERT_EQ( im2.rows(), 3 );
  EXPECT_EQ( im2(1,1), 1 );
  EXPECT_EQ( im2(0,0), 0 );
  EXPECT_EQ( im2(1,2), 3 );
}

TEST( Transform, Resample ) {
  ImageView<double> im(2,3); im(0,0)=1; im(1,0)=2; im(0,1)=3; im(1,1)=4; im(0,2)=5; im(1,2)=6;
  TransformView<InterpolationView<EdgeExtensionView<ImageView<double>, ConstantEdgeExtension>, BilinearInterpolation>, ResampleTransform> im2 = resample(im, 2, 2);
  ASSERT_EQ( im2.cols(), 4 );
  ASSERT_EQ( im2.rows(), 6 );
  EXPECT_EQ( im2(0,0), 1 );
  EXPECT_EQ( im2(2,2), 4 );
  EXPECT_EQ( im2(1,1), 2.5 );
  EXPECT_EQ( im2(-1,-1), 1 );
  EXPECT_EQ( im2(-100,-100), 1 );
  EXPECT_EQ( im2(100,100), 6 );

  // Test accessor
  EXPECT_EQ( *(im2.origin().advance(0,0)), 1 );
  EXPECT_EQ( *(im2.origin().advance(-1,-1)), 1 );

  // Test the traits
  ASSERT_TRUE( bool_trait<IsFloatingPointIndexable>( im2 ) );
  ASSERT_TRUE( bool_trait<IsFloatingPointIndexable>( transform(im, ResampleTransform(1,1)) ) );
  ASSERT_FALSE( bool_trait<IsMultiplyAccessible>( transform(im, ResampleTransform(1,1)) ) );
}

TEST( Transform, Rotate ) {
  typedef PixelRGB<uint8> Px;
  Px gray(0x7f,0x7f,0x7f), r(0xff,0,0), g(0,0xff,0), b(0,0,0xff);

  const int32 WIDH = 128, HEIT = 64;
  ImageView<Px> src1(WIDH,HEIT), src2(WIDH,HEIT);
  fill(crop(src1,      0,      0, WIDH/2, HEIT/2), gray);
  fill(crop(src1, WIDH/2,      0, WIDH/2, HEIT/2), r);
  fill(crop(src1,      0, HEIT/2, WIDH/2, HEIT/2), g);
  fill(crop(src1, WIDH/2, HEIT/2, WIDH/2, HEIT/2), b);

  fill(crop(src1,      0,      0, WIDH/2, HEIT/2), b);
  fill(crop(src1, WIDH/2,      0, WIDH/2, HEIT/2), g);
  fill(crop(src1,      0, HEIT/2, WIDH/2, HEIT/2), r);
  fill(crop(src1, WIDH/2, HEIT/2, WIDH/2, HEIT/2), gray);

  // Rotate is vulnerable to terrible trig precision
  ImageView<Px> dst1 = crop(rotate(src1, 2*M_PI), 0, 0, WIDH, HEIT),
                dst2 = crop(rotate(src2, M_PI), -WIDH/2, -HEIT/2, WIDH, HEIT);

  EXPECT_MATRIX_EQ(src1, dst1);
  EXPECT_MATRIX_EQ(src2, dst2);

  ImageView<Px> dst3 =
    vw::crop(
        vw::rotate(
          vw::translate(src1, -src1.cols()/2, -src1.rows()/2),
          M_PI),
        -src1.cols()/2, -src1.rows()/2, src1.cols(), src1.rows());

  EXPECT_MATRIX_EQ(src2, dst3);
}
