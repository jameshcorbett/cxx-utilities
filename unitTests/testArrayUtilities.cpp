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

// Source includes
#include "streamIO.hpp"
#include "MallocBuffer.hpp"

// TPL includes
#include <gtest/gtest.h>

// System includes
#include <string>

const char IGNORE_OUTPUT[] = ".*";

namespace LvArray
{

template< typename T, typename PERMUTATION >
using ArrayT = Array< T, getDimension( PERMUTATION {} ), PERMUTATION, std::ptrdiff_t, MallocBuffer >;

TEST( testArrayUtilities, stringToArrayErrors )
{
  std::string input;

  // This should work
  {
    input = " { { {0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_IJK > array;
    stringToArray( array, input );
  }

  {
    input = " { 10 1 } ";
    ArrayT< int, RAJA::PERM_I > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }

  {
    input = " { { 1, 2 }{ 3, 4 } } ";
    ArrayT< int, RAJA::PERM_IJ > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }

  // This should fail the num('{')==num('}') test
  {
    input = " { { {0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } ";
    ArrayT< int, RAJA::PERM_IJK > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = " { { {0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17}  , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_IKJ > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = " { { {0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14,{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_JIK > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = " { { {0,1,2},{3,4,5} }, { {6,7,8,{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_JKI > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = " { { 0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_KIJ > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = "  { {0,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } ";
    ArrayT< int, RAJA::PERM_KJI > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }

  {
    input = " { { {,1,2},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_IJK > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }

  {
    input = " { { {},{3,4,5} }, { {6,7,8},{9,10,11} }, { {12,13,14},{15,16,17} } , { {18,19,20},{21,22,23} } }";
    ArrayT< int, RAJA::PERM_IJK > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }
  {
    input = " { { {0,1,2}}{ } }";
    ArrayT< int, RAJA::PERM_IJK > array;
    EXPECT_DEATH_IF_SUPPORTED( stringToArray( array, input ), IGNORE_OUTPUT );
  }


}

TEST( testArrayUtilities, stringToArray3d )
{
  std::string input;
  input += "{ ";
  int numI = 4;
  int numJ = 5;
  int numK = 3;
  for( int i=0; i<4; ++i )
  {
    input += "{ ";
    for( int j=0; j<5; ++j )
    {
      input += "{ ";
      for( int k=0; k<3; ++k )
      {
        input += std::to_string( i*2+j*3+k*4 );
        if( k<(numK-1) )
        {
          input += " , ";
        }
      }
      input += " }";
      if( j<(numJ-1) )
      {
        input += " , ";
      }
    }
    input += " }";
    if( i<(numI-1) )
    {
      input += " , ";
    }
  }
  input += " }";

  ArrayT< int, RAJA::PERM_JIK > array;
  stringToArray( array, input );

  ASSERT_EQ( array.size( 0 ), numI );
  ASSERT_EQ( array.size( 1 ), numJ );
  ASSERT_EQ( array.size( 2 ), numK );

  for( int i=0; i<array.size( 0 ); ++i )
  {
    for( int j=0; j<array.size( 1 ); ++j )
    {
      for( int k=0; k<array.size( 2 ); ++k )
      {
        ASSERT_EQ( array[i][j][k], i*2+j*3+k*4 );
      }
    }
  }
}



TEST( testArrayUtilities, arrayToString )
{
  ArrayT< int, RAJA::PERM_IKJ > array( 2, 4, 3 );

  for( int i=0; i<2; ++i )
  {
    for( int j=0; j<4; ++j )
    {
      for( int k=0; k<3; ++k )
      {
        array[i][j][k] = i*100 + j*10 + k;
      }
    }
  }

  std::stringstream ss;
  ss << array;
  ASSERT_EQ( ss.str(), "{ { { 0, 1, 2 }, { 10, 11, 12 }, { 20, 21, 22 }, { 30, 31, 32 } },"
                       " { { 100, 101, 102 }, { 110, 111, 112 }, { 120, 121, 122 }, { 130, 131, 132 } } }" );

}

} // namespace LvArray
