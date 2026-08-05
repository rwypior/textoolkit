#include "dds/dds.hpp"

// temporary
#include <gli/save_dds.hpp>
#include <numeric>

namespace textoolkit
{
	namespace DDS
	{
		gli::texture_cube createCubemap(const Cubemap& cubemap)
		{
			gli::texture_cube result;
			
			return result;
		}

		gli::texture2d create2D(unsigned int width, unsigned int height, const Pixel& baseColor)
		{
			gli::texture2d result(
				gli::texture::format_type::FORMAT_RGB8_UINT_PACK8,
				gli::texture2d::extent_type(width, height)
			);

			for (unsigned int y = 0; y < height; y++)
			{
				for (unsigned int x = 0; x < width; x++)
				{
					result.store(gli::texture2d::extent_type(x, y), 0, baseColor.toVec3<glm::u8vec3>());
				}
			}

			return result;
		}

		gli::texture2d create2D(const Image& image)
		{
			gli::texture2d result(
				gli::texture::format_type::FORMAT_RGB8_UINT_PACK8, 
				gli::texture2d::extent_type(image.getWidth(), image.getHeight())
			);
						
			for (unsigned int y = 0; y < image.getHeight(); y++)
			{
				for (unsigned int x = 0; x < image.getWidth(); x++)
				{
					auto pixel = image.getPixel(x, y);
					result.store(gli::texture2d::extent_type(x, y), 0, pixel->toVec3<glm::u8vec3>());
				}
			}

			return result;
		}
	}
}