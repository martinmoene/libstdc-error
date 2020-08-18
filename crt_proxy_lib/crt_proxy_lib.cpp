#include "crt_proxy_lib.h"

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

namespace crt_proxy_lib
{
// default lock/unlock is no lock/unlock

// default logging is going to stderr
// reminder: stderr is buffered by default

valstat<size_t> strlen ( const char * & input_ ) noexcept
{
     CRT_PROXY_LIB_PADLOCK;

        if (nullptr == input_)
            // ERROR metastate
            // same message is loged and returned
            return { {}, CRT_PROXY_LIB_LOG("ERROR : null argument") };
    
    // OK metastate
        return { ::strlen(input_), {} };
}

} //  namespace crt_proxy_lib