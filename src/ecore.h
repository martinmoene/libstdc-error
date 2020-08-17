/*
 * ecore.h
 *
 * Provide error message functions for OS-related errors and non-OS-related errors.
 *
 * Created on 1 July 2011, by Martin Moene
 *
 * Copyright 2011 Universiteit Leiden
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UL_STDCLIB_CORE_H_INCLUDED
#define UL_STDCLIB_CORE_H_INCLUDED

/**
 * \mainpage Error handling for standard C library functions
 * \copydoc ul_stdclib_error_main
 */

/**
 * \addtogroup ul_stdclib_error Error handling for standard C library functions
 * @{
 * \addtogroup ul_stdclib_error_core ecore
 * \addtogroup ul_stdclib_error_malloc emalloc
 * \addtogroup ul_stdclib_error_stdio estdio
 * @}
 */

/**
 * \addtogroup ul_stdclib_error_main Error handling for standard C library functions
 *
 * This module provides macro's and functions that add error handling to
 * standard functions, such as malloc(), fopen(), fprintf().
 *
 * The ruling assumption is that a program is exited when an error occurs.
 *
 * The idea was gleaned from:
 * - Brian W. Kernighan and Rob Pike. The Practice of Programming. Addison�Wesley, 1999. ISBN 0-201-61586-X.
 *
 * Currently the module consists of the following submodules:
 * - \ref ul_stdclib_error_core - basic error functions
 * - \ref ul_stdclib_error_malloc - error handling for functions such as malloc(), realloc() and free()
 * - \ref ul_stdclib_error_stdio - error handling for functions such as fopen(), fclose() and fprintf()
 *
 * <br>
 * Created on 1 July 2011, by Martin Moene
 *
 * Copyright 2011 Universiteit Leiden
 *
 * This software is distributed under the Boost Software License, Version 1.0.
 * See http://www.boost.org/LICENSE_1_0.txt
 */

/**
 * \file
 * \addtogroup ul_stdclib_error_core
 *
 * This module provides macro's and functions to simplify the addition of
 * error handling to standard functions, such as malloc(), fopen(), fprintf().
 *
 * The ruling assumption is that a program is exited when an error occurs.
 *
 * The idea was gleaned from:
 * - Brian W. Kernighan and Rob Pike. The Practice of Programming. Addison�Wesley, 1999. ISBN 0�201�61586�X.
 *
 * ecore.h provides that basic tools to create error functions. See estdio.h and
 * emalloc.h for function-specific solutions.
 *
 * Basic usage is as follows.
 * \include ex-ecore.c
 *
 * @{
 */

#if defined ( __cplusplus )
extern "C" {
#endif

/**
 * the program path as set, or default "(...)" if not set.
 */
char const * const error_program_path( void );

/**
 * set program path (filename) for error message prefix;
 * return true on error.
 */
int set_error_program_path( char const * const path );

/**
 * format error message, return status;
 * for errors from functions that set errno, use os_error().
 *
 * Example message: "progname: filename(line): non-numeric line content 'content'".
 *
 * Usage: exit( error( EXIT_FAILURE, "%s(%d): non-numeric line content '%s'", filename, lineno, content ) );
 */
int error( int const status, char const * const fmt, ... );

/**
 * format error message, return status;
 * use for errors from functions that set errno; see also error().
 *
 * Example message: "progname: cannot open file 'filename': file does not exist".
 *
 * Usage: exit( error( EXIT_FAILURE, "cannot open file '%s'", path ) );
 */
int os_error( int const status, char const * const fmt, ... );

/** @} */

#if defined ( __cplusplus )
} // extern "C" {
#endif

#endif // UL_STDCLIB_CORE_H_INCLUDED

/*
 * end of file
 */
