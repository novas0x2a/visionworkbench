// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


/// \file Exception.h
///
/// Exceptions used in the platefile system.
///
#ifndef __VW_PLATE_EXCEPTION_H__
#define __VW_PLATE_EXCEPTION_H__

#include <vw/Plate/PlateExport.h>
#include <vw/Core/Exception.h>

namespace vw {
namespace platefile {

  /// PlateFileErr exception
  ///
  /// This is the base class for all platefile exceptions.
  ///
  VW_DEFINE_EXCEPTION(PlatefileErr, Exception, VW_PLATE_DECL);

  /// TileNotFound exception
  ///
  /// This exception is thrown by the Tree and Index classes whenever
  /// a tile is requested that does not exist.  It is frequently
  /// caught by higher level classes like PlateFile when they are
  /// trying to determine whether a tile exists or not.
  ///
  VW_DEFINE_EXCEPTION(TileNotFoundErr, PlatefileErr, VW_PLATE_DECL);

  /// InvalidPlatefileErr exception
  ///
  /// This exception is thrown by the IndexService when there is an
  /// attempt to access or operate on a platefile that has not been
  /// opened, and is not being tracked by the system.
  ///
  VW_DEFINE_EXCEPTION(InvalidPlatefileErr, PlatefileErr, VW_PLATE_DECL);

  /// BlobLimitErr exception 
  ///
  /// This exception is thrown whenever the maximum number of blobs is
  /// reached.  This shouldn't ever really happen in practice unless
  /// something is wrong.
  ///
  VW_DEFINE_EXCEPTION(BlobLimitErr, PlatefileErr, VW_PLATE_DECL);

  /// BlobIoErr exception 
  ///
  /// This exception is thrown whenever the maximum number of blobs is
  /// reached.  This shouldn't ever really happen in practice unless
  /// something is wrong.
  ///
  VW_DEFINE_EXCEPTION(BlobIoErr, PlatefileErr, VW_PLATE_DECL);

  /// InvalidPlatefileErr exception 
  ///
  /// This exception is thrown by the IndexService when an error
  /// occurs while attempting to create a new platefile.
  ///
  VW_DEFINE_EXCEPTION(PlatefileCreationErr, PlatefileErr, VW_PLATE_DECL);

  /// InvalidPlatefileErr exception
  ///
  /// This exception is thrown when an unexpect RPC error occurs.
  ///
  VW_DEFINE_EXCEPTION(RpcErr, PlatefileErr, VW_PLATE_DECL);

}} // namespace vw::platefile

#endif // __VW_PLATE_TREE_H__
