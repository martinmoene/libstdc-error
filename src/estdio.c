#define _CRT_SECURE_NO_WARNINGS 1

/*
 * estdio.c
 *
 * Provide error checked version of function from stdio.h.
 *
 * Created on 2 July 2011, by Martin Moene
 *
 * Copyright 2011 Universiteit Leiden
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// header
#include "estdio.h"

// system include files
#include <stdarg.h>     // va_arg()
#include <stdlib.h>     // EXIT_FAILURE
//#include <varargs.h>    // va_arg(), not needed for Win32/gcc/g++, Win32/MSVC8 C/C++

// other own include files
#include "ecore.h"

/**
 * fopen with error handling; exit on error.
 */
FILE * efopen( char const * const path, char const * const mode )
{
   FILE * fp = fopen( path, mode );

   if ( NULL == fp )
   {
      exit( os_error( EXIT_FAILURE, "cannot open file '%s', mode '%s'", path, mode ) );
   }

   return fp;
}

/**
 * fclose with error handling; exit on error.
 */
void efclose( FILE * fp, char const * const path )
{
   if ( EOF == fclose( fp ) )
   {
      exit( os_error( EXIT_FAILURE, "cannot close file '%s'", path ) );
   }
}

/**
 * fprintf with error handling; exit on error.
 */
void efprintf( FILE * fp, char const * const filename, char const * const fmt, ... )
{
   va_list argptr;
   va_start( argptr, fmt );

   if ( 0 > vfprintf( fp, fmt, argptr ) )
   {
      exit( os_error( EXIT_FAILURE, "error while writing to file '%s'", filename ) );
   }

   va_end( argptr );
}

/**
 * fopen with error handling (includes error location); exit on error.
 */
FILE * efopen_loc( char const * const path, char const * const mode, char const * const filename, char const * const function, int const line )
{
   FILE * fp = fopen( path, mode );

   if ( NULL == fp )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to open file '%s', mode '%s'", filename, line, function, path, mode ) );
   }

   return fp;
}

/**
 * fclose with error handling (includes error location); exit on error.
 */
void efclose_loc( FILE * fp, char const * const path, char const * const filename, char const * const function, int const line )
{
   if ( EOF == fclose( fp ) )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to close file '%s'", filename, line, function, path ) );
   }
}

/**
 * fprintf with error handling (includes error location); exit on error.
 */
void efprintf_loc( FILE * fp, char const * const path, char const * const filename, char const * const function, int const line , char const * const fmt, ... )
{
   va_list argptr;
   va_start( argptr, fmt );

   if ( 0 > vfprintf( fp, fmt, argptr ) )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to write to file '%s'", filename, line, function, path ) );
   }

   va_end( argptr );
}


#ifdef TEST_ESTDIO
// -Wall for VC8 generates warnings for system headers :(
// MSVC8-C  : cl  -W4 -DTEST_ESTDIO -D_CRT_SECURE_NO_WARNINGS -TC estdio.c ecore.c
// MSVC8-C++: cl  -W4 -DTEST_ESTDIO -D_CRT_SECURE_NO_WARNINGS -TP estdio.c ecore.c
// GNU C    : gcc -Wall -DTEST_ESTDIO -std=c99 estdio.c ecore.c
// GNU C++  : g++ -Wall -DTEST_ESTDIO estdio.c ecore.c

#include "estdio.h"     // include guard check

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

/**
 * exercise efopen(), efprintf(), efclose().
 */
int main( int argc, char *argv[] )
{
   UNUSED( argc );

   set_error_program_path( argv[0] );

   EPRINTF0 ( "EPRINTF0: a string without parameters expecting to succeed.\n" );
   EPRINTF  ( "EPRINTF: report of N (%i) expecting to succeed.\n", 22 );
   EFPRINTF0( stdout, "stdout", "EFPRINTF0: a string without parameters expecting to succeed.\n" );
   EFPRINTF ( stdout, "stdout", "EFPRINTF: report of N (%i) expecting to succeed.\n", 22 );

   efprintf ( stdout, "stdout", "efprintf: report of N (%i) expecting to succeed.\n", 33 );

   // create file namd below as read only to test write-failure:
//   {
//      char const * const ro_filename = "estdio-test-ro.tmp";
//
//      FILE * ro_fp = efopen( ro_filename, "w" );
//      efprintf( ro_fp, ro_filename, "expecting to fail to report to ro_fp filepointer");
//      efclose( ro_fp, ro_filename );
//   }

   // create file namd below as read only to test write-failure:
   {
      char const * const ro_filename = "estdio-test-ro.tmp";

      FILE * ro_fp = EFOPEN( ro_filename, "w" );
      EFPRINTF( ro_fp, ro_filename, "expecting to fail to report to ro_fp filepointer", 0);
      EFCLOSE( ro_fp, ro_filename );
   }

   efopen( "estdio-test-non-existing-file.tmp", "r");

   return EXIT_SUCCESS;
}

#endif
