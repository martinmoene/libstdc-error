#include "ecore.h"
#include "emalloc.h"

int main( int argc, char *argv[] )
{
   set_error_program_path(argv[0]);

   {
      void * p = EMALLOC( 123, p );
      p = EREALLOC( p, 512 );
      EFREE( p );
   }

   return 0;
}
