#ifndef _h_common_image
#define _h_common_image

#include "common/pixel.hpp"

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
		virtual ~Image() = default;

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