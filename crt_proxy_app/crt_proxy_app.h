#pragma once

// #define _CRT_SECURE_NO_WARNINGS 1

#include "../crt_proxy_lib/crt_proxy_lib.h"

#ifdef _DEBUG
#define DBJ_LOG_TESTING
#endif

/* we want console while in debug builds */
#ifdef _DEBUG
#define DBJ_LOG_DEFAULT_SETUP DBJ_LOG_DEFAULT_WITH_CONSOLE
#endif

#include "dbj--simplelog/dbj_simple_log.h"


