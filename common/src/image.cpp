#include "common/image.hpp"

#include <fstream>

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
		return TextureType::Texture2D;
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