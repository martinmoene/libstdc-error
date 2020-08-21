#ifndef CRT_PROXY_LIB_LOG_INC_
#define CRT_PROXY_LIB_LOG_INC_

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

---------------------------------------------------------------------------------------------------

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

*/

/*
This is a header delivered by users who use dbj--simplelog
https://github.com/dbj-systems/dbj--simplelog
*/

// all eventually goes in here
extern "C" {
#ifndef DBJ_LOG_LEVELS_ENUM_DEFINED
#define DBJ_LOG_LEVELS_ENUM_DEFINED
    typedef enum DBJ_LOG_LEVELS_ENUM { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL } DBJ_LOG_LEVELS_ENUM;
#endif // DBJ_LOG_LEVELS_ENUM_DEFINED

    void log_log(int /*level*/, const char* /*file*/, int /*line*/, const char* /*fmt*/, ...);
}

 // log and return
// file and line are logged but not returned
#undef CRT_PROXY_LIB_LOG_TRACE
#define CRT_PROXY_LIB_LOG_TRACE(...) \
(log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#undef CRT_PROXY_LIB_LOG_DEBUG
#define CRT_PROXY_LIB_LOG_DEBUG(...) \
(log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#undef CRT_PROXY_LIB_LOG_INFO
#define CRT_PROXY_LIB_LOG_INFO(...) \
(log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#undef CRT_PROXY_LIB_LOG_WARN
#define CRT_PROXY_LIB_LOG_WARN(...) \
(log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#undef CRT_PROXY_LIB_LOG_ERROR
#define CRT_PROXY_LIB_LOG_ERROR(...) \
(log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#undef CRT_PROXY_LIB_LOG_FATAL
#define CRT_PROXY_LIB_LOG_FATAL(...) \
(log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__),   #__VA_ARGS__ )

#endif // CRT_PROXY_LIB_LOG_INC_
