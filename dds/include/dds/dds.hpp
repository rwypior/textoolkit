#ifndef _h_textoolkit_dds
#define _h_textoolkit_dds

#include "common/image.hpp"
#include "common/pixel.hpp"

#include <gli/texture.hpp>
#include <gli/texture_cube.hpp>

#include <string>
#include <array>

namespace textoolkit
{
	namespace DDS
	{
		gli::texture_cube createCubemap(const Cubemap& cubemap);
		gli::texture2d create2D(unsigned int width, unsigned int height, const Pixel& baseColor);
		gli::texture2d create2D(const Image& image);
	};
}

#endif