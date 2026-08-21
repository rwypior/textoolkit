#ifndef _h_textoolkit_dds
#define _h_textoolkit_dds

#include "common/image.hpp"
#include "common/pixel.hpp"
#include "common/result.hpp"

#include <gli/gli.hpp>
#include <gli/texture.hpp>
#include <gli/texture_cube.hpp>

#include <string>
#include <array>

namespace textoolkit
{
	class DDS;

	using DDSLoadResult = Result<DDS>;

	class DDS : public Image
	{
	public:
		DDS();
		DDS(const gli::texture& dds);
		DDS(gli::texture&& dds);

		virtual Type getType() const override;
		virtual TextureType getTextureType() const override;

		static DDSLoadResult load(std::istream& stream);
		static DDSLoadResult load(const std::string& path);
		static DDSLoadResult fromString(const std::string& data);

		bool save(std::ostream& stream) const override;

		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<unsigned char> getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual const void* getBytesPtr(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<Pixel> getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;
		virtual unsigned int getWidth(unsigned int level = 0) const override;
		virtual unsigned int getHeight(unsigned int level = 0) const override;
		virtual unsigned int getDepth(unsigned int level = 0) const override;
		virtual size_t getSize(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const override;
		virtual unsigned int getLayers() const override;
		virtual unsigned int getFaces() const override;
		virtual unsigned int getLevels() const override;
		virtual Swizzles getSwizzles() const override;
		virtual InfoMode getInfoMode() const override;
		virtual unsigned int getInternalFormat() const override;
		virtual unsigned int getFormat() const override;
		virtual unsigned int getDataType() const override;
		virtual bool isCompressed() const override;

		gli::texture& getTexture();

		static DDS createCubemap(const Cubemap& cubemap);
		static DDS create2D(unsigned int width, unsigned int height, const Pixel& baseColor);
		static DDS create2D(const Image& image);

	private:
		void updateInfo();
		std::optional<size_t> getIndex(unsigned int x, unsigned int y, unsigned int level) const;

		gli::gl::swizzles swizzles;
		gli::format originalForamt;
		gli::texture dds;
		unsigned int bytesPerPixel = 0;
	};
}

#endif