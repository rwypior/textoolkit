#include "dds/dds.hpp"

#include <gli/gli.hpp>
#include <gli/load_dds.hpp>
#include <gli/convert.hpp>
#include <gli/format.hpp>

#include <fstream>
#include <sstream>

namespace textoolkit
{
	// TODO:
	// Some data getting operations will fail because we're operating on bytes
	// and GLI may operate on other datatypes - need to fix this

	DDS::DDS() = default;

	DDS::DDS(const gli::texture& dds)
		: originalForamt(dds.format())
		, dds(gli::convert(dds, gli::format::FORMAT_RGBA8_UNORM_PACK8))
	{
		this->updateInfo();
	}

	DDS::DDS(gli::texture&& dds)
		: originalForamt(dds.format())
		, dds(gli::convert(std::move(dds), gli::format::FORMAT_RGBA8_UNORM_PACK8))
	{
		this->updateInfo();
	}

	void DDS::updateInfo()
	{
		static gli::gl gl = gli::gl(gli::gl::profile::PROFILE_GL33);
		auto format = gl.translate(this->dds.format(), this->dds.swizzles());
		this->swizzles = format.Swizzles;
		auto formatInfo = gli::detail::get_format_info(this->dds.format());
		this->bytesPerPixel = formatInfo.BlockSize;
	}

	DDSLoadResult DDS::load(std::istream& stream)
	{
		stream.seekg(0, std::ios_base::end);
		unsigned int size = stream.tellg();
		stream.seekg(0, std::ios_base::beg);

		std::string data(size, 0);
		stream.read(&data[0], size);

		auto dds = gli::load_dds(&data[0], size);
		return DDS(std::move(dds));
	}

	DDSLoadResult DDS::load(const std::string& path)
	{
		std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
		if (!stream.is_open())
			return DDSLoadResult::Code::FileInaccessible;
		return DDS::load(stream);
	}

	DDSLoadResult DDS::fromString(const std::string& data)
	{
		std::stringstream stream(data, std::ios_base::in | std::ios_base::binary);
		return DDS::load(stream);
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
			pixel.r = data[idx + 0];
			pixel.g = data[idx + 1];
			pixel.b = data[idx + 2];
			pixel.a = this->swizzles[3] == 1 ? 255 : data[3];
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
				auto zxc = pixel->toVec3<glm::u8vec3>();

				result.store(gli::texture2d::extent_type(x, y), 0, pixel->toVec3<glm::u8vec3>());
			}
		}

		return DDS(std::move(result));
	}
}