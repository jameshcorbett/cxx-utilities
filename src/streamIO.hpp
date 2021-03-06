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

/**
 * @file streamIO.hpp
 */

#ifndef ARRAYUTILITIES_HPP_
#define ARRAYUTILITIES_HPP_

// SRC includes
#include "Array.hpp"
#include "SortedArray.hpp"
#include "ArrayOfArrays.hpp"

// System includes
#include <string>
#include <vector>
#include <iostream>

namespace LvArray
{

/**
 * @struct stringToArrayHelper
 * @brief A helper struct to recursively read an istringstream into an array.
 */
template< typename T, typename INDEX_TYPE >
struct stringToArrayHelper
{

  /**
   * @brief function to skip ',' delimiters in a istringstream
   * @param inputStream The istringstream to operate on.
   */
  static void skipDelimters( std::istringstream & inputStream )
  {
    while( inputStream.peek() == ' ' )
    {
      inputStream.ignore();
    }

  }

  /**
   * @brief Reads a value of the array from the stream
   * @param arrayValue A reference to the array value to read in
   * @param inputStream the stream to read a value from
   */
  template< int NDIM >
  static void Read( T & arrayValue,
                    INDEX_TYPE const *,
                    std::istringstream & inputStream )
  {
    inputStream >> arrayValue;

    LVARRAY_ERROR_IF( inputStream.fail(),
                      "Invalid value of type " << typeid(T).name() << " in: " <<
                      ( inputStream.eof()  ?  "" : inputStream.str().substr( inputStream.tellg() ) ) );
  }

  /**
   * @brief Recursively read values from an istringstream into an array.
   * @param arraySlice The arraySlice that provides the interface to write data into the array.
   * @param dims The dimensions of the array.
   * @param inputStream The stream to read from.
   */
  template< int NDIM, int USD >
  static void
  Read( ArraySlice< T, NDIM, USD, INDEX_TYPE > const & arraySlice,
        INDEX_TYPE const * const dims,
        std::istringstream & inputStream )
  {
    LVARRAY_ERROR_IF( inputStream.peek() != '{', "opening { not found for input array: "<<inputStream.str() );
    inputStream.ignore();

    for( int i=0; i<(*dims); ++i )
    {
      skipDelimters( inputStream );
      Read< NDIM-1 >( arraySlice[i], dims+1, inputStream );
    }

    skipDelimters( inputStream );
    LVARRAY_ERROR_IF( inputStream.peek() != '}', "closing } not found for input array: "<<inputStream.str() );
    inputStream.ignore();
  }
};



/**
 * @brief This function reads the contents of a string into an Array.
 * @param array Reference to the array that will receive the contents of the input.
 * @param valueString The string that contains the data to read into @p array.
 *
 * @details The contents of @p valueString are parsed and placed into @p array. @p array is resized to
 *   allow space for receiving the contents of @p valueString. The required notation for array values in
 *   @p valueString are similar to the requirements of initialization of a standard c-array:
 * @code
 *     Array<T,1> --> "{ val[0], val[1], val[2], ... }"
 *     Array<T,2> --> "{ { val[0][0], val[0][1], val[0][2], ... },
 *                       { val[1][0], val[1][1], val[1][2], ... }, ... } "
 * @endcode
 *
 * @note * A null initializer is allowed via "{}". All values must be delimited with a ','.
 *   All spaces are stripped prior to processing, please don't use tabs for anything.
 */
template< typename T,
          int NDIM,
          typename PERMUTATION,
          typename INDEX_TYPE,
          template< typename > class BUFFER_TYPE >
static void stringToArray( Array< T, NDIM, PERMUTATION, INDEX_TYPE, BUFFER_TYPE > & array,
                           std::string valueString )
{
  // Check to make sure there are no space delimited values. The assumption is anything that is not
  // a '{' or '}' or ',' or ' ' is part of a value. Loope over the string and keep track of whether
  // or not there is a value on the left of the char. If there is a value to the left, with a space
  // on the left, and we run into another value, then this means that there is a space delimited
  // entry.
  {
    bool valueOnLeft = false;
    bool spaceOnLeft = false;

    for( char const & c : valueString )
    {
      if( c != '{' && c != ',' && c != '}' && c!=' ' )
      {
        if( valueOnLeft && spaceOnLeft )
        {
          LVARRAY_ERROR( "Array value sequence specified without ',' delimiter: "<<valueString );
        }
      }

      // If a open/close brace or ',' delimiter, then there is neither a value or space
      // to the left of subsequent characters.
      if( c == '{' || c == ',' || c == '}' )
      {
        valueOnLeft = false;
        spaceOnLeft = false;
      }
      else // if the first check fails, then either c is a space or value
      {
        // if it is a value, then set the valueOnLeft flag to true for subsequent c
        if( c != ' ' )
        {
          valueOnLeft = true;
          spaceOnLeft = false;
        }
        else // if it is a space, then set spaceOnLeft to true for subsequent c
        {
          spaceOnLeft = true;
        }
      }
    }
  }

  // erase all spaces from input string to simplify parsing
  valueString.erase( std::remove( valueString.begin(), valueString.end(), ' ' ), valueString.end());

  // allow for a null input
  if( valueString=="{}" )
  {
    array.clear();
    return;
  }

  // checking for various formatting errors
  LVARRAY_ERROR_IF( valueString.find( "}{" ) != std::string::npos,
                    "Sub arrays not separated by ',' delimiter: "<<valueString );

  LVARRAY_ERROR_IF( valueString[0]!='{',
                    "First non-space character of input string for an array must be {. Given string is: \n"<<valueString );

  size_t const numOpen = std::count( valueString.begin(), valueString.end(), '{' );
  size_t const numClose = std::count( valueString.begin(), valueString.end(), '}' );

  LVARRAY_ERROR_IF( numOpen != numClose,
                    "Number of opening { not equal to number of } in processing of string for filling"
                    " an Array. Given string is: \n"<<valueString );


  // after allowing for the null input, disallow a sub-array null input
  LVARRAY_ERROR_IF( valueString.find( "{}" )!=std::string::npos,
                    "Cannot have an empty sub-dimension of an array, i.e. { { 0, 1}, {} }. "
                    "The input is"<<valueString );

  // get the number of dimensions from the number of { characters that begin the input string
  int const ndims = LvArray::integerConversion< int >( valueString.find_first_not_of( '{' ));
  LVARRAY_ERROR_IF( ndims!=NDIM,
                    "number of dimensions in string ("<<ndims<<
                    ") does not match dimensions of array("<<NDIM<<
                    "). String is:/n"<<valueString );


  // now get the number of dimensions, and the size of each dimension.

  // use dimLevel to track the current dimension we are parsing
  int dimLevel = -1;

  // dims is the dimensions that get set the first diving down.
  INDEX_TYPE dims[NDIM] = {0};

  // currentDims is used to track the dimensions for subsequent dives down the dimensions.
  INDEX_TYPE currentDims[NDIM] = {0};

  // flag to see if the dims value has been set for a given dimension
  bool dimSet[NDIM] = {false};

  for( int i=0; i<NDIM; ++i )
  {
    dims[i]=1;
    currentDims[i] = 1;
  }

  char lastChar = 0;
  for( size_t charCount = 0; charCount<valueString.size(); ++charCount )
  {
    char const c = valueString[charCount];
    // this had better be true for the first char...we had a check for this. This is why we can
    // set dimLevel = -1 to start.
    if( c=='{' )
    {
      ++dimLevel;
    }
    else if( c=='}' )
    {
      // } means that we are closing a dimension. That means we know the size of this dimLevel
      dimSet[dimLevel] = true;
      LVARRAY_ERROR_IF( dims[dimLevel]!=currentDims[dimLevel],
                        "Dimension "<<dimLevel<<" is inconsistent across the expression. "
                                                "The first set value of the dimension is "<<dims[dimLevel]<<
                        " while the current value of the dimension is"<<currentDims[dimLevel]<<
                        ". The values that have been parsed prior to the error are:\n"<<
                        valueString.substr( 0, charCount+1 ) );

      // reset currentDims and drop dimLevel for post-closure parsing
      currentDims[dimLevel] = 1;
      --dimLevel;
      LVARRAY_ERROR_IF( dimLevel<0 && charCount<(valueString.size()-1),
                        "In parsing the input string, the current dimension of the array has dropped "
                        "below 0. This means that there are more '}' than '{' at some point in the"
                        " parsing. The values that have been parsed prior to the error are:\n"<<
                        valueString.substr( 0, charCount+1 ) );

    }
    else if( c==',' ) // we are counting the dimension sizes because there is a delimiter.
    {
      LVARRAY_ERROR_IF( lastChar=='{' || lastChar==',',
                        "character of ',' follows '"<<lastChar<<"'. Comma must follow an array value." );
      if( dimSet[dimLevel]==false )
      {
        ++(dims[dimLevel]);
      }
      ++(currentDims[dimLevel]);
    }
    lastChar = c;
  }
  LVARRAY_ERROR_IF( dimLevel!=-1,
                    "Expression fails to close all '{' with a corresponding '}'. Check your input:"<<
                    valueString );

  array.resize( NDIM, dims );


  // we need to replace our ',' with ' ' for reading in an array of strings, otherwise the
  // stringstream::operator>> will grab the ','
  std::replace( valueString.begin(), valueString.end(), ',', ' ' );

  // we also need to add a ' ' in front of any '}' otherwise the
  // stringstream::operator>> will grab the }
  for( std::string::size_type a=0; a<valueString.size(); ++a )
  {
    if( valueString[a] == '}' )
    {
      valueString.insert( a, " " );
      ++a;
    }
  }
  std::istringstream strstream( valueString );
  // this recursively reads the values from the stringstream
  stringToArrayHelper< T, INDEX_TYPE >::Read( array.toSlice(), array.dims(), strstream );
}

/**
 * @tparam T The type of the values in @p slice.
 * @tparam NDIM The number of dimensions of @p slice.
 * @tparam USD The unit stride dimension of @p slice.
 * @tparam INDEX_TYPE The integer used by @p slice.
 * @brief This function outputs the contents of an array slice to an output stream.
 * @param stream The output stream to write to.
 * @param slice The slice to output.
 * @return @p stream .
 */
template< typename T, int NDIM, int USD, typename INDEX_TYPE >
std::ostream & operator<<( std::ostream & stream,
                           ArraySlice< T, NDIM, USD, INDEX_TYPE > const & slice )
{
  stream << "{ ";

  if( slice.size( 0 ) > 0 )
    stream << slice[ 0 ];

  for( INDEX_TYPE i = 1; i < slice.size( 0 ); ++i )
  {
    stream << ", " << slice[ i ];
  }

  stream << " }";
  return stream;
}

/**
 * @tparam T The type of the values in @p view.
 * @tparam NDIM The number of dimensions of @p view.
 * @tparam USD The unit stride dimension of @p view.
 * @tparam INDEX_TYPE The integer used by @p view.
 * @tparam BUFFER_TYPE The buffer type used by @p view.
 * @brief This function outputs the contents of an ArrayView to an output stream.
 * @param stream The output stream to write to.
 * @param view The view to output.
 * @return @p stream .
 */
template< typename T,
          int NDIM,
          int USD,
          typename INDEX_TYPE,
          template< typename > class BUFFER_TYPE >
std::ostream & operator<<( std::ostream & stream,
                           ArrayView< T, NDIM, USD, INDEX_TYPE, BUFFER_TYPE > const & view )
{ return stream << view.toSliceConst(); }

/**
 * @tparam T The type of the values in @p view.
 * @tparam INDEX_TYPE The integer used by @p view.
 * @brief This function outputs the contents of @p view to an output stream.
 * @param stream The output stream to write to.
 * @param view The SortedArrayView to output.
 * @return @p stream .
 */
template< typename T, typename INDEX_TYPE, template< typename > class BUFFER_TYPE >
std::ostream & operator<< ( std::ostream & stream,
                            SortedArrayView< T const, INDEX_TYPE, BUFFER_TYPE > const & view )
{
  if( view.size() == 0 )
  {
    stream << "{}";
    return stream;
  }

  stream << "{ " << view[ 0 ];

  if( view.size() > 0 )
    stream << view[ 0 ];

  for( INDEX_TYPE i = 1; i < view.size(); ++i )
  {
    stream << ", " << view[ i ];
  }

  stream << " }";
  return stream;
}

/**
 * @tparam T The type of the values in @p array.
 * @tparam INDEX_TYPE The integer used by @p array.
 * @brief This function outputs the contents of @p array to an output stream.
 * @param stream The output stream to write to.
 * @param array The SortedArray to output.
 * @return @p stream .
 */
template< typename T, typename INDEX_TYPE, template< typename > class BUFFER_TYPE >
std::ostream & operator<< ( std::ostream & stream,
                            SortedArray< T, INDEX_TYPE, BUFFER_TYPE > const & array )
{ return stream << array.toViewConst(); }

/**
 * @tparam T The type of the values in @p view.
 * @tparam INDEX_TYPE The integer used by @p view.
 * @brief This function outputs the contents of @p view to an output stream.
 * @param stream The output stream to write to.
 * @param view The ArrayOfArraysView to output.
 * @return @p stream .
 */
template< typename T, typename INDEX_TYPE, template< typename > class BUFFER_TYPE >
std::ostream & operator<< ( std::ostream & stream,
                            ArrayOfArraysView< T const, INDEX_TYPE const, true, BUFFER_TYPE > const & view )
{
  stream << "{" << std::endl;

  for( INDEX_TYPE i = 0; i < view.size(); ++i )
  {
    stream << i << "\t{";
    for( INDEX_TYPE j = 0; j < view.sizeOfArray( i ); ++j )
    {
      stream << view( i, j ) << ", ";
    }

    // for (INDEX_TYPE j = view.sizeOfArray(i); j < view.capacityOfArray(i); ++j)
    // {
    //   stream << "X" << ", ";
    // }

    stream << "}" << std::endl;
  }

  stream << "}" << std::endl;
  return stream;
}

/**
 * @tparam T The type of the values in @p array.
 * @tparam INDEX_TYPE The integer used by @p array.
 * @brief This function outputs the contents of @p array to an output stream.
 * @param stream The output stream to write to.
 * @param array The ArrayOfArrays to output.
 * @return @p stream .
 */
template< typename T, typename INDEX_TYPE, template< typename > class BUFFER_TYPE >
std::ostream & operator<< ( std::ostream & stream,
                            ArrayOfArrays< T, INDEX_TYPE, BUFFER_TYPE > const & array )
{ return stream << array.toViewConst(); }

/**
 * @brief Output a c-array to a stream.
 * @tparam T The type contained in the array.
 * @tparam N The size of the array.
 * @param stream The output stream to write to.
 * @param array The c-array to output.
 * @return @p stream.
 */
template< typename T, int N >
std::enable_if_t< !std::is_same< T, char >::value, std::ostream & >
operator<< ( std::ostream & stream, T const ( &array )[ N ] )
{
  stream << "{ " << array[ 0 ];
  for( int i = 1; i < N; ++i )
  {
    stream << ", " << array[ i ];
  }
  stream << " }";
  return stream;
}

/**
 * @brief Output a 2D c-array to a stream.
 * @tparam T The type contained in the array.
 * @tparam M The size of the first dimension.
 * @tparam N The size of the second dimension.
 * @param stream The output stream to write to.
 * @param array The 2D c-array to output.
 * @return @p stream.
 */
template< typename T, int M, int N >
std::ostream & operator<< ( std::ostream & stream, T const ( &array )[ M ][ N ] )
{
  stream << "{ " << array[ 0 ];
  for( int i = 1; i < M; ++i )
  {
    stream << ", " << array[ i ];
  }
  stream << " }";
  return stream;
}

} // namespace LvArray

#endif /* STRINGUTILITIES_HPP_ */
