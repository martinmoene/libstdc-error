#ifndef CRT_PROXY_LIB_LOG_INC_
#define CRT_PROXY_LIB_LOG_INC_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ 

In WIN32 parlance module is a component.

Logging is important

Win console is win32 app with std streams created and attached
C/C++ real life apps are not console apps. And (very likely) are not GUI apps
they are just server side apps. Invisible. 

Each data center or just simply server side component will have to have some logging used.
Without logging in place admins can not see what is going on with your component.

Very basic and very default policy is to redirect stderr to a file.

What about stdout?

In win32 modules stdout simply leads to nowhere. be carefull and vigilant not to use stdout in standard win modules.
sometimes in very rare (legacy usage) situations one redirects stdout to another file, on startup.

crt poxy lib loging

The requirement is: log the string message and return what have you logged.

WG21 C++20 wisdom

Who care about UTF-8? The whole internet cares. Logging mesages are not unicode. why not? because C++20 brings char8_t into play.
and that "boson" is implemented as char, 
that is utf8 string is coded into char *, not wchar_t * ... that means

// C++20 ... or before, it depends
// Windows is wchar_t default executable char set
char * hiragana = u8"平仮名" ;

Your system and modules will send and receive JSON messages coded in UTF-8. 
And Linux/Mac code simply does:

// Linux/Unix OS 
// UTF-8 is the executable char set by default
char * hiragana = "平仮名" ;

So we better stick to char *

The users of crt_proxy_lib need to provide the following six functions.

// format the message, log it and return it
template<typename ... Args >
const char * crt_proxy_lib_log_trace( 
    const char * file, 
    const long   line,
    const char * fmt_string, 
    Args ... args
    );

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

*/

#undef CRT_PROXY_LIB_LOG_TRACE
#define CRT_PROXY_LIB_LOG_TRACE(M_) crt_proxy_lib_log_trace(  __FILE__ , __LINE__, "%s(%d) : %s() : %s", __func__, M_ )

#endif // CRT_PROXY_LIB_LOG_INC_
