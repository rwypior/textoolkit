#ifndef _h_textoolkit_common_logger
#define _h_textoolkit_common_logger

#include <string>

namespace textoolkit
{
	class Logger
	{
	public:
		static Logger& getLogger();

		Logger& operator<<(const std::string& msg);
	};
}

#endif