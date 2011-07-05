#include "ecore.h"
#include "estdio.h"

int main( int argc, char *argv[] )
{
   set_error_program_path( argv[0] );

   EPRINTF0 ( "EPRINTF0: a string without parameters expecting to succeed.\n" );
   EPRINTF  ( "EPRINTF: report of N (%i) expecting to succeed.\n", 22 );
   EFPRINTF0( stdout, "stdout", "EFPRINTF0: a string without parameters expecting to succeed.\n" );
   EFPRINTF ( stdout, "stdout", "EFPRINTF: report of N (%i) expecting to succeed.\n", 22 );

   efprintf( stdout, "stdout", "report of N (%i) expecting to succeed.\n", 33 );

   // create file named below as read only to test write-failure:
   {
      char const * const ro_filename = "estdio-test-ro.tmp";

      FILE * ro_fp = EFOPEN( ro_filename, "w" );
      EFPRINTF0( ro_fp, ro_filename, "expecting to fail to report to ro_fp filepointer");
      EFCLOSE( ro_fp, ro_filename );
   }

   efopen( "estdio-test-non-existing-file.tmp", "r");

   return 0;
}
