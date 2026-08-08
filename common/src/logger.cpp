#include "common/logger.hpp"

namespace textoolkit
{
	Logger& Logger::getLogger()
	{
		static Logger logger;
		return logger;
	}

	Logger& Logger::operator<<(const std::string& msg)
	{
		// TODO - implement this
		return *this;
	}
}