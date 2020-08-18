
#include "crt_proxy_app.h"

int main( const int argc, char * argv[] )
{
    namespace cpl =  crt_proxy_lib;

    const char* input_ = "ABRA\v\n\f\r\t CA DABRA";

    auto rezult_ = cpl::strlen(input_);

    static_assert(19 == cpl::strlen("ABRA\v\n\f\r\t CA DABRA").value );

    static_assert(1 == cpl::strlen("").value );

    const char* nullstr{};

    auto [val, stat] = cpl::strlen(nullstr);

    if (!val)
    {
        // already logged
        auto msg = stat;
    }

    return 0;
}

