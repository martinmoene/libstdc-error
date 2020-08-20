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
/*
 crt on winows
------------------------------------------------------------------------------
Each and every crt function on windows is implemented by calling WIN32
Using crt is just a shortcut.
win console is win32 app with std streams created and attached
C/C++ real life apps are not console apps. and very likely are not GUI apps
they are just server side apps. invisible. 

Policy for apps and components not having a console
------------------------------------------------------------------------------
windows app (service or whatever) initially has no associated streams
simple and good policy is to redirect the stderr (on startup)
to the "stderr log" file

   app_full_path + "stderr.log" 

Each data center or just simply server side app will have some logging used.
This is usually not a log file. There might be  runtime situtations when it is not
possible to create this file. Do not exit or crash in such a event. 

 *use functions not macros*

Global Macro 	            Functional Equivalents
------------------------------------------------------------------------------
_doserrno 	                _get_doserrno, _set_doserrno
errno 	                    _get_errno, _set_errno
_sys_errlist, _sys_nerr 	strerror_s, _strerror_s, _wcserror_s, __wcserror_s

*/

#undef  DBJ_PERROR 
#ifdef _DEBUG
#define DBJ_PERROR (perror(__FILE__ " # " _CRT_STRINGIZE(__LINE__))) 
#else
#define DBJ_PERROR
#endif // _DEBUG

#undef DBJ_FERROR
#ifdef _DEBUG
#define DBJ_FERROR( FP_) \
do { \
if (ferror(FP_) != 0) {\
	DBJ_PERROR ;\
	clearerr_s(FP_);\
} \
} while(0)
#else
#define DBJ_FERROR( FP_ )
#endif // _DEBUG

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
/*
MS DOCS are saying:

Always clear errno by calling _set_errno(0) immediately before a call that may set it, 
and check it immediately after the call.

Because we use valstat to pass the metastate we ca allways return INFO metastate 
with the errno obtained immediately after the call

        _set_errno(0);
    // INFO metastate
        return { ::strlen(input_), errno };

But that puts the burden on the callers. And in reality most of the CRT funtions do not set
errno on proper arguments given, which we check here anyway.
*/
        _set_errno(0);
    // OK metastate
        return { ::strlen(input_), {} };
}

} //  namespace crt_proxy_lib