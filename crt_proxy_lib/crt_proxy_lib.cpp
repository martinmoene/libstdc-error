/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

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
#include <locale.h>
/*
 crt on winows
------------------------------------------------------------------------------
Each and every crt function on windows is implemented by calling WIN32
Using crt is just a shortcut to WIN32 API.

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
valstat<size_t> strlen ( const char * & input_ ) noexcept
{
     CRT_PROXY_LIB_PADLOCK;

        if (nullptr == input_)
            // ERROR metastate
            // same message is loged and returned
            return { {}, CRT_PROXY_LIB_LOG_ERROR(" null argument") };
/*
MS DOCS are saying:

Always clear errno by calling _set_errno(0) immediately before a call that may set it, 
and check it immediately after the call.

Because we use valstat to pass the metastate we can allways return INFO metastate 
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

//////////////////////////////////////////////////////////////////////////////////////////
// strcmp proxy begins here
// returned valstat is defined here, not before
valstat< int >
strcmp(const char* lhs_, const char* rhs_)
{
    CRT_PROXY_LIB_PADLOCK;

    if (!lhs_ || !rhs_)
        // ERROR metastate return
        // value is empty, status is not
        return { {}, CRT_PROXY_LIB_LOG_ERROR("null pointer argument received") };

    if (is_empty(lhs_) || is_empty(rhs_))
        // ERROR metastate return
        return { {}, CRT_PROXY_LIB_LOG_ERROR("empty argument received") };

    // OK metastate return
    // value is not empty, status is
    return { ::strcmp(lhs_, rhs_) , {} };
}
//////////////////////////////////////////////////////////////////////////////////////////
valstat< void* >    memccpy(void* CRT_PROXY_LIB_RESTRICT, const void* CRT_PROXY_LIB_RESTRICT, int, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< void* >    memchr(const void*, int, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< int >      memcmp(const void*, const void*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< void* >    memcpy(void* CRT_PROXY_LIB_RESTRICT, const void* CRT_PROXY_LIB_RESTRICT, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< void* >    memmove(void*, const void*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< void* >    memset(void*, int, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    stpcpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    stpncpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strcat(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strchr(const char*, int){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< int >      strcoll(const char*, const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< int >      strcoll_l(const char*, const char*, _locale_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strcpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< size_t >   strcspn(const char*, const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strdup(const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strerror(int){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strerror_l(int, _locale_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< int >      strerror_r(int, char*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

// see above -- valstat< size_t >   strlen(const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strncat(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< int >      strncmp(const char*, const char*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strncpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strndup(const char*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< size_t >   strnlen(const char*, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strpbrk(const char*, const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strrchr(const char*, int){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strsignal(int){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< size_t >   strspn(const char*, const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strstr(const char*, const char*){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }
valstat< char* >    strtok(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< char* >    strtok_r(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, char** CRT_PROXY_LIB_RESTRICT){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< size_t >   strxfrm(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

valstat< size_t >   strxfrm_l(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t, _locale_t){  CRT_PROXY_LIB_PADLOCK ;  return {  {} , CRT_PROXY_LIB_LOG_ERROR("not yet implemented") };  }

} //  namespace crt_proxy_lib