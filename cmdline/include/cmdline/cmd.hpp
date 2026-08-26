#ifndef _h_textoolkit_cmdline_cmd
#define _h_textoolkit_cmdline_cmd

#include "common/result.hpp"

#include <string>

namespace textoolkit::cmdline
{
	using CmdResult = Result<std::string>;

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual CmdResult operator()() = 0;
	};
}

#endif