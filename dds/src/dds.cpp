#include "dds/dds.hpp"

namespace textoolkit
{
	// TODO:
	// Some data getting operations will fail because we're operating on bytes
	// and GLI may operate on other datatypes - need to fix this

	DDS::DDS() = default;

	DDS::DDS(const gli::texture& dds)
		: dds(dds)
	{
	}

	DDS::DDS(gli::texture&& dds)
		: dds(std::move(dds))
	{
	}

	DDS::Type DDS::getType() const
	{
		return Type::DDS;
	}

	bool DDS::save(std::ostream& stream) const
	{
		// TODO - implement this
		return true;
	}

	std::optional<unsigned char> DDS::getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		const unsigned char* data = static_cast<const unsigned char*>(this->dds.data(layer, face, level));
		if (auto opt = getIndex(x, y, level))
		{
			unsigned int idx = *opt;
			return data[idx];
		}

		return {};
	}

	std::vector<unsigned char> DDS::getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		unsigned int size = this->dds.size(level);
		const unsigned char* data = static_cast<const unsigned char*>(this->dds.data(layer, face, level));
		return std::vector<unsigned char>(&data[0], &data[size]);
	}

	std::optional<Pixel> DDS::getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		const unsigned char* data = static_cast<const unsigned char*>(this->dds.data(layer, face, level));
		if (auto opt = getIndex(x, y, level))
		{
			unsigned int idx = *opt;
			Pixel pixel;
			pixel.r = data[0];
			pixel.g = data[0];
			pixel.b = data[0];
			return pixel;
		}

		return {};
	}

	std::vector<Pixel> DDS::getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		std::vector<Pixel> result;
		const unsigned char* data = static_cast<const unsigned char*>(this->dds.data(layer, face, level));
		for (unsigned int i = 0; i < this->dds.size(level); i++)
		{
			unsigned int byteidx = i % bytesPerPixel;

			if (byteidx == 0)
			{
				result.emplace_back();
			}

			Pixel& newpixel = result.back();
			newpixel[byteidx] = data[i];
		}
		return result;
	}

	bool DDS::setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode)
	{
		unsigned char* data = static_cast<unsigned char*>(this->dds.data(layer, face, level));
		if (auto opt = getIndex(x, y, level))
		{
			unsigned int idx = *opt;
			data[idx] = byte;
			return true;
		}
			
		return false;
	}

	bool DDS::setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode)
	{
		glm::vec3 texel(pixel.r, pixel.g, pixel.b);
		this->dds.store(gli::texture::extent_type(x, y, 0), layer, face, level, texel);
		return true;
	}

	unsigned int DDS::getWidth(unsigned int level) const
	{
		return this->dds.extent(level).x;
	}

	unsigned int DDS::getHeight(unsigned int level) const
	{
		return this->dds.extent(level).y;
	}

	unsigned int DDS::getLayers() const
	{
		return this->dds.layers();
	}

	unsigned int DDS::getFaces() const
	{
		return this->dds.faces();
	}

	unsigned int DDS::getLevels() const
	{
		return this->dds.levels();
	}

	gli::texture& DDS::getTexture()
	{
		return this->dds;
	}

	std::optional<size_t> DDS::getIndex(unsigned int x, unsigned int y, unsigned int level) const
	{
		auto extent = this->dds.extent(level);
		unsigned int idx = x * bytesPerPixel + y * extent.x * bytesPerPixel;
		return idx;
	}

	DDS DDS::createCubemap(const Cubemap& cubemap)
	{
		gli::texture_cube result;
			
		return DDS(std::move(result));
	}

	DDS DDS::create2D(unsigned int width, unsigned int height, const Pixel& baseColor)
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

		return DDS(std::move(result));
	}

	DDS DDS::create2D(const Image& image)
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

		return DDS(std::move(result));
	}
}