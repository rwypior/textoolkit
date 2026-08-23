#ifndef _h_common_image
#define _h_common_image

#include "common/pixel.hpp"

#include <glm/vec4.hpp>

#include <string>
#include <memory>
#include <vector>
#include <optional>

namespace textoolkit
{
	enum class DataOption
	{
		Normal,
		InvertY
	};

	class Image
	{
	public:
		using Swizzles = glm::ivec4;
		static constexpr int SwizzleRed = 0x1903;
		static constexpr int SwizzleGreen = 0x1904;
		static constexpr int SwizzleBlue = 0x1905;
		static constexpr int SwizzleAlpha = 0x1906;
		static constexpr int SwizzleOne = 0x0001;
		static constexpr int SwizzleZero = 0x0000;

		enum class Type
		{
			RgbImage,
			BMP,
			DDS
		};

		enum class TextureType
		{
			Texture2D = 1,
			Texture3D = 2,
			TextureCube = 3,
			Texture2DArray = 4,
			TextureCubeArray = 5
		};

		enum class CompressionType
		{
			None,
			DXT1,
			DXT3,
			DXT5
		};

		/// Specify how texture information enums are returned and if they need to be translated
		/// This is related to TextureInternalFormat, TextureFormat and DataType enums
		enum class InfoMode
		{
			Opengl, // Returned information is already OpenGL constants
			Custom // Returned information is returned as internal enums and need to be translated
		};

		enum class TextureInternalFormat
		{
			Rgb8
		};
		
		enum class TextureFormat
		{
			Rgb
		};

		enum class DataType
		{
			UnsignedChar
		};

		/// Specify how pixels are stored in the texture
		enum class StorageMode
		{
			TopBottom, // 0, 0 coordinate will point to top left pixel on the texture
			BottomTop // 0, 0 coordinate will point to bottom left pixel on the texture
		};

	public:
		virtual ~Image() = default;

		static TextureType translateTextureType(const std::string& name);

		virtual Type getType() const = 0;
		virtual TextureType getTextureType() const = 0;
		virtual StorageMode getStorageMode() const;

		virtual bool save(const std::string& path) const;
		virtual bool save(std::ostream& stream) const = 0;

		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, DataOption mode = DataOption::Normal) const;
		virtual std::vector<unsigned char> getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::vector<unsigned char> getBytes(DataOption mode = DataOption::Normal) const;
		virtual const void* getBytesPtr(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, DataOption mode = DataOption::Normal) const;
		virtual std::vector<Pixel> getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::vector<Pixel> getPixels(DataOption mode = DataOption::Normal) const;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) = 0;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, DataOption mode = DataOption::Normal);
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) = 0;
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, DataOption mode = DataOption::Normal);

		virtual unsigned int getWidth(unsigned int level = 0) const = 0;
		virtual unsigned int getHeight(unsigned int level = 0) const = 0;
		virtual unsigned int getDepth(unsigned int level = 0) const { return 0; };
		virtual size_t getSize(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const = 0;
		virtual unsigned int getBytesPerPixel() const = 0;
		virtual unsigned int getLayers() const;
		virtual unsigned int getFaces() const;
		virtual unsigned int getLevels() const;
		virtual Swizzles getSwizzles() const;
		virtual InfoMode getInfoMode() const;
		virtual unsigned int getInternalFormat() const;
		virtual unsigned int getFormat() const;
		virtual unsigned int getDataType() const;
		virtual bool isCompressed() const;
	};

	struct Cubemap
	{
		std::shared_ptr<Image> top;
		std::shared_ptr<Image> bottom;
		std::shared_ptr<Image> front;
		std::shared_ptr<Image> back;
		std::shared_ptr<Image> left;
		std::shared_ptr<Image> right;
	};

	DataOption pickDataOption(Image::StorageMode storageA, Image::StorageMode storageB);
}

#endif