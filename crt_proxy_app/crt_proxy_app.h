#pragma once

// #define _CRT_SECURE_NO_WARNINGS 1

#include "../crt_proxy_lib/crt_proxy_lib.h"

//#ifdef _DEBUG
//#define DBJ_LOG_TESTING
//#endif

/* we want console while in debug builds */
//#ifdef _DEBUG
//#define DBJ_LOG_DEFAULT_SETUP DBJ_LOG_DEFAULT_WITH_CONSOLE
//#endif

#include "dbj--simplelog/dbj_simple_log.h"

#pragma region dbj pico testing

//#include <cstdio>
//#include <cstdlib>
#include <iostream>
//#include <vector>
//#include <array>
//#include <type_traits>
//#include <typeinfo>

// Show eXpression
// innocent loking but saves a lot of typing and rezults saving
#undef  SX
#define SX(x_) do { std::cout << "\n" #x_ " : " << (x_); } while(0)


namespace dbj::picotest {

#ifndef VT100_ESC

#define VT100_ESC "\x1b["
#define VT100_RESET VT100_ESC "0m"
#define VT100_LIGHT_GRAY VT100_ESC "90m"
#define VT100_LIGHT_BLUE VT100_ESC "94m"
#define VT100_LIGHT_CYAN VT100_ESC "36m"
#define VT100_LIGHT_YELLOW VT100_ESC "33m"
#define VT100_LIGHT_GREEN VT100_ESC "32m"
#define VT100_LIGHT_RED VT100_ESC "31m"
#define VT100_LIGHT_MAGENTA VT100_ESC "35m"

#endif // VT100_ESC

	// ERRR funy synbol name is because of 'ERROR' name clash with god knows what ...
	enum class levels { TRACE, DEBUG, INFO, WARN, ERRR, FATAL } ;


	static const char* level_colors[] = {
		/* TRACE */  VT100_LIGHT_BLUE,
		/* DEBUG */ VT100_LIGHT_CYAN,
		/* INFO */ VT100_LIGHT_GREEN,
		/* WARN */ VT100_LIGHT_YELLOW,
		/* ERRR */ VT100_LIGHT_RED,
		/* FATAL */ VT100_LIGHT_MAGENTA
	};


	using std::cout;

	namespace {
		inline auto initialize_once = []() { cout << std::boolalpha; return true; } ();
	}

	// return false on empty
	inline auto show_option = [](const char* prompt_, auto opt_) ->bool
	{
		cout << "\t" << prompt_ << "   ";
		if (opt_.has_value()) {
			cout << opt_.value();
			return true;
		}
		cout << "empty";
		return false;
	};

	inline auto show_status = [](const char* prompt_, const char* stat_) ->bool
	{
		cout << "\t" << prompt_ << "   ";
		if (stat_) {
			cout << stat_;
			return true;
		}
		cout << "empty";
		return false;
	};

	// not generic but for crt proxy lib tests where we know
	// status is always const char *
	inline auto show_metastate = [](const char* prompt_, auto val_, const char* stat_) {

		cout << "\n\n" << (prompt_ ? prompt_ : "");

		bool value_state = val_.has_value();
		bool status_state = stat_ != nullptr;

		cout << "\nmetastate: ";
		if (value_state && status_state) { cout << VT100_LIGHT_GREEN << "info"; }
		if (value_state && !status_state) { cout << "ok"; }
		if (!value_state && status_state) { cout << VT100_LIGHT_RED << "error"; }
		if (!value_state && !status_state) { cout << VT100_LIGHT_BLUE << "empty"; }
		cout << VT100_RESET ;
		show_option("Value : ", val_);
		show_status("Status: ", stat_);
		cout << "\n";


	};

	// for this to work we must send  
	// crt_proxy_lib::valstat instances
#undef metatest
#define metatest( CALL_ ) do { \
   auto [val_,stat_] = CALL_ ;\
       dbj::picotest::show_metastate(#CALL_, val_, stat_) ; \
} while(0)

} // dbj
#pragma endregion dbj pico testing


