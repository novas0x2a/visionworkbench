// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


template struct vw::math::ArgUnaryFunctorTypeHelper<FuncT, ArgT, true>;
template class  vw::math::BBox<RealT, 0>;
template struct vw::math::BinaryFunctorTypeHelper<FuncT, Arg1T, Arg2T, false, true>;
template struct vw::math::BinaryFunctorTypeHelper<FuncT, Arg1T, Arg2T, true, false>;
template struct vw::math::BinaryFunctorTypeHelper<FuncT, Arg1T, Arg2T, true, true>;
template class  vw::math::Matrix<ElemT, 0, 0>;
template class  vw::math::MatrixProxy<ElemT, 0, 0>;
template class  vw::math::Vector<ElemT, 0>;
template struct vw::math::VectorAssignImpl<vw::math::Vector<DstElemT, 2>, SrcVecT>;
template struct vw::math::VectorAssignImpl<vw::math::Vector<DstElemT, 3>, SrcVecT>;
template struct vw::math::VectorAssignImpl<vw::math::Vector<DstElemT, 4>, SrcVecT>;
template class  vw::math::VectorProxy<ElemT, 0>;
