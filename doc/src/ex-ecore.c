#include "ecore.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
   (void) set_error_program_path( argv[0] );

   (void) error( EXIT_FAILURE,
      "this is an error message from funtion '%s()' in file '%s'.",
         __FUNCTION__, __FILE__ );

   {
      char const * const filename = "non-existing-file.$$$";

      FILE * fp = fopen( filename , "r");

      if ( NULL == fp )
      {
         return os_error( EXIT_FAILURE,
            "in funtion '%s()', failed to open file '%s'",
               __FUNCTION__, filename );
      }
   }

   return 0;
}
