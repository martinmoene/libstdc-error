
#include "crt_proxy_app.h"

static inline void test_strlen()
{
    namespace cpl = crt_proxy_lib;

    const char* input_ = "ABRA\v\n\f\r\t CA DABRA";

    auto rezult_ = cpl::strlen(input_);

    static_assert(19 == cpl::strlen("ABRA\v\n\f\r\t CA DABRA").value);

    static_assert(1 == cpl::strlen("").value);

    const char* nullstr{};

    auto [val, stat] = cpl::strlen(nullstr);

    if (!val)
    {
        // already logged
        auto msg = stat;
    }

}

struct dbj_simplelog_finalizer final {

    ~dbj_simplelog_finalizer() {
        FILE* fp_ = dbj_fhandle_log_file_ptr(NULL);
        if (fp_) {
            DBJ_FERROR(fp_);
            #ifdef _DEBUG
            int numflushed = _flushall();
            fprintf(stderr, "There were %d streams flushed\n", numflushed);
            #else
            (void)_flushall();
            #endif
               if (fp_) { ::fclose(fp_); fp_ = nullptr; }
        }
    }
};

inline dbj_simplelog_finalizer dsf_ ;

int main( const int argc, char * argv[] )
{
    if (EXIT_SUCCESS != dbj_simple_log_startup(argv[0]))
        return EXIT_FAILURE;

    test_strlen() ;
   return EXIT_SUCCESS ;
}

