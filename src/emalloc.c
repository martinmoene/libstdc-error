/*
 * emalloc.c
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

// header
#include "emalloc.h"

// system include files
#include <errno.h>      // errno
#include <stdlib.h>     // malloc(), free()

// other own include files
#include "ecore.h"

/**
 * malloc with error handling (no error location); exit on error.
 */
void * emalloc( size_t bytes, char const * const var )
{
   void * p = malloc( bytes );

   if ( NULL == p )
   {
      exit( os_error( EXIT_FAILURE, "failed to allocate %u bytes for variable '%s'", bytes, var ) );
   }

   return p;
}

/**
 * realloc with error handling (no error location); exit on error.
 */
void * erealloc( void * p, size_t bytes, char const * const var )
{
   void * p_new = realloc( p, bytes );

   if ( NULL == p_new )
   {
      exit( os_error( EXIT_FAILURE, "failed to reallocate %u bytes for variable '%s'", bytes, var ) );
   }

   return p_new;
}

/**
 * free with error handling (no error location); exit on error.
 */
void efree( void * p, char const * const var )
{
   free( p );

   if ( errno )
   {
      exit( os_error( EXIT_FAILURE, "failed to free memory for variable '%s'", var ) );
   }
}

/**
 * malloc with error handling (includes error location); exit on error.
 */
void * emalloc_loc( size_t bytes, char const * const var, char const * const filename, char const * const function, int const line )
{
   void * p = malloc( bytes );

   if ( NULL == p )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to allocate %u bytes for variable '%s'", filename, line, function, bytes, var ) );
   }

   return p;
}

/**
 * realloc with error handling (includes error location); exit on error.
 */
void * erealloc_loc( void * p, size_t bytes, char const * const var, char const * const filename, char const * const function, int const line )
{
   void * p_new = realloc( p, bytes );

   if ( NULL == p_new )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to reallocate %u bytes for variable '%s'", filename, line, function, bytes, var ) );
   }

   return p_new;
}

/**
 * free with error handling (includes error location); exit on error.
 */
void efree_loc( void * p, char const * const var, char const * const filename, char const * const function, int const line )
{
   free( p );

   if ( errno )
   {
      exit( os_error( EXIT_FAILURE, "%s(%i): in function '%s()' failed to free memory for variable '%s'", filename, line, function, var ) );
   }
}


#ifdef TEST_EMALLOC
// -Wall for VC8 generates warnings for system headers :(
// MSVC8-C  : cl  -W4 -DTEST_EMALLOC -D_CRT_SECURE_NO_WARNINGS -TC emalloc.c ecore.c
// MSVC8-C++: cl  -W4 -DTEST_EMALLOC -D_CRT_SECURE_NO_WARNINGS -TP emalloc.c ecore.c
// GNU C    : gcc -Wall -DTEST_EMALLOC -std=c99 emalloc.c ecore.c
// GNU C++  : g++ -Wall -DTEST_EMALLOC emalloc.c ecore.c

#include "emalloc.h"     // include guard check

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

/**
 * exercise EMALLOC(), EREALLOC(), EFREE().
 */
int main( int argc, char *argv[] )
{
   UNUSED( argc );

   set_error_program_path( argv[0] );

   {
      void * p = EMALLOC( 123, p );
      p = EREALLOC( p, 512 );
      EFREE( p );
   }

   return EXIT_SUCCESS;
}

#endif
