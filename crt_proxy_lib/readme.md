
<h1>crt proxy lib</h1>

> Copyright &copy; 2019-2020, Dusan B. Jovanovic (dbj@dbj.org)
> 
> LICENSE DBJ -- https://dbj.org/license_dbj/ 


- [1. motivation](#1-motivation)
  - [1.1. The CL Kernel Mode](#11-the-cl-kernel-mode)
- [2. design](#2-design)
  - [2.1. returns handling is new error handling policy](#21-returns-handling-is-new-error-handling-policy)
  - [2.2. For Standard C++ users](#22-for-standard-c-users)
    - [2.2.1. True noexcept](#221-true-noexcept)
  - [2.3. Logging is important](#23-logging-is-important)
- [3. Implementation details](#3-implementation-details)
  - [3.1. Locking](#31-locking)
  - [3.2. Code structure details](#32-code-structure-details)
  - [3.3. benchmarking attitude](#33-benchmarking-attitude)
  - [3.4. Dependencies](#34-dependencies)

## 1. motivation

Circa 2020, C++ is very rarely, if ever, used for application programming. C++ is used for systems programming.

Who might use this library?  Teams wishing to code in standard C++ but who are under limitations, grouped as follows in two sets.

- Limitations of  restricted run-time requirements
  - specifically **not allowed to use** any of the following
    -  std lib
    -  throw/try/catch
    -  "special" return values 
    -  arguments for returns
    -  special globals
    -  magical constants

Very often heap allocation is added to the list above. These limitations often leave no choice but using C Run Time aka CRT. But, CRT has issues too.

- Limitation imposed by crt legacy issues. 
  - CRT legacy issues (non exhaustive list)
    1. errno based error handling aka using globals
       - example: pure function can not use globals 
    2. crashing on wrong input
       - example: null arguments 
    3. not crashing but returning wrong results
       - example: empty strings as arguments 

Thus all of the above are limiting the design decisions and directly shaping the implementation. 

### 1.1. The CL Kernel Mode

We might say this library is also for the people who use standard C++ **and** are building  using CL /kernel switch.
Here is the table from [further reading](https://docs.microsoft.com/en-us/cpp/build/reference/kernel-create-kernel-mode-binary?view=vs-2019) on-line:

| Behavior Type	| /kernel Behavior |
|---------------|------------------|
|C++ Exception Handling	|Disabled. All instances of the throw and try keywords emit a compiler error (except for the exception specification throw()). No /EH options are compatible with /kernel, except for /EH-.
|RTTI |	Disabled. All instances of the dynamic_cast and typeid keywords emit a compiler error, unless dynamic_cast is used statically.
| new and delete | You must explicitly define the new() or delete() operator; neither the compiler nor the runtime will supply a default definition.

NOTE: Sampling console app, part of this repository, is indeed built with the `/kernel` switch in use.

## 2. design

### 2.1. returns handling is new error handling policy

- no runtime exit, abort or crash
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

### 2.2. For Standard C++ users

- standard C++ (17 or better) core language
  - no classes
  - no inheritance
  - overloads are ok and are in use inside. Example:
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
  - `static_assert()`

#### 2.2.1. True noexcept

Issue with C++ and `noexcept` and LLVM and GCC is they implement some key CRT functions as throwing exceptions, like for example malloc.  

Known [way around that](https://compiler-explorer.com/z/rco9eh) is to cast to `noexcept` function pointer.

One using CL does not need that. But we know about it and we might use it if necessary.

### 2.3. Logging is important

Win console is just an win32 app with std streams created and attached.
C/C++ real life apps are not console apps. And (very likely) are not GUI apps
they are just server side apps. Invisible. 

Each data center or just simply server side component will have to have some logging used. Without logging in place admins can not see what is going on with your component. Neither can you.

Users of crt proxy lib need to deliver header named `crt_proxy_lib_log.h` with actual logging implementation. Macros to be defined are:
```cpp
// Same syntax as for the printf
CRT_PROXY_LIB_LOG_TRACE(...);
CRT_PROXY_LIB_LOG_DEBUG(...);
CRT_PROXY_LIB_LOG_INFO(...);
CRT_PROXY_LIB_LOG_WARN(...);
CRT_PROXY_LIB_LOG_ERROR(...);
CRT_PROXY_LIB_LOG_FATAL(...);
```
if this header is not defined `crt_proxy_lib_log_default.h` is used which 
defines macros that print to stderr.

```cpp
#if __has_include("crt_proxy_lib_log.h")
// user defied logging
#include "crt_proxy_lib_log.h"
#else
// default logging, to stderr
#include "crt_proxy_lib_log_default.h"
#endif 
```
## 3. Implementation details

### 3.1. Locking

This library users may provide resilience in the presence of threads. Either users provide lock/unlock type or there is no locking:
```cpp
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
```  
Therefore users need to provide `crt_proxy_lib_lock.h` . Each proxy function implementation start in this manner:
```cpp
// standard prologue example
valstat<size_t> strlen 
( const char * & input_ ) 
noexcept
{
     CRT_PROXY_LIB_PADLOCK;

    // ... the rest goes here ...
}
```
### 3.2. Code structure details

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
- **Which version of C99?**
- it is a "cat and mouse" game one has to play with CL.exe to find out which C99 features are enabled or not
  - that might change with each visual studio update.
  - even some C11 features are in
- we will not use deprecated crt function, whatever that means.
  - we will always try and use so called 'safe' versions
- we will [use the following](https://en.cppreference.com/w/c/experimental/dynamic) without knowing for sure if it has effect or not, while using CL.
```cpp
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
```

Of course users of this lib is protected also from the issue of every compile vendor having CRT `extension`, mixed with various levels of [POSIX](https://en.wikipedia.org/wiki/POSIX) conformance.

An illustrative citation from that link:

*Windows C Runtime Library and Windows Sockets API implement commonly used POSIX API functions for file, time, environment, and socket access, although the support remains largely incomplete and not fully interoperable with POSIX-compliant implementations...*

### 3.3. benchmarking attitude

- comparing code is artificial benchmarking
- comparing applications delivers true results for:
  - performance
  - size
- Windows Task Manager results are part of benchmarking results

### 3.4. Dependencies

Martin Moene's non [standard bare optional](https://github.com/martinmoene/optional-bare).
It allows us to have the optional, minus std lib, minus exceptions.

The limitations are the handled value is required to be copyable and default constructible.

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
