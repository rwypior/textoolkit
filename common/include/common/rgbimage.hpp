#ifndef _h_common_rgbimage
#define _h_common_rgbimage

#include "common/pixel.hpp"
#include "common/image.hpp"

#include <memory>
#include <vector>
#include <optional>

namespace textoolkit
{
	/// Raw image representation
	class RgbImage : public Image
	{
	public:
		using DataContainer = std::vector<unsigned char>;
		using Level = DataContainer;
		using Face = std::vector<Level>;
		using Layer = std::vector<Face>;

		static constexpr unsigned int bytesPerPixel = 3;

	public:
		RgbImage(unsigned int baseWidth, unsigned int baseHeight, unsigned int layers = 1, unsigned int faces = 1, unsigned int levels = 1, const Pixel& baseColor = Pixel::white());

		virtual Type getType() const override;

		virtual bool save(std::ostream& stream) const override;

		void allocate(unsigned int baseWidth, unsigned int baseHeight, unsigned int layers = 1, unsigned int faces = 1, unsigned int levels = 1, const Pixel& baseColor = Pixel::white());
		/// Fill whole image with given color
		void fill(const Pixel& baseColor);

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

		const DataContainer* getData(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const;
		DataContainer* getData(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);
		size_t getSize(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const;
		size_t calculateSize(unsigned int level) const;
		std::optional<size_t> getIndex(unsigned int x, unsigned int y, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0, DataOption mode = DataOption::Normal) const;

	private:
		void fill(Level& level, const Pixel& baseColor);
		void fill(Face& face, const Pixel& baseColor);
		void fill(Layer& layer, const Pixel& baseColor);

		std::vector<Layer> layers;
		unsigned int baseWidth;
		unsigned int baseHeight;
	};
}

#endif