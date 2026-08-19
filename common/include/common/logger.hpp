#ifndef _h_textoolkit_common_logger
#define _h_textoolkit_common_logger

#include "common/util.hpp"

#include <string>
#include <fstream>
#include <unordered_set>

#define LOGGER_UNIQUE_ID() __FILE__ STRINGIFY(__LINE__) __TIMESTAMP__

namespace textoolkit
{
	class Logger
	{
	public:
		/// Pass this to logger with unique id to only log the message once - decided by the ID provided
		struct Uniq
		{
			Uniq(const std::string& id)
				: id(id)
			{
			}

			std::string id;
		};

		class Session
		{
		public:
			Session(const std::string& path);
			Session(std::ofstream&& stream);
			~Session();

			Session operator<<(const std::string& msg);
			Session operator<<(const Uniq& uniq);

		private:
			std::ofstream stream;
		};

		friend class Session;

	public:
		static Logger& getLogger();
		static Logger& init(const std::string& logpath);

		Session operator<<(const std::string& msg);
		Session operator<<(const Uniq& uniq);

	private:
		std::string logPath;
		std::unordered_set<std::string> uniqueIds;
	};
}

#endif