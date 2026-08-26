#ifndef _h_textoolkit_cmdline_info
#define _h_textoolkit_cmdline_info

#include "cmdline/cmd.hpp"

namespace textoolkit
{
	class Texture;

	namespace cmdline
	{
		enum class InfoProperty
		{
			Invalid,
			Summary,
			Type,
			Extents,
			Layers,
			Faces,
			Levels
		};

		InfoProperty translateProperty(const std::string& property);

		class Info : public Command
		{
		public:
			Info(const Texture& texture, const std::string& property);

			virtual CmdResult operator()() override;

		private:
			std::string get(InfoProperty property);

			const Texture& texture;
			InfoProperty property;
		};
	}
}

#endif