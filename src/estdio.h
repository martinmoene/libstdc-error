/*
 * estdio.h
 *
 * Created on 2 July 2011, by Martin Moene
 *
 * Copyright 2011 Universiteit Leiden
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UL_STDCLIB_ESTDIO_H_INCLUDED
#define UL_STDCLIB_ESTDIO_H_INCLUDED

#include <stdio.h>      // FILE

/**
 * \file
 * \addtogroup ul_stdclib_error_stdio
 *
 * This module provides macro's and functions that add error handling to several
 * standard file functions, such as fopen(), fclose() and fprintf().
 *
 * The ruling assumption is that a program is exited when an error occurs.
 *
 * Example error message:
 * \verbatim estdio.exe: estdio.c(147): in function 'main()' failed to open file 'estdio-test-ro.tmp', mode 'w': Permission denied \endverbatim
 *
 * Basic usage is as follows.
 * \include ex-estdio.c
 *
 * Note: instead of including ecore.h and emalloc.h, you can also include the
 * header file error.h to declare all there is in the error module.
 *
 * @{
 */

#if defined ( __cplusplus )
extern "C" {
#endif

/**
 * fopen with error handling (includes error location); uses efopen_loc(), exits on error.
 */
#define EFOPEN( path, mode ) \
   efopen_loc( path, mode, __FILE__, __FUNCTION__, __LINE__ )

/**
 * fclose with error handling (includes error location); uses efclose_loc(), exits on error.
 */
#define EFCLOSE( fp, path ) \
   efclose_loc( fp, path, __FILE__, __FUNCTION__, __LINE__ )

/**
 * fprintf with error handling (includes error location); uses efprintf_loc(), exits on error;
 * for printing of string only, see EPRINTF0.
 */
#define EPRINTF( fmt, ... ) \
   efprintf_loc( stdout, "(stdout)", __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__ )

/**
 * fprintf with error handling to print a string only (includes error location); uses efprintf_loc(), exits on error; see also EFPRINTF0().
 */
#define EPRINTF0( fmt ) \
   efprintf_loc( stdout, "(stdout)", __FILE__, __FUNCTION__, __LINE__, fmt )

/**
 * fprintf with error handling (includes error location); uses efprintf_loc(), exits on error;
 * for printing of string only, see EFPRINTF0.
 */
#define EFPRINTF( fp, path, fmt, ... ) \
   efprintf_loc( fp, path, __FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__ )

/**
 * fprintf with error handling to print a string only (includes error location); uses efprintf_loc(), exits on error.
 *
 * In standard C, you are not allowed to leave the macro's variable argument out
 * entirely; but you are allowed to pass an empty argument. For example, this
 * invocation is invalid in ISO C, because there is no comma after the string:
 * \code
 * #define debug(format, ...) fprintf (stderr, format, __VA_ARGS__)
 * debug ("A message")
 * \endcode
 * See http://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
 */
#define EFPRINTF0( fp, path, fmt ) \
   efprintf_loc( fp, path, __FILE__, __FUNCTION__, __LINE__, fmt )

/**
 * fopen with error handling (no error location); exit on error.
 */
FILE * efopen( char const * const path, char const * const mode );

/**
 * fclose with error handling (no error location); exit on error.
 */
void efclose( FILE * fp, char const * const path );

/**
 * fprintf with error handling (no error location); exit on error.
 */
void efprintf( FILE * fp, char const * const path, char const * const fmt, ... );

/**
 * fopen with error handling (includes error location); exit on error.
 */
FILE * efopen_loc( char const * const path, char const * const mode, char const * const filename, char const * const function, int const line );

/**
 * fclose with error handling (includes error location); exit on error.
 */
void efclose_loc( FILE * fp, char const * const path, char const * const filename, char const * const function, int const line );

/**
 * fprintf with error handling (includes error location); exit on error.
 */
void efprintf_loc( FILE * fp, char const * const path, char const * const filename, char const * const function, int const line , char const * const fmt, ... );

/** @} */

#if defined ( __cplusplus )
} // extern "C" {
#endif

#endif // UL_STDCLIB_ESTDIO_H_INCLUDED

/*
 * end of file
 */

