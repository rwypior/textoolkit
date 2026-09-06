#ifndef _h_textoolkit_cmdline_import
#define _h_textoolkit_cmdline_import

#include "cmdline/cmd.hpp"
#include "texture/texture.hpp"

namespace textoolkit
{
	class Texture;

	namespace cmdline
	{
		class Import : public Command
		{
		public:
			Import(Texture& texture, const std::string& path, unsigned int layer, unsigned int face, unsigned int level, InterpolationMinMag interpolation);

			virtual CmdResult operator()() override;

		private:
			Texture& texture;
			std::string path;
			unsigned int layer;
			unsigned int face;
			unsigned int level;
			InterpolationMinMag interpolation;
		};
	}
}

#endif