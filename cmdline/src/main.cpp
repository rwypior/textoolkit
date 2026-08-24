#include "cmdline/main.hpp"

#include <libcmdline/cmdline.h>

#include <string>
#include <iostream>

int main(int argc, char** argv)
{
	cmdline::Parser parser;
	parser.addArgument("path", "", cmdline::Req::required, "Path to the source texture");

	auto res = parser.parse(argc, argv);

	if (parser.getSwitch("help")->on())
	{
		std::cout << parser.getHelp() << std::endl;
		return 0;
	}

	if (!res)
	{
		std::cout << res.errorStr() << std::endl;
		return 1;
	}

	return 0;
}