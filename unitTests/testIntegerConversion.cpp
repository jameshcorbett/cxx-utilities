/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "gtest/gtest.h"
#include "IntegerConversion.hpp"

typedef  int32_t int32;
typedef uint32_t uint32;
typedef  int64_t int64;
typedef uint64_t uint64;


TEST( IntegerConversion, unsignedToSigned )
{
  uint32 source0 = std::numeric_limits< int32 >::max();
  uint32 source1 = std::numeric_limits< uint32 >::max();
  uint64 source2 = std::numeric_limits< int64 >::max();
  uint64 source3 = std::numeric_limits< uint64 >::max();

  int32 compare0 = std::numeric_limits< int32 >::max();
  int64 compare2 = std::numeric_limits< int64 >::max();

  ASSERT_TRUE( compare0 == LvArray::integerConversion< int32 >( source0 ) );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< int32 >( source1 ), "" );

  ASSERT_TRUE( compare2 == LvArray::integerConversion< int64 >( source2 ) );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< int64 >( source3 ), "" );
}


TEST( IntegerConversion, signedToUnsigned )
{
  int32 source0 = -1;
  int64 source1 = -1;
  int64 source3 = std::numeric_limits< int64 >::max();

  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< uint32 >( source0 ), "" );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< uint64 >( source1 ), "" );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< uint32 >( source3 ), "" );
}


TEST( IntegerConversion, sameSign )
{
  int64 source0 = std::numeric_limits< int32 >::lowest();
  int64 source1 = std::numeric_limits< int64 >::lowest();

  ASSERT_EQ( std::numeric_limits< int32 >::lowest(), LvArray::integerConversion< int32 >( source0 ) );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< int32 >( source1 ), "" );


  int64 source2 = std::numeric_limits< int32 >::max();
  int64 source3 = std::numeric_limits< int64 >::max();

  ASSERT_EQ( std::numeric_limits< int32 >::max(), LvArray::integerConversion< int32 >( source2 ) );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< int32 >( source3 ), "" );


  uint64 source4 = std::numeric_limits< uint32 >::max();
  uint64 source5 = std::numeric_limits< uint64 >::max();

  ASSERT_EQ( std::numeric_limits< uint32 >::max(), LvArray::integerConversion< uint32 >( source4 ) );
  ASSERT_DEATH_IF_SUPPORTED( LvArray::integerConversion< uint32 >( source5 ), "" );
}
