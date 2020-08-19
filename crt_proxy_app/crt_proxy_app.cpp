
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

int main( const int argc, char * argv[] )
{
    if (EXIT_SUCCESS == dbj_simple_log_startup( argv[0] ))
    {
        auto lfp = current_log_file_path();

        FILE* fp_ = dbj_fhandle_log_file_ptr(NULL);

        assert(fp_);

        int errcode = ferror(fp_);
        if (errcode != 0) {
            perror( __FILE__ " # " _CRT_STRINGIZE(__LINE__) );
            clearerr_s(fp_);
        }

        int numflushed = _flushall();
        fprintf(stderr, "There were %d streams flushed\n", numflushed);
        
        if (fp_) { ::fclose(fp_); fp_ = nullptr; }
    }
    else
        return EXIT_FAILURE;
}

