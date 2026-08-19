#include "common/logger.hpp"

#include <fstream>

namespace textoolkit
{
	// Session

	Logger::Session::Session(const std::string& path)
		: stream(path, std::ios_base::out | std::ios_base::app)
	{
	}

	Logger::Session::Session(std::ofstream&& stream)
		: stream(std::move(stream))
	{
	}

	Logger::Session::~Session()
	{
		if (this->stream.is_open())
		{
			this->stream << "\n";
			this->stream.flush();
		}
	}

	Logger::Session Logger::Session::operator<<(const std::string& msg)
	{
		if (this->stream.is_open())
			this->stream << msg;
		return Session(std::move(this->stream));
	}

	Logger::Session Logger::Session::operator<<(const Uniq&)
	{
		return Session(std::move(this->stream));
	}

	// Logger

	Logger& Logger::getLogger()
	{
		static Logger logger;
		return logger;
	}

	Logger& Logger::init(const std::string& logpath)
	{
		Logger& logger = getLogger();
		logger.logPath = logpath;
		std::ofstream stream(logpath, std::ios_base::out); // Clear the file
		return logger;
	}

	Logger::Session Logger::operator<<(const std::string& msg)
	{
		return Session(this->logPath) << msg;
	}

	Logger::Session Logger::operator<<(const Logger::Uniq& uniq)
	{
		bool exists = this->uniqueIds.count(uniq.id) > 0;
		this->uniqueIds.insert(uniq.id);

		if (!exists)
			return Session(this->logPath) << uniq;
		return Session(std::ofstream());
	}
}