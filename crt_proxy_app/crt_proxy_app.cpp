/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#include "crt_proxy_app.h"

#define SX_FMT(fmt_, x_) log_info("%s : " fmt_, #x_, x_ )

static inline void test_string_h_proxy()
{
    namespace cpl = crt_proxy_lib;

    const char* input_ = "ABRA\v\n\f\r\t CA DABRA";

    metatest( cpl::strlen(input_) );

    metatest( cpl::strlen("ABRA\v\n\f\r\t CA DABRA") );

    metatest( cpl::strlen("") );

    const char* nullstr{};

    metatest( cpl::strlen(nullstr) ) ;

    metatest( cpl::strcmp("", "OK") );
    metatest( cpl::strcmp(0, "OK") );

    metatest( cpl::strcmp("NOT", "OK") );
    metatest( cpl::strcmp("OK", "OK") );

}

struct dbj_simplelog_finalizer final {
    ~dbj_simplelog_finalizer() {
            dbj_log_finalize();
        }
};

static dbj_simplelog_finalizer dsf_ ;

int main( const int argc, char * argv[] )
{
    if (EXIT_SUCCESS != dbj_simple_log_startup(argv[0]))
        return EXIT_FAILURE;

    test_string_h_proxy() ;

    printf("\n\n");

   return EXIT_SUCCESS ;
}

