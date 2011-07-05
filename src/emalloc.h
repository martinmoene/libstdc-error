/*
 * emalloc.h
 *
 * Provide error checked version of function from malloc.h.
 *
 * Created on 3 July 2011, by Martin Moene
 *
 * Copyright 2011 Universiteit Leiden
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UL_STDCLIB_EMALLOC_H_INCLUDED
#define UL_STDCLIB_EMALLOC_H_INCLUDED

#include <stddef.h>      // size_t

/**
 * \file
 * \addtogroup ul_stdclib_error_malloc
 *
 * This module provides macro's and functions that add error handling to the
 * standard memory allocation functions, such as malloc(), realloc() and free().
 *
 * The ruling assumption is that a program is exited when an error occurs.
 *
 * Example error message:
 * \verbatim emalloc.exe: emalloc.c(86): in function 'main()' failed to reallocate 512 bytes for variable 'p': Not enough space\endverbatim
 *
 * Basic usage is as follows.
 * \include ex-emalloc.c
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
 * malloc with error handling (includes error location); uses emalloc_loc(), exits on error.
 */
#define EMALLOC( size, var ) \
   emalloc_loc( size, #var, __FILE__, __FUNCTION__, __LINE__ )

/**
 * erealloc with error handling (includes error location); uses erealloc_loc(), exits on error.
 */
#define EREALLOC( var, newsize ) \
   erealloc_loc( var, newsize, #var, __FILE__, __FUNCTION__, __LINE__ )

/**
 * free with error handling (includes error location); uses efree_loc(), exits on error.
 */
#define EFREE( var ) \
   efree_loc( var, #var, __FILE__, __FUNCTION__, __LINE__ )

/**
 * malloc with error handling (no error location); exit on error.
 */
void * emalloc( size_t bytes, char const * const var );

/**
 * realloc with error handling (no error location); exit on error.
 */
void * erealloc( void * p, size_t bytes, char const * const var );

/**
 * free with error handling (no error location); exit on error.
 */
void efree( void * p, char const * const var );

/**
 * malloc with error handling (includes error location); exit on error.
 */
void * emalloc_loc( size_t bytes, char const * const var, char const * const filename, char const * const function, int const line );

/**
 * realloc with error handling (includes error location); exit on error.
 */
void * erealloc_loc( void * p, size_t bytes, char const * const var, char const * const filename, char const * const function, int const line );

/**
 * free with error handling (includes error location); exit on error.
 */
void efree_loc( void * p, char const * const var, char const * const filename, char const * const function, int const line );

/** @} */

#if defined ( __cplusplus )
} // extern "C" {
#endif

#endif // UL_STDCLIB_EMALLOC_H_INCLUDED

/*
 * end of file
 */

