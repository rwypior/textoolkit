#include "common/image.hpp"
#include "common/util.hpp"

#include <fstream>
#include <algorithm>
#include <string>

namespace textoolkit
{
	DataOption pickDataOption(Image::StorageMode storageA, Image::StorageMode storageB)
	{
		return storageA == storageB ? DataOption::Normal : DataOption::InvertY;
	}

	Image::TextureType Image::translateTextureType(const std::string& name)
	{
		if (name == "Texture2D")
			return TextureType::Texture2D;
		if (name == "Texture3D")
			return TextureType::Texture3D;
		if (name == "TextureCube")
			return TextureType::TextureCube;

		assert(!"Invalid texture type");
		return TextureType::Invalid;
	}

	std::string Image::translateTextureType(TextureType type)
	{
		switch (type)
		{
		case TextureType::Texture2D: return "Texture2D";
		case TextureType::Texture3D: return "Texture3D";
		case TextureType::TextureCube: return "TextureCube";
		}

		return "[Invalid]";
	}

	Image::TextureInternalFormat Image::translateInternalFormat(const std::string& name)
	{
		std::string n = trimmed(name);
		std::transform(n.begin(), n.end(), n.begin(), [](char c) { return std::tolower(c); });
		if (n == "rgb8")
			return Image::TextureInternalFormat::Rgb8;
		return Image::TextureInternalFormat::Invalid;
	}

	std::string Image::translateInternalFormat(TextureInternalFormat type)
	{
		switch (type)
		{
		case TextureInternalFormat::Rgb8: return "RGB8";
		}

		return "[Invalid]";
	}

	std::map<std::string, Image::TextureInternalFormat> Image::getInternalFormatMap()
	{
		return {
			{ "RGB8", TextureInternalFormat::Rgb8 }
		};
	}

	Image::CompressionType Image::translateCompression(const std::string& name)
	{
		std::string n = trimmed(name);
		std::transform(n.begin(), n.end(), n.begin(), [](char c) { return std::tolower(c); });
		if (n == "none" || n.empty())
			return Image::CompressionType::None;
		if (n == "dxt1")
			return Image::CompressionType::DXT1;
		if (n == "dxt3")
			return Image::CompressionType::DXT3;
		if (n == "dxt5")
			return Image::CompressionType::DXT5;
		return Image::CompressionType::Invalid;
	}

	std::string Image::translateCompression(Image::CompressionType type)
	{
		switch (type)
		{
		case CompressionType::None: return "None";
		case CompressionType::DXT1: return "DXT1";
		case CompressionType::DXT3: return "DXT3";
		case CompressionType::DXT5: return "DXT5";
		}

		return "[Invalid]";
	}

	std::map<std::string, Image::CompressionType> Image::getCompressionMap()
	{
		return {
			{ "None", CompressionType::None },
			{ "DXT1", CompressionType::DXT1 },
			{ "DXT3", CompressionType::DXT3 },
			{ "DXT5", CompressionType::DXT5 }
		};
	}

	Image::StorageMode Image::getStorageMode() const
	{
		return StorageMode::TopBottom;
	}

	bool Image::save(const std::string& path) const
	{
		std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);

		if (ofs.is_open())
		{
			return this->save(ofs);
		}

		return false;
	}

	std::optional<unsigned char> Image::getByte(unsigned int x, unsigned int y, DataOption mode) const
	{
		return getByte(x, y, 0, 0, 0, mode);
	}

	std::vector<unsigned char> Image::getBytes(DataOption mode) const
	{
		return getBytes(0, 0, 0, mode);
	}

	std::optional<Pixel> Image::getPixel(unsigned int x, unsigned int y, DataOption mode) const
	{
		return getPixel(x, y, 0, 0, 0, mode);
	}

	std::vector<Pixel> Image::getPixels(DataOption mode) const
	{
		return getPixels(0, 0, 0, mode);
	}

	bool Image::setByte(unsigned char byte, unsigned int x, unsigned int y, DataOption mode)
	{
		return setByte(byte, x, y, 0, 0, 0, mode);
	}

	bool Image::setPixel(Pixel pixel, unsigned int x, unsigned int y, DataOption mode)
	{
		return setPixel(pixel, x, y, 0, 0, 0, mode);
	}

	unsigned int Image::getLayers() const
	{
		return 1;
	}

	unsigned int Image::getFaces() const
	{
		return 1;
	}

	unsigned int Image::getLevels() const
	{
		return 1;
	}

	Image::Swizzles Image::getSwizzles() const
	{
		return { SwizzleRed, SwizzleGreen, SwizzleBlue, SwizzleOne };
	}

	Image::InfoMode Image::getInfoMode() const
	{
		return InfoMode::Custom;
	}

	unsigned int Image::getInternalFormat() const
	{
		return static_cast<unsigned int>(TextureInternalFormat::Rgb8);
	}

	unsigned int Image::getFormat() const
	{
		return static_cast<unsigned int>(TextureFormat::Rgb);
	}

	unsigned int Image::getDataType() const
	{
		return static_cast<unsigned int>(DataType::UnsignedChar);
	}

	bool Image::isCompressed() const
	{
		return false;
	}
}