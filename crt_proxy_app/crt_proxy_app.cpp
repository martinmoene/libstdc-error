/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#include "crt_proxy_app.h"

#define SX(fmt_, x_) log_info("%s : " fmt_, #x_, x_ )

static inline void test_strlen()
{
    namespace cpl = crt_proxy_lib;

    const char* input_ = "ABRA\v\n\f\r\t CA DABRA";

    SX( "%d", cpl::strlen(input_) );

    SX("%d" , 19 == cpl::strlen("ABRA\v\n\f\r\t CA DABRA").value);

    SX("%d" , 1 == cpl::strlen("").value);

    const char* nullstr{};

    auto [val, stat] = cpl::strlen(nullstr);

    if (!val)
    {
        // already logged
        auto msg = stat;
        log_warn("Already logged: %s", msg);
    }

    SX("%s", cpl::strcmp("", "OK").status );
    SX("%s", cpl::strcmp(0, "OK").status );

    SX( "%d" , *(cpl::strcmp("NOT", "OK").value) );
    SX( "%d" , *(cpl::strcmp("OK", "OK").value) );

}

struct dbj_simplelog_finalizer final {

    ~dbj_simplelog_finalizer() {
            dbj_log_finalize();
        }
};

inline dbj_simplelog_finalizer dsf_ ;

int main( const int argc, char * argv[] )
{
    if (EXIT_SUCCESS != dbj_simple_log_startup(argv[0]))
        return EXIT_FAILURE;

    test_strlen() ;

    printf("\n\n");

   return EXIT_SUCCESS ;
}

