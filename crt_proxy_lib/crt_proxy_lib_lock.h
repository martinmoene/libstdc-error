#ifndef crt_proxy_lib_lock_h_
#define crt_proxy_lib_lock_h_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// user defined padlock type releases semaphore/mutex/critical section in destructor
// grabs in constructor
// crt_proxy_lib_lock.h, must define CRT_PROXY_LIB_PADLOCK

#undef  CRT_PROXY_LIB_PADLOCK
#define CRT_PROXY_LIB_PADLOCK  crt_proxy_lib_padlock crt_proxy_lib_padlock_instance__

struct crt_proxy_lib_padlock final 
{
    // this is one per instance of this type
	CRITICAL_SECTION   CS_ ;

	explicit crt_proxy_lib_padlock()
	{
		InitializeCriticalSection(&CS_);
		EnterCriticalSection(&CS_);
	}
	~crt_proxy_lib_padlock()
    {
		LeaveCriticalSection(&CS_);
		DeleteCriticalSection(&CS_);
	}
} ;

#endif // crt_proxy_lib_lock_h_