
#define _CRT_SECURE_NO_WARNINGS 1
/*
 * ecore.c
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

// header
#include "ecore.h"      // header first to check if it's self-contained.

// system headers
#include <errno.h>      // errno
#include <stdio.h>      // vfprintf()
#include <stdarg.h>     // va_arg()
#include <stdlib.h>     // free()
#include <string.h>     // strdup(), strerror()
//#include <varargs.h>    // va_arg(), not needed for Win32/gcc/g++, Win32/MSVC8 C/C++

// other own headers (none): here, after system headers, to prevent errors in
// our headers to show in the context of system headers.

// Workaround for gcc -std=c99
// http://stackoverflow.com/questions/5573775/strdup-error-on-g-with-c0x
#if defined( __GNUC__ ) && (__STDC_VERSION__ >= 199901L )
extern _CRTIMP char* __cdecl __MINGW_NOTHROW _strdup (const char*) __MINGW_ATTRIB_MALLOC;
#endif

/**
 * local function to encapsulate program path string;
 * initializes with dynamic string so that handling (free/dup) is the same everywhere.
 */
static char const * * program_path_ptr( void )
{
   static const char * program_path = NULL;

   if ( NULL == program_path )
   {
      program_path = _strdup( "(program name not set)" );
   }

   return &program_path;
}

/**
 * the program path as set, or default "(...)" if not set.
 */
char const * const error_program_path( void )
{
   return *program_path_ptr();
}

/**
 * set program path (filename) for error message prefix;
 * return true on error.
 */
int set_error_program_path( char const * const path )
{
   free( (void*) *program_path_ptr() );
   *program_path_ptr() = _strdup( path );

   return NULL == *program_path_ptr();
}

/**
 * format error message, return status;
 * for errors from functions that set errno, use os_error().
 *
 * Example message: "progname: filename(line): non-numeric line content 'content'".
 *
 * Usage: exit( error( EXIT_FAILURE, "%s(%d): non-numeric line content '%s'", filename, lineno, content ) );
 */
int error( int const status, char const * const fmt, ... )
{
   va_list argptr;
   va_start( argptr, fmt );
   (void) fprintf( stderr, "%s: ", error_program_path() );
   (void) vfprintf( stderr, fmt, argptr );
   (void) fprintf( stderr, "\n" );
   va_end( argptr  );

   return status;
}

/**
 * format error message, return status;
 * use for errors from functions that set errno; see also error().
 *
 * Example message: "progname: cannot open file 'filename': file does not exist".
 *
 * Usage: exit( error( EXIT_FAILURE, "cannot open file '%s'", path ) );
 */
int os_error( int const status, char const * const fmt, ... )
{
   va_list argptr;
   va_start( argptr, fmt );
   (void) fprintf( stderr, "%s: ", error_program_path() );
   (void) vfprintf( stderr, fmt, argptr );
   (void) fprintf( stderr, ": %s\n", strerror( errno ) );
   va_end( argptr  );

   return status;
}

/*
 * Compilation and quick functional test.
 */
#ifdef TEST_ERROR

// -Wall for VC8 generates warnings for system headers :(
// MSVC8-C  : cl  -W4 -DTEST_ERROR -D_CRT_SECURE_NO_WARNINGS -TC ecore.c
// MSVC8-C++: cl  -W4 -DTEST_ERROR -D_CRT_SECURE_NO_WARNINGS -TP ecore.c
// GNU C    : gcc -Wall -DTEST_ERROR -std=c99 ecore.c
// GNU C++  : g++ -Wall -DTEST_ERROR ecore.c

#include "ecore.h"      // include guard check

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

/**
 * exercise error(), os_error().
 */
int main( int argc, char *argv[] )
{
   UNUSED( argc );

   (void) error( EXIT_FAILURE,
      "this is an error message from funtion '%s()' in file '%s'.",
         __FUNCTION__, __FILE__ );

   (void) set_error_program_path( argv[0] );

   {
      char const * const filename = "non-existing-file.$$$";

      (void) fopen( filename , "r");

      return os_error( EXIT_FAILURE,
         "in funtion '%s()', failed to open file '%s'",
            __FUNCTION__, filename );
   }

   return EXIT_SUCCESS;
}
#endif

/*
 * end of file
 */
