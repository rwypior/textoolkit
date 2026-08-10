#include "common/image.hpp"
#include "common/result.hpp"
#include "common/pixel.hpp"

#include <string>
#include <vector>
#include <array>
#include <optional>
#include <ostream>
#include <istream>

namespace textoolkit
{
	class Bmp;

	using BmpLoadResult = Result<Bmp>;

	class Bmp : public Image
	{
	public:
		static constexpr char magic[] = "BM";
						
		enum class LoadMode
		{
			Normal,
			LoadPalette
		};

		enum class PixelStorage
		{
			Rgb = 0x01,
			Bgr = 0x02
		};

		enum class YStorage
		{
			TopBottom,
			BottomTop
		};

		struct BmpHeader
		{
			std::array<char, 2> magic{ 'B', 'M' };
			uint32_t fileSize;
			uint32_t reserved = 0;
			uint32_t dataOffset;

			size_t size() const;
			std::vector<unsigned char> serialize() const;
		};

		struct InfoHeader
		{
			uint32_t infoHeaderSize = size();
			uint32_t width;
			uint32_t height;
			uint16_t planes = 1;
			uint16_t bpp = 24;
			uint32_t compression = 0;
			uint32_t imageSize;
			uint32_t printResX = 11811;
			uint32_t printResY = 11811;
			uint32_t colorsUsed = 0;
			uint32_t colorsImportant = 0;
			std::vector<unsigned char> paletteBytes;

			size_t size() const;
			std::vector<unsigned char> serialize() const;
		};

	public:
		Bmp();
		Bmp(
			unsigned int width,
			unsigned int height,
			unsigned int bytesPerPixel = 3,
			PixelStorage pixelStorage = PixelStorage::Rgb,
			YStorage yStorage = YStorage::TopBottom,
			std::vector<unsigned char> paletteBytes = {}
		);
		Bmp(
			const std::vector<unsigned char>& bytes,
			unsigned int width,
			unsigned int height,
			unsigned int bytesPerPixel = 3,
			PixelStorage pixelStorage = PixelStorage::Rgb,
			YStorage yStorage = YStorage::TopBottom,
			std::vector<unsigned char> paletteBytes = {}
		);
		Bmp(
			const unsigned char* bytes,
			unsigned int width,
			unsigned int height,
			unsigned int bytesPerPixel = 3,
			PixelStorage pixelStorage = PixelStorage::Rgb,
			YStorage yStorage = YStorage::TopBottom,
			std::vector<unsigned char> paletteBytes = {}
		);
		Bmp(Bmp&& b) noexcept;
		Bmp(BmpLoadResult&& b) noexcept;
		Bmp& operator=(BmpLoadResult&& b) noexcept;

		virtual Type getType() const override;

		static BmpLoadResult load(std::istream& stream, LoadMode mode = LoadMode::LoadPalette);
		static BmpLoadResult load(const std::string& path, LoadMode mode = LoadMode::LoadPalette);
		static BmpLoadResult fromString(const std::string& data);

		void setData(const std::vector<unsigned char>& pixels, unsigned int width, unsigned int height, unsigned int bytesPerPixel = 3);
		bool save(std::ostream& stream) const override;

		virtual unsigned int getWidth(unsigned int level = 0) const override;
		virtual unsigned int getHeight(unsigned int level = 0) const override;

		virtual std::optional<unsigned char> getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<unsigned char> getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::optional<Pixel> getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual std::vector<Pixel> getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) const override;
		virtual bool setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;
		virtual bool setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode = DataOption::Normal) override;

		const std::vector<unsigned char>& getPalette() const;
		void setPalette(const std::vector<unsigned char>& paletteBytes);
		unsigned int getBytesPerPixel() const;

		unsigned int calculateDataSize(unsigned int width, unsigned int height, unsigned int bpp) const;

		/// Flip red and blue components in RGB values
		void flipRB();
		/// Flip image upside down
		void flipTopDown();

	private:
		void update(unsigned int width, unsigned int height, unsigned int bpp, const std::optional<std::vector<unsigned char>>& paletteBytes = {});
		void update();

		std::optional<size_t> getIndex(unsigned int x, unsigned int y, DataOption mode = DataOption::Normal) const;

		BmpHeader fileHeader;
		InfoHeader infoHeader;
		std::vector<unsigned char> bytes;
	};
}