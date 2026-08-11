#ifndef _h_textoolkit_common_util
#define _h_textoolkit_common_util

#include "common/textoolkit.hpp"

#ifdef TEXTOOLKIT_WINDOWS
#	define breakpoint() __debugbreak()
#else
#	define breakpoint() raise(SIGTRAP)
#endif

#define STRINGIFY_IMPL(WHAT) #WHAT
#define STRINGIFY(WHAT) STRINGIFY_IMPL(WHAT)

#endif