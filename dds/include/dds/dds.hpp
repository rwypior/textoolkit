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
	class DDS : public Image
	{
	public:
		static constexpr unsigned int bytesPerPixel = 3;

	public:
		DDS();
		DDS(const gli::texture& dds);
		DDS(gli::texture&& dds);

		virtual Type getType() const override;

		bool save(std::ostream& stream) const override;

		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<unsigned char> getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<Pixel> getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;
		virtual unsigned int getWidth(unsigned int level = 0) const override;
		virtual unsigned int getHeight(unsigned int level = 0) const override;
		virtual unsigned int getLayers() const override;
		virtual unsigned int getFaces() const override;
		virtual unsigned int getLevels() const override;

		gli::texture& getTexture();

		static DDS createCubemap(const Cubemap& cubemap);
		static DDS create2D(unsigned int width, unsigned int height, const Pixel& baseColor);
		static DDS create2D(const Image& image);

	private:
		std::optional<size_t> getIndex(unsigned int x, unsigned int y, unsigned int level) const;

		gli::texture dds;
	};
}

#endif