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

#include "Array.hpp"

#include <gtest/gtest.h>
#include <RAJA/RAJA.hpp>

namespace LvArray
{
namespace testing
{

TEST( Permutations, isValid )
{
  // 1D
  static_assert( isValidPermutation( camp::idx_seq< 0 > {} ), "This is a valid permutation." );

  static_assert( !isValidPermutation( camp::idx_seq< 1 > {} ), "This is not a valid permutation." );
  static_assert( !isValidPermutation( camp::idx_seq< -1 > {} ), "This is not a valid permutation." );

  // 2D
  static_assert( isValidPermutation( camp::idx_seq< 0, 1 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 1, 0 > {} ), "This is a valid permutation." );

  static_assert( !isValidPermutation( camp::idx_seq< 1, 1 > {} ), "This is not a valid permutation." );
  static_assert( !isValidPermutation( camp::idx_seq< 0, 2 > {} ), "This is not a valid permutation." );
  static_assert( !isValidPermutation( camp::idx_seq< -1, 0 > {} ), "This is not a valid permutation." );

  // 3D
  static_assert( isValidPermutation( camp::idx_seq< 0, 1, 2 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 0, 2, 1 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 1, 0, 2 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 1, 2, 0 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 2, 0, 1 > {} ), "This is a valid permutation." );
  static_assert( isValidPermutation( camp::idx_seq< 2, 1, 0 > {} ), "This is a valid permutation." );

  static_assert( !isValidPermutation( camp::idx_seq< 0, 1, 5 > {} ), "This is not a valid permutation." );
  static_assert( !isValidPermutation( camp::idx_seq< 0, 1, 0 > {} ), "This is not a valid permutation." );
  static_assert( !isValidPermutation( camp::idx_seq< -6, 1, 0 > {} ), "This is not a valid permutation." );
}

TEST( Permutations, findStrideOneDimension )
{
  // 1D
  static_assert( getStrideOneDimension( RAJA::PERM_I {} ) == 0, "Incorrect stride one dimension." );

  // 2D
  static_assert( getStrideOneDimension( RAJA::PERM_IJ {} ) == 1, "Incorrect stride one dimension." );

  // 3D
  static_assert( getStrideOneDimension( RAJA::PERM_IJK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JIK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_IKJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KIJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JKI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KJI {} ) == 0, "Incorrect stride one dimension." );

  // 4D
  static_assert( getStrideOneDimension( RAJA::PERM_IJKL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JIKL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_IKJL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KIJL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JKIL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KJIL {} ) == 3, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_IJLK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JILK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_ILJK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LIJK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JLIK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LJIK {} ) == 2, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_IKLJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KILJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_ILKJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LIKJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KLIJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LKIJ {} ) == 1, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JKLI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KJLI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_JLKI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LJKI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_KLJI {} ) == 0, "Incorrect stride one dimension." );
  static_assert( getStrideOneDimension( RAJA::PERM_LKJI {} ) == 0, "Incorrect stride one dimension." );
}


TEST( Permutation, AsArray )
{
  {
    constexpr CArray< camp::idx_t, 1 > carray = asArray( RAJA::PERM_I {} );
    constexpr std::array< camp::idx_t, 1 > stdarray = RAJA::as_array< RAJA::PERM_I >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 2 > carray = asArray( RAJA::PERM_IJ {} );
    constexpr std::array< camp::idx_t, 2 > stdarray = RAJA::as_array< RAJA::PERM_IJ >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 2 > carray = asArray( RAJA::PERM_JI {} );
    constexpr std::array< camp::idx_t, 2 > stdarray = RAJA::as_array< RAJA::PERM_JI >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_IJK {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_IJK >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_IKJ {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_IKJ >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_JIK {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_JIK >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_JKI {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_JKI >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_KIJ {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_KIJ >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }

  {
    constexpr CArray< camp::idx_t, 3 > carray = asArray( RAJA::PERM_KJI {} );
    constexpr std::array< camp::idx_t, 3 > stdarray = RAJA::as_array< RAJA::PERM_KJI >::get();
    static_assert( carray[ 0 ] == stdarray[ 0 ], "Should be true." );
    static_assert( carray[ 1 ] == stdarray[ 1 ], "Should be true." );
    static_assert( carray[ 2 ] == stdarray[ 2 ], "Should be true." );
  }
}

} // namespace testing
} // namespace LvArray
