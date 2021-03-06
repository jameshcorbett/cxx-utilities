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
#include "benchmarkMatrixMatrixKernels.hpp"

namespace LvArray
{
namespace benchmarking
{

#define MATRIX_MATRIX_KERNEL( N, L, M, a_ik, b_kj, c_ij ) \
  for( INDEX_TYPE i = 0; i < N; ++i ) \
  { \
    for( INDEX_TYPE j = 0; j < M; ++j ) \
    { \
      VALUE_TYPE dot = 0; \
      for( INDEX_TYPE k = 0; k < L; ++k ) \
      { \
        dot = dot + a_ik * b_kj; \
      } \
      c_ij += dot; \
    } \
  } \
  return

#define MATRIX_MATRIX_KERNEL_RAJA( N, L, M, a_ik, b_kj, c_ij ) \
  forall< POLICY >( N, [=] LVARRAY_HOST_DEVICE ( INDEX_TYPE const i ) \
  { \
    for( INDEX_TYPE j = 0; j < M; ++j ) \
    { \
      VALUE_TYPE dot = 0; \
      for( INDEX_TYPE k = 0; k < L; ++k ) \
      { \
        dot = dot + a_ik * b_kj; \
      } \
      c_ij += dot; \
    } \
  } ); \
  return

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
fortranArrayKernel( Array< VALUE_TYPE, PERMUTATION > const & a,
                    Array< VALUE_TYPE, PERMUTATION > const & b,
                    Array< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a( i, k ), b( k, j ), c( i, j ) ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
fortranViewKernel( ArrayView< VALUE_TYPE const, PERMUTATION > const & a,
                   ArrayView< VALUE_TYPE const, PERMUTATION > const & b,
                   ArrayView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a( i, k ), b( k, j ), c( i, j ) ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
fortranSliceKernel( ArraySlice< VALUE_TYPE const, PERMUTATION > const a,
                    ArraySlice< VALUE_TYPE const, PERMUTATION > const b,
                    ArraySlice< VALUE_TYPE, PERMUTATION > const c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a( i, k ), b( k, j ), c( i, j ) ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
subscriptArrayKernel( Array< VALUE_TYPE, PERMUTATION > const & a,
                      Array< VALUE_TYPE, PERMUTATION > const & b,
                      Array< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a[ i ][ k ], b[ k ][ j ], c[ i ][ j ] ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
subscriptViewKernel( ArrayView< VALUE_TYPE const, PERMUTATION > const & a,
                     ArrayView< VALUE_TYPE const, PERMUTATION > const & b,
                     ArrayView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a[ i ][ k ], b[ k ][ j ], c[ i ][ j ] ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
subscriptSliceKernel( ArraySlice< VALUE_TYPE const, PERMUTATION > const a,
                      ArraySlice< VALUE_TYPE const, PERMUTATION > const b,
                      ArraySlice< VALUE_TYPE, PERMUTATION > const c )
{ MATRIX_MATRIX_KERNEL( a.size( 0 ), a.size( 1 ), b.size( 1 ), a[ i ][ k ], b[ k ][ j ], c[ i ][ j ] ); }

template< typename PERMUTATION >
void MatrixMatrixNative< PERMUTATION >::
RAJAViewKernel( RajaView< VALUE_TYPE const, PERMUTATION > const & a,
                RajaView< VALUE_TYPE const, PERMUTATION > const & b,
                RajaView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL( a.layout.sizes[ 0 ], a.layout.sizes[ 1 ], b.layout.sizes[ 1 ], a( i, k ), b( k, j ), c( i, j ) ); }

template<>
void MatrixMatrixNative< RAJA::PERM_IJ >::
pointerKernel( INDEX_TYPE const N,
               INDEX_TYPE const L,
               INDEX_TYPE const M,
               VALUE_TYPE const * const LVARRAY_RESTRICT a,
               VALUE_TYPE const * const LVARRAY_RESTRICT b,
               VALUE_TYPE * const LVARRAY_RESTRICT c )
{
  MATRIX_MATRIX_KERNEL( N, L, M,
                        a[ ACCESS_IJ( N, L, i, k ) ],
                        b[ ACCESS_IJ( L, M, k, j ) ],
                        c[ ACCESS_IJ( N, M, i, j ) ] );
}

template<>
void MatrixMatrixNative< RAJA::PERM_JI >::
pointerKernel( INDEX_TYPE const N,
               INDEX_TYPE const L,
               INDEX_TYPE const M,
               VALUE_TYPE const * const LVARRAY_RESTRICT a,
               VALUE_TYPE const * const LVARRAY_RESTRICT b,
               VALUE_TYPE * const LVARRAY_RESTRICT c )
{
  MATRIX_MATRIX_KERNEL( N, L, M,
                        a[ ACCESS_JI( N, L, i, k ) ],
                        b[ ACCESS_JI( L, M, k, j ) ],
                        c[ ACCESS_JI( N, M, i, j ) ] );
}

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
fortranViewKernel( ArrayView< VALUE_TYPE const, PERMUTATION > const & a,
                   ArrayView< VALUE_TYPE const, PERMUTATION > const & b,
                   ArrayView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL_RAJA( a.size( 0 ), a.size( 1 ), b.size( 1 ), a( i, k ), b( k, j ), c( i, j ) ); }

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
fortranSliceKernel( ArraySlice< VALUE_TYPE const, PERMUTATION > const a,
                    ArraySlice< VALUE_TYPE const, PERMUTATION > const b,
                    ArraySlice< VALUE_TYPE, PERMUTATION > const c )
{ MATRIX_MATRIX_KERNEL_RAJA( a.size( 0 ), a.size( 1 ), b.size( 1 ), a( i, k ), b( k, j ), c( i, j ) ); }

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
subscriptViewKernel( ArrayView< VALUE_TYPE const, PERMUTATION > const & a,
                     ArrayView< VALUE_TYPE const, PERMUTATION > const & b,
                     ArrayView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL_RAJA( a.size( 0 ), a.size( 1 ), b.size( 1 ), a[ i ][ k ], b[ k ][ j ], c[ i ][ j ] ); }

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
subscriptSliceKernel( ArraySlice< VALUE_TYPE const, PERMUTATION > const a,
                      ArraySlice< VALUE_TYPE const, PERMUTATION > const b,
                      ArraySlice< VALUE_TYPE, PERMUTATION > const c )
{ MATRIX_MATRIX_KERNEL_RAJA( a.size( 0 ), a.size( 1 ), b.size( 1 ), a[ i ][ k ], b[ k ][ j ], c[ i ][ j ] ); }

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
RAJAViewKernel( RajaView< VALUE_TYPE const, PERMUTATION > const & a,
                RajaView< VALUE_TYPE const, PERMUTATION > const & b,
                RajaView< VALUE_TYPE, PERMUTATION > const & c )
{ MATRIX_MATRIX_KERNEL_RAJA( a.layout.sizes[ 0 ], a.layout.sizes[ 1 ], b.layout.sizes[ 1 ], a( i, k ), b( k, j ), c( i, j ) ); }

template< typename POLICY >
void RAJAPointerKernelHelper( RAJA::PERM_IJ,
                              INDEX_TYPE const N,
                              INDEX_TYPE const L,
                              INDEX_TYPE const M,
                              VALUE_TYPE const * const LVARRAY_RESTRICT a,
                              VALUE_TYPE const * const LVARRAY_RESTRICT b,
                              VALUE_TYPE * const LVARRAY_RESTRICT c )
{
  MATRIX_MATRIX_KERNEL_RAJA( N, L, M,
                             a[ ACCESS_IJ( N, L, i, k ) ],
                             b[ ACCESS_IJ( L, M, k, j ) ],
                             c[ ACCESS_IJ( N, M, i, j ) ] );
}

template< typename POLICY >
void RAJAPointerKernelHelper( RAJA::PERM_JI,
                              INDEX_TYPE const N,
                              INDEX_TYPE const L,
                              INDEX_TYPE const M,
                              VALUE_TYPE const * const LVARRAY_RESTRICT a,
                              VALUE_TYPE const * const LVARRAY_RESTRICT b,
                              VALUE_TYPE * const LVARRAY_RESTRICT c )
{
  MATRIX_MATRIX_KERNEL_RAJA( N, L, M,
                             a[ ACCESS_JI( N, L, i, k ) ],
                             b[ ACCESS_JI( L, M, k, j ) ],
                             c[ ACCESS_JI( N, M, i, j ) ] );
}

template< typename PERMUTATION, typename POLICY >
void MatrixMatrixRAJA< PERMUTATION, POLICY >::
pointerKernel( INDEX_TYPE const N,
               INDEX_TYPE const L,
               INDEX_TYPE const M,
               VALUE_TYPE const * const LVARRAY_RESTRICT a,
               VALUE_TYPE const * const LVARRAY_RESTRICT b,
               VALUE_TYPE * const LVARRAY_RESTRICT c )
{ return RAJAPointerKernelHelper< POLICY >( PERMUTATION {}, N, L, M, a, b, c ); }


template class MatrixMatrixNative< RAJA::PERM_IJ >;
template class MatrixMatrixNative< RAJA::PERM_JI >;

template class MatrixMatrixRAJA< RAJA::PERM_IJ, serialPolicy >;
template class MatrixMatrixRAJA< RAJA::PERM_JI, serialPolicy >;

#if defined(USE_OPENMP)

template class MatrixMatrixRAJA< RAJA::PERM_IJ, parallelHostPolicy >;
template class MatrixMatrixRAJA< RAJA::PERM_JI, parallelHostPolicy >;

#endif

#if defined(USE_CUDA) && defined(USE_CHAI)

template class MatrixMatrixRAJA< RAJA::PERM_IJ, RAJA::cuda_exec< THREADS_PER_BLOCK > >;
template class MatrixMatrixRAJA< RAJA::PERM_JI, RAJA::cuda_exec< THREADS_PER_BLOCK > >;

#endif

} // namespace benchmarking
} // namespace LvArray
