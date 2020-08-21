#ifndef CRT_PROXY_LIB_LOG_DEFAULT_INC_
#define CRT_PROXY_LIB_LOG_DEFAULT_INC_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ 

Users of crt proxy lib need to deliver header named crt_proxy_lib_log.h with actual logging implementation
Macros to be defined are:

     CRT_PROXY_LIB_LOG_TRACE(...) 
     CRT_PROXY_LIB_LOG_DEBUG(...) 
     CRT_PROXY_LIB_LOG_INFO(...) 
     CRT_PROXY_LIB_LOG_WARN(...) 
     CRT_PROXY_LIB_LOG_ERROR(...)
     CRT_PROXY_LIB_LOG_FATAL(...)

     Syntax is same as for the printf. 

if this header is not defined crt_proxy_lib_log_default.h is used which prints to stderr.


*/

#include <assert.h> //
#include <stdio.h> // _snprintf_s

template<typename ... Args >
inline auto crt_proxy_lib_log_default (
    const char* file,
    const long   line,
    const char * level_tag ,
    Args ... args
)
{
    // the part without a file and line prefix
    char bufy_[1024]{ 0 };
    int rez = _snprintf_s(bufy_, 1024, _TRUNCATE, args ...);
    assert(rez > 0);
    // 
    char file_line_prefix[1024]{ 0 };
    rez = _snprintf_s(file_line_prefix, 1024, _TRUNCATE, "%s : %d : %10s", file, line, level_tag);
    assert(rez > 0);

    static char rezult_buffer[2048 + 1]{ 0 };
    // append the two and send out
    rez = _snprintf_s(rezult_buffer, 2048, _TRUNCATE, "%s : %s ", file_line_prefix, bufy_ );
    assert(rez > 0);

    rez = fprintf(stderr, "\n%s", rezult_buffer);
    assert(rez > 0);

    return rezult_buffer; // use immediately
}

 // log and return
// file and line are logged but not returned
#undef CRT_PROXY_LIB_LOG_TRACE
#define CRT_PROXY_LIB_LOG_TRACE(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_TRACE" , __VA_ARGS__  )

#undef CRT_PROXY_LIB_LOG_DEBUG
#define CRT_PROXY_LIB_LOG_DEBUG(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_DEBUG" , __VA_ARGS__  )

#undef CRT_PROXY_LIB_LOG_INFO
#define CRT_PROXY_LIB_LOG_INFO(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_INFO" , __VA_ARGS__  )

#undef CRT_PROXY_LIB_LOG_WARN
#define CRT_PROXY_LIB_LOG_WARN(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_WARN" , __VA_ARGS__  )

#undef CRT_PROXY_LIB_LOG_ERROR
#define CRT_PROXY_LIB_LOG_ERROR(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_ERROR" , __VA_ARGS__  )

#undef CRT_PROXY_LIB_LOG_FATAL
#define CRT_PROXY_LIB_LOG_FATAL(...) \
crt_proxy_lib_log_default( __FILE__, __LINE__ , "LOG_FATAL" , __VA_ARGS__  )

#endif // CRT_PROXY_LIB_LOG_DEFAULT_INC_
