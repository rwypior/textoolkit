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
			TextureCube = 2
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

	public:
		virtual ~Image() = default;

		virtual Type getType() const = 0;
		virtual TextureType getTextureType() const = 0;

		virtual bool save(const std::string& path) const;
		virtual bool save(std::ostream& stream) const = 0;

		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, DataOption mode = DataOption::Normal) const
		{
			return getByte(x, y, 0, 0, 0, mode);
		}
		virtual std::vector<unsigned char> getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::vector<unsigned char> getBytes(DataOption mode = DataOption::Normal) const
		{
			return getBytes(0, 0, 0, mode);
		}
		virtual const void* getBytesPtr(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, DataOption mode = DataOption::Normal) const
		{
			return getPixel(x, y, 0, 0, 0, mode);
		}
		virtual std::vector<Pixel> getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const = 0;
		virtual std::vector<Pixel> getPixels(DataOption mode = DataOption::Normal) const
		{
			return getPixels(0, 0, 0, mode);
		}
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) = 0;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, DataOption mode = DataOption::Normal)
		{
			return setByte(byte, x, y, 0, 0, 0, mode);
		}
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) = 0;
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, DataOption mode = DataOption::Normal)
		{
			return setPixel(pixel, x, y, 0, 0, 0, mode);
		}

		virtual unsigned int getWidth(unsigned int level = 0) const = 0;
		virtual unsigned int getHeight(unsigned int level = 0) const = 0;
		virtual unsigned int getDepth(unsigned int level = 0) const { return 0; };
		virtual size_t getSize(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const = 0;
		virtual unsigned int getLayers() const
		{
			return 1;
		}
		virtual unsigned int getFaces() const
		{
			return 1;
		}
		virtual unsigned int getLevels() const
		{
			return 1;
		}
		virtual Swizzles getSwizzles() const
		{
			return {SwizzleRed, SwizzleGreen, SwizzleBlue, SwizzleOne};
		}
		virtual InfoMode getInfoMode() const
		{
			return InfoMode::Custom;
		}
		virtual unsigned int getInternalFormat() const
		{
			return static_cast<unsigned int>(TextureInternalFormat::Rgb8);
		}
		virtual unsigned int getFormat() const
		{
			return static_cast<unsigned int>(TextureFormat::Rgb);
		}
		virtual unsigned int getDataType() const
		{
			return static_cast<unsigned int>(DataType::UnsignedChar);
		}
		virtual bool isCompressed() const { return false; };
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
}

#endif