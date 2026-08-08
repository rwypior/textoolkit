#ifndef _h_textoolkit_common_util
#define _h_textoolkit_common_util

#include "common/meta.hpp"

#ifdef SYS_WINDOWS
#	define breakpoint() __debugbreak()
#else
#	define breakpoint() raise(SIGTRAP)
#endif

#endif