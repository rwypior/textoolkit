#ifndef _h_textoolkit_cmdline_save
#define _h_textoolkit_cmdline_save

#include "cmdline/cmd.hpp"

namespace textoolkit
{
	class Texture;

	namespace cmdline
	{
		class Save : public Command
		{
		public:
			Save(Texture& texture, const std::string& path);

			virtual CmdResult operator()() override;

		private:
			Texture& texture;
			std::string path;
		};
	}
}

#endif