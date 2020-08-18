
<h1>crt proxy lib</h1>

> Copyright &copy; 2019-2020, Dusan B. Jovanovic (dbj@dbj.org)

- [1. motivation](#1-motivation)
  - [1.1. run-time requirements](#11-run-time-requirements)
- [2. design](#2-design)
  - [2.1. returns handling is new error handling policy](#21-returns-handling-is-new-error-handling-policy)
  - [2.2. benchmarking policy](#22-benchmarking-policy)
- [3. implementation](#3-implementation)
  - [3.1. Dependencies](#31-dependencies)

## 1. motivation

- decoupling users from crt legacy issues
    - errno based error handling
    - crashing on wrong input
    - not crashing but returning wrong results, on bad input
- supporting standard C++ users labouring under strict run-time requirements

### 1.1. run-time requirements
- no std lib
- no throw/try/catch
- no "special" return values 
- no returns in arguments
- no special globals
- no magical constants

## 2. design

- standard C++ (17 or better) core language
  - no classes
  - no inheritance
  - overloads are ok and are inside
```cpp
template<size_t N>
constexpr inline size_t strlen ( const char (*str)[N] ) noexcept
{
    return N;
}

template<size_t N>
constexpr inline size_t strlen ( const char (&str)[N] ) noexcept
{
    return N;
}
```
- compile time assertions are used
- **logging** -- no direct console access
    -  user provided and defined
       -  logging function
       -  logging target 
-  resilience in the presence of threads
   -  user provided lock/unlock two functions

### 2.1. returns handling is new error handling policy

- null input, empty input or logically wrong input will provoke different responses
  - API consuming functions will have to capture that
- based on the metastate paradigm -- [P2192](https://gitlab.com/dbjdbj/valstat/-/blob/07ce13ab26f662c7301a463fee55dc21cbd7a585/P2192R2.md)
- no special return types
- no special return structures beside valstat definitions
```cpp
// valstat is not (yet) in the std namespace
namespace dbj {
    template <typename T_, typename S_ >
    struct valstat final {
        using type       = valstat ;
        using value_type = T_;
        using status_type = S_;

        value_type value ;
        status_type status ;
    };  // valstat 
}
```
- reminder
    - template given parameters is a definition
    - template definition is a type
    - template alias is still a template
- status returned is always `const char *` message
  - decoupling users from crt error handling
  - message is always already logged
- **future extension**: status returned is handle to the message logged
   -  that handle will be GUID inside a `const char *` string

### 2.2. benchmarking policy

- comparing code is artificial results
- comparing applications is true results
  - performance
  - size
- Windows Task Manager results are part of benchmarking results


## 3. implementation

- one header and one cpp file
   - just include and use
   - no lib or dll is built
   - recommendation: use as git sub-module
- Windows only but not using WIN32 API
- Visual Studio 2019 using both CL and CLANG
  - 64 bit builds only
- SEMVER 
  - starting from 0.0.1
  - 1.0.0 is first release
- top level namespace: `crt_proxy_lib`
- macros prefix: `CRT_PROXY_LIB_`
- user provides 
  - logging
  - locking
  - if no user provided **defaults** are used
    - no locking
    - stderr as a log target
- **Which version of C?**
- it is a "cat and mouse" game one has to play with CL.exe to find out which C features are enabled or not
  - that might change with each visual studio update.
- we will not use deprecated crt function, whatever that means.
  - we will always try and use 'safe' versions
- we will [use the following](https://en.cppreference.com/w/c/experimental/dynamic) without knowing for sure if it has effect or not, while using CL.
```cpp
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
```

### 3.1. Dependencies

1. Martin Moene's `nonstd` : `nonstd::optional`.
   It allows us to have the optional, minus std lib, minus exceptions.
```cpp
// nonstd optional
#define optional_CONFIG_SELECT_OPTIONAL optional_OPTIONAL_NONSTD
// no exceptions
#define optional_CONFIG_NO_EXCEPTIONS 1

#include <nonstd/optional.h>
namespace crt_proxy_lib 
{
    // any type used. nonstd::optional permitting.
    template <typename T_, typename S_ >
    using valstat = dbj::valstat<
        // value type
        nonstd::optional<T> ,
        // status type
        const char *
    > ;
}
```
Now we can readily ask the occupancy state; on both value and status. For any value type used. nonstd::optional permitting.
```cpp
// example
auto [ val, status_msg ] = crt_proxy_lib::atoi( "42" ) ;

// the occupancy of the val returned
// empty
if ( ! val )
  // not 'empty', type is const char *
  if ( status_msg )
     log ( status_msg ) ;
// not empty
if ( val ) 
     // use the value returned
     log ( *val ) ;    
```
