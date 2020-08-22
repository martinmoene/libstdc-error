#ifndef CRT_PROXY_LIB_INC_
#define CRT_PROXY_LIB_INC_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#ifndef _MSC_VER
#error Sorry ... Windows only.
#endif

#define CRT_PROXY_LIB_VERSION "0.1.0"

#ifndef   CRT_PROXY_LIB_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define CRT_PROXY_LIB_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define CRT_PROXY_LIB_CPLUSPLUS  __cplusplus
# endif
#endif

#define CRT_PROXY_LIB_CPP98_OR_GREATER  ( CRT_PROXY_LIB_CPLUSPLUS >= 199711L )
#define CRT_PROXY_LIB_CPP11_OR_GREATER  ( CRT_PROXY_LIB_CPLUSPLUS >= 201103L )
#define CRT_PROXY_LIB_CPP14_OR_GREATER  ( CRT_PROXY_LIB_CPLUSPLUS >= 201402L )
#define CRT_PROXY_LIB_CPP17_OR_GREATER  ( CRT_PROXY_LIB_CPLUSPLUS >= 201703L )
#define CRT_PROXY_LIB_CPP20_OR_GREATER  ( CRT_PROXY_LIB_CPLUSPLUS >= 202000L )

#if ! CRT_PROXY_LIB_CPP17_OR_GREATER
#error crt proxy lib requires C++17 or better
#endif

#undef CRT_PROXY_LIB_NDEBUG
#ifndef _DEBUG
#define CRT_PROXY_LIB_NDEBUG
#endif // !_DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////////

// default lock/unlock is no lock / no unlock
// user defined padlock type releases semaphore/mutex/critical section in destructor
// grabs in constructor
// crt_proxy_lib_lock.h, must define CRT_PROXY_LIB_PADLOCK

#if __has_include("crt_proxy_lib_lock.h")
    #include "crt_proxy_lib_lock.h"
#ifndef CRT_PROXY_LIB_PADLOCK
#error  CRT_PROXY_LIB_PADLOCK is not defined?
#endif // ! CRT_PROXY_LIB_LOCK
#else
// defualt is no locking
    #ifndef CRT_PROXY_LIB_PADLOCK
    #define CRT_PROXY_LIB_PADLOCK
    #endif // ! CRT_PROXY_LIB_LOCK
#endif // 

/*
Users of crt proxy lib need to deliver header named crt_proxy_lib_log.h with actual logging implementation
Macros to be defined are:

     CRT_PROXY_LIB_LOG_TRACE(...)
     CRT_PROXY_LIB_LOG_DEBUG(...)
     CRT_PROXY_LIB_LOG_INFO(...)
     CRT_PROXY_LIB_LOG_WARN(...)
     CRT_PROXY_LIB_LOG_ERROR(...)
     CRT_PROXY_LIB_LOG_FATAL(...)

     Syntax is same as for the printf.

if this header is not defined crt_proxy_lib_log_default.h is used which 
defines macros that print to stderr.
*/
#if __has_include("crt_proxy_lib_log.h")
#include "crt_proxy_lib_log.h"
#else
#include "crt_proxy_lib_log_default.h"
#endif // 


//////////////////////////////////////////////////////////////////////////////////////////////////

/* until std::valstat  we use dbj::valstat*/
// valstat is not (yet) in the std namespace
namespace dbj {
    template <typename T_, typename S_ >
    struct valstat final {
        using type        = valstat ;
        using value_type  = T_;
        using status_type = S_;

        value_type  value ;
        status_type status ;
    };  // valstat 
} // dbj

// CRT_PROXY_LIB bare requires types handled to be
// defualt constructible and copyable
// in the context of CRT types that should work
// as they are all fundamental types
// probably the best requirement would be to demand
// T to yield true from std::is_standard_layout<T>
#include "optional_bare.h"

namespace crt_proxy_lib 
{
    // CRT uses only fundamental types , besides pointers and few trivial layout structs
    // T is any fundamental type used. nonstd bare CRT_PROXY_LIB permitting.
    template <typename T_ >
    using valstat = dbj::valstat<
        // value type
        nonstd::optional<T_> ,
        // status message
        const char *
    > ;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// crt proxy functions begin here
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __cplusplus
    #define CRT_PROXY_LIB_RESTRICT restrict
#else
    #define CRT_PROXY_LIB_RESTRICT __restrict 
#endif // __cplusplus

// NOTE: we do use CRT names in this namespace and that compiles
// which means these names can not be used without namespace prefix
// of course one can and will do something like
//   namespace cpl =  crt_proxy_lib;

// strlen overloads
// unlike CRT we can use overloads here
// notice the argument type required so overloads bellow are captured
// this overload uses crt strlen and is thus hidden inside the cpp
// instead of crashing the app we return ERROR metastate
// on null argument used
valstat<size_t> strlen ( const char * & ) noexcept;

template<size_t N>
constexpr inline 
// we always return a valstat for the uniformity of calling algorithms
// in this case we do not need our valstat alias
// we do not use nonstd optional here
// notice how we treat valstat as literal here
dbj::valstat<size_t, const char * >
strlen ( const char (*str)[N] ) noexcept
{
    return { N, {} }; // OK metastate
}

template<size_t N>
constexpr inline 
dbj::valstat<size_t, const char* >
strlen ( const char (&str)[N] ) noexcept
{
    return { N, {} }; // OK metastate
}

//////////////////////////////////////////////////////////////////////////////////////////
/// neither CRT or std lib have the paradigm of "empty" properly grasped ... yet
/// here we will provide is_empty overloads that operate on values 
/// not on types like std::empty() does
 
/// empty string
constexpr bool is_empty(const char* const s_) noexcept {
    return (s_ && (s_[0] == '\0'));
}

// empty char array
template<size_t N>
constexpr bool is_empty(const char(&s_) [N]) noexcept {
    return (s_[0] == '\0') ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// proxyes for <string.h>
//
valstat< int >      strcmp(const char* lhs_, const char* rhs_);
valstat< void* >    memccpy(void* CRT_PROXY_LIB_RESTRICT, const void* CRT_PROXY_LIB_RESTRICT, int, size_t);

valstat< void* >    memchr(const void*, int, size_t);
valstat< int >      memcmp(const void*, const void*, size_t);
valstat< void* >    memcpy(void* CRT_PROXY_LIB_RESTRICT, const void* CRT_PROXY_LIB_RESTRICT, size_t);
valstat< void* >    memmove(void*, const void*, size_t);
valstat< void* >    memset(void*, int, size_t);

valstat< char* >    stpcpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT);
valstat< char* >    stpncpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t);

valstat< char* >    strcat(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT);
valstat< char* >    strchr(const char*, int);
valstat< int >      strcmp(const char*, const char*);
valstat< int >      strcoll(const char*, const char*);

valstat< int >      strcoll_l(const char*, const char*, _locale_t);

valstat< char* >    strcpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT);
valstat< size_t >   strcspn(const char*, const char*);

valstat< char* >    strdup(const char*);

valstat< char* >    strerror(int);

valstat< char* >    strerror_l(int, _locale_t);
valstat< int >      strerror_r(int, char*, size_t);

// valstat< size_t >   strlen(const char*);
valstat< char* >    strncat(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t);
valstat< int >      strncmp(const char*, const char*, size_t);
valstat< char* >    strncpy(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t);

valstat< char* >    strndup(const char*, size_t);
valstat< size_t >   strnlen(const char*, size_t);

valstat< char* >    strpbrk(const char*, const char*);
valstat< char* >    strrchr(const char*, int);

valstat< char* >    strsignal(int);

valstat< size_t >   strspn(const char*, const char*);
valstat< char* >    strstr(const char*, const char*);
valstat< char* >    strtok(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT);

valstat< char* >    strtok_r(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, char** CRT_PROXY_LIB_RESTRICT);

valstat< size_t >   strxfrm(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t);

valstat< size_t >   strxfrm_l(char* CRT_PROXY_LIB_RESTRICT, const char* CRT_PROXY_LIB_RESTRICT, size_t, _locale_t);

//////////////////////////////////////////////////////////////////////////////////////////////////

} //  namespace crt_proxy_lib

/*

in this release we shall implement string.h functions

string.h synopsis
https://pubs.opengroup.org/onlinepubs/9699919799/


The <string.h> header shall define NULL and size_t as described in <stddef.h>.

    The <string.h> header shall define the locale_t type as described in <locale.h>.  

The following shall be declared as functions and may also be defined as macros. 
Function prototypes shall be provided for use with ISO C standard compilers.

  
void    *memccpy(void *restrict, const void *restrict, int, size_t);
 
void    *memchr(const void *, int, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void *restrict, const void *restrict, size_t);
void    *memmove(void *, const void *, size_t);
void    *memset(void *, int, size_t);
  
char    *stpcpy(char *restrict, const char *restrict);
char    *stpncpy(char *restrict, const char *restrict, size_t);
 
char    *strcat(char *restrict, const char *restrict);
char    *strchr(const char *, int);
int      strcmp(const char *, const char *);
int      strcoll(const char *, const char *);
  
int      strcoll_l(const char *, const char *, locale_t);
 
char    *strcpy(char *restrict, const char *restrict);
size_t   strcspn(const char *, const char *);
  
char    *strdup(const char *);
 
char    *strerror(int);
  
char    *strerror_l(int, locale_t);
int      strerror_r(int, char *, size_t);
 
size_t   strlen(const char *);
char    *strncat(char *restrict, const char *restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char *restrict, const char *restrict, size_t);
  
char    *strndup(const char *, size_t);
size_t   strnlen(const char *, size_t);
 
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
  
char    *strsignal(int);
 
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
  
char    *strtok_r(char *restrict, const char *restrict, char **restrict);
 
size_t   strxfrm(char *restrict, const char *restrict, size_t);
  
size_t   strxfrm_l(char *restrict, const char *restrict,
             size_t, locale_t);

*/

#endif // CRT_PROXY_LIB_INC_



