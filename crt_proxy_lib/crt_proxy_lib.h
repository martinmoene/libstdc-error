#ifndef CRT_PROXY_LIB_INC_
#define CRT_PROXY_LIB_INC_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#define CRT_PROXY_LIB_VERSION "0.1.0"

#undef CRT_PROXY_LIB_NDEBUG
#ifndef _DEBUG
#define CRT_PROXY_LIB_NDEBUG
#endif // !_DEBUG

// C++ language version detection (C++20 is speculative):

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
#include "optional_bare.h"

namespace crt_proxy_lib 
{
    // any type used. nonstd bare CRT_PROXY_LIB permitting.
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
valstat< int >  strcmp(const char* lhs_, const char* rhs_);

} //  namespace crt_proxy_lib

#endif // CRT_PROXY_LIB_INC_
