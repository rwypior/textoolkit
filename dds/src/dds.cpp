#include "dds/dds.hpp"

#include <gli/gli.hpp>
#include <gli/load_dds.hpp>
#include <gli/save_dds.hpp>
#include <gli/convert.hpp>
#include <gli/format.hpp>

#include <fstream>
#include <sstream>
#include <unordered_set>

namespace
{
	gli::texture::target_type translateTarget(textoolkit::Image::TextureType type)
	{
		switch (type)
		{
		case textoolkit::Image::TextureType::Texture2D:
			return gli::texture::target_type::TARGET_2D;
		case textoolkit::Image::TextureType::Texture3D:
			return gli::texture::target_type::TARGET_3D;
		case textoolkit::Image::TextureType::TextureCube:
			return gli::texture::target_type::TARGET_CUBE;
		case textoolkit::Image::TextureType::Texture2DArray:
			return gli::texture::target_type::TARGET_2D_ARRAY;
		case textoolkit::Image::TextureType::TextureCubeArray:
			return gli::texture::target_type::TARGET_CUBE_ARRAY;
		}

		assert(!"Invalid texture type");
		return gli::texture::target_type::TARGET_2D;
	}

	gli::texture::format_type translateFormat(textoolkit::Image::TextureInternalFormat format, textoolkit::Image::CompressionType compression)
	{
		switch (format)
		{
		case textoolkit::Image::TextureInternalFormat::Rgb8:
			switch (compression)
			{
			case textoolkit::Image::CompressionType::None:
				return gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8;
			case textoolkit::Image::CompressionType::DXT1:
				return gli::texture::format_type::FORMAT_RGBA_DXT1_SRGB_BLOCK8;
			case textoolkit::Image::CompressionType::DXT3:
				return gli::texture::format_type::FORMAT_RGBA_DXT3_SRGB_BLOCK16;
			case textoolkit::Image::CompressionType::DXT5:
				return gli::texture::format_type::FORMAT_RGBA_DXT5_SRGB_BLOCK16;
			}
		}

		assert(!"Invalid format/compression combination");
		return gli::texture::format_type::FORMAT_RGBA8_UNORM_PACK8;
	}

	unsigned int getFaces(textoolkit::Image::TextureType type)
	{
		return type == textoolkit::Image::TextureType::TextureCube ? 6 : 1;		
	}

	unsigned int getLevels(const glm::vec3& extents)
	{
		float smallerDimension = std::min(extents.x, extents.y);
		return static_cast<unsigned int>(std::log2(smallerDimension));
	}
}

namespace textoolkit
{
	// TODO:
	// Some data getting operations will fail because we're operating on bytes
	// and GLI may operate on other datatypes - need to fix this

	DDS::DDS() = default;

	DDS::DDS(const gli::texture& dds)
		: originalFormat(dds.format())
		, dds(gli::convert(dds, gli::format::FORMAT_RGBA8_UNORM_PACK8))
	{
		this->updateInfo();
	}

	DDS::DDS(
		TextureType type,
		TextureInternalFormat format,
		CompressionType compression,
		const glm::vec3& extents,
		unsigned int layers,
		bool generateMipmaps
	)
		: dds(
			translateTarget(type),
			translateFormat(format, compression),
			extents, layers, ::getFaces(type), generateMipmaps ? ::getLevels(extents) : 1
		)
		, originalFormat(translateFormat(format, compression))
	{
		this->updateInfo();
	}

	DDS::DDS(gli::texture&& dds)
		: originalFormat(dds.format())
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

	DDS::TextureType DDS::getTextureType() const
	{
		switch (this->dds.target())
		{
		case gli::target::TARGET_CUBE: return TextureType::TextureCube;
		}

		return TextureType::Texture2D;
	}

	bool DDS::save(std::ostream& stream) const
	{
		std::vector<char> buffer;
		gli::save_dds(this->dds, buffer);
		stream.write(buffer.data(), buffer.size());
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

	const void* DDS::getBytesPtr(unsigned int layer, unsigned int face, unsigned int level, DataOption /*mode*/) const
	{
		return this->dds.data(layer, face, level);
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
		auto blocksize = gli::block_size(this->dds.format());
		switch (blocksize)
		{
			case sizeof(glm::vec4):
				this->dds.store(gli::texture::extent_type(x, y, 0), layer, face, level, glm::vec4(pixel.r, pixel.g, pixel.b, pixel.a));
				break;
			case sizeof(glm::u8vec3):
				this->dds.store(gli::texture::extent_type(x, y, 0), layer, face, level, glm::u8vec3(pixel.r, pixel.g, pixel.b));
				break;
			case sizeof(glm::u8vec4):
				this->dds.store(gli::texture::extent_type(x, y, 0), layer, face, level, glm::u8vec4(pixel.r, pixel.g, pixel.b, pixel.a));
				break;
			default:
				return false;
		}

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

	unsigned int DDS::getDepth(unsigned int level) const
	{
		return this->dds.extent(level).z;
	}

	size_t DDS::getSize(unsigned int /*layer*/, unsigned int /*face*/, unsigned int level) const
	{
		return this->dds.size(level);
	}

	unsigned int DDS::getBytesPerPixel() const
	{
		return this->bytesPerPixel;
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

	DDS::Swizzles DDS::getSwizzles() const
	{
		gli::gl gl(gli::gl::PROFILE_GL33);
		auto format = gl.translate(this->dds.format(), this->dds.swizzles());
		return format.Swizzles;
	}

	DDS::InfoMode DDS::getInfoMode() const
	{
		return InfoMode::Opengl;
	}

	unsigned int DDS::getInternalFormat() const
	{
		gli::gl gl(gli::gl::PROFILE_GL33);
		auto format = gl.translate(this->dds.format(), this->dds.swizzles());
		return format.Internal;
	}

	unsigned int DDS::getFormat() const
	{
		gli::gl gl(gli::gl::PROFILE_GL33);
		auto format = gl.translate(this->dds.format(), this->dds.swizzles());
		return format.External;
	}

	unsigned int DDS::getDataType() const
	{
		gli::gl gl(gli::gl::PROFILE_GL33);
		auto format = gl.translate(this->dds.format(), this->dds.swizzles());
		return format.Type;
	}

	bool DDS::isCompressed() const
	{
		// This is copied from GLI. At this point GLI doesn't seem to have any mechamism of supplying this
		// kind of information, so keep watch for it.
		// TODO - create a pull request to GLI
		std::unordered_set<gli::gl::internal_format> compressedFormats{
			gli::gl::INTERNAL_RGB_DXT1,
			gli::gl::INTERNAL_RGBA_DXT1,
			gli::gl::INTERNAL_RGBA_DXT3,
			gli::gl::INTERNAL_RGBA_DXT5,
			gli::gl::INTERNAL_R_ATI1N_UNORM,
			gli::gl::INTERNAL_R_ATI1N_SNORM,
			gli::gl::INTERNAL_RG_ATI2N_UNORM,
			gli::gl::INTERNAL_RG_ATI2N_SNORM,
			gli::gl::INTERNAL_RGB_BP_UNSIGNED_FLOAT,
			gli::gl::INTERNAL_RGB_BP_SIGNED_FLOAT,
			gli::gl::INTERNAL_RGB_BP_UNORM,
			gli::gl::INTERNAL_RGB_PVRTC_4BPPV1,
			gli::gl::INTERNAL_RGB_PVRTC_2BPPV1,
			gli::gl::INTERNAL_RGBA_PVRTC_4BPPV1,
			gli::gl::INTERNAL_RGBA_PVRTC_2BPPV1,
			gli::gl::INTERNAL_RGBA_PVRTC_4BPPV2,
			gli::gl::INTERNAL_RGBA_PVRTC_2BPPV2,
			gli::gl::INTERNAL_ATC_RGB,
			gli::gl::INTERNAL_ATC_RGBA_EXPLICIT_ALPHA,
			gli::gl::INTERNAL_ATC_RGBA_INTERPOLATED_ALPHA,
			gli::gl::INTERNAL_RGB_ETC,
			gli::gl::INTERNAL_RGB_ETC2,
			gli::gl::INTERNAL_RGBA_PUNCHTHROUGH_ETC2,
			gli::gl::INTERNAL_RGBA_ETC2,
			gli::gl::INTERNAL_R11_EAC,
			gli::gl::INTERNAL_SIGNED_R11_EAC,
			gli::gl::INTERNAL_RG11_EAC,
			gli::gl::INTERNAL_SIGNED_RG11_EAC,
			gli::gl::INTERNAL_RGBA_ASTC_4x4,
			gli::gl::INTERNAL_RGBA_ASTC_5x4,
			gli::gl::INTERNAL_RGBA_ASTC_5x5,
			gli::gl::INTERNAL_RGBA_ASTC_6x5,
			gli::gl::INTERNAL_RGBA_ASTC_6x6,
			gli::gl::INTERNAL_RGBA_ASTC_8x5,
			gli::gl::INTERNAL_RGBA_ASTC_8x6,
			gli::gl::INTERNAL_RGBA_ASTC_8x8,
			gli::gl::INTERNAL_RGBA_ASTC_10x5,
			gli::gl::INTERNAL_RGBA_ASTC_10x6,
			gli::gl::INTERNAL_RGBA_ASTC_10x8,
			gli::gl::INTERNAL_RGBA_ASTC_10x10,
			gli::gl::INTERNAL_RGBA_ASTC_12x10,
			gli::gl::INTERNAL_RGBA_ASTC_12x12,
			gli::gl::INTERNAL_SRGB_DXT1,
			gli::gl::INTERNAL_SRGB_ALPHA_DXT1,
			gli::gl::INTERNAL_SRGB_ALPHA_DXT3,
			gli::gl::INTERNAL_SRGB_ALPHA_DXT5,
			gli::gl::INTERNAL_SRGB_BP_UNORM,
			gli::gl::INTERNAL_SRGB_PVRTC_2BPPV1,
			gli::gl::INTERNAL_SRGB_PVRTC_4BPPV1,
			gli::gl::INTERNAL_SRGB_ALPHA_PVRTC_2BPPV1,
			gli::gl::INTERNAL_SRGB_ALPHA_PVRTC_4BPPV1,
			gli::gl::INTERNAL_SRGB_ALPHA_PVRTC_2BPPV2,
			gli::gl::INTERNAL_SRGB_ALPHA_PVRTC_4BPPV2,
			gli::gl::INTERNAL_SRGB8_ETC2,
			gli::gl::INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ETC2_EAC,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_4x4,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_5x4,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_5x5,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_6x5,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_6x6,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_8x5,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_8x6,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_8x8,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_10x5,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_10x6,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_10x8,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_10x10,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_12x10,
			gli::gl::INTERNAL_SRGB8_ALPHA8_ASTC_12x12,
		};
		return compressedFormats.count(static_cast<gli::gl::internal_format>(this->getInternalFormat())) > 0;
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