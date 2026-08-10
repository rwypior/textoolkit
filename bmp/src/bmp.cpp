#include "bmp/bmp.hpp"
#include "common/pixel.hpp"

#include <fstream>
#include <sstream>
#include <cassert>

namespace textoolkit
{
	inline std::vector<unsigned char> byteVector(const unsigned char* bytes, unsigned int width, unsigned int height, unsigned int bytesPerPixel)
	{
		unsigned int size = width * height * bytesPerPixel;
		return std::vector<unsigned char>(bytes, bytes + size);
	}

	// Bmp header

	size_t Bmp::BmpHeader::size() const
	{
		return
			sizeof(Bmp::BmpHeader::magic) +
			sizeof(Bmp::BmpHeader::fileSize) +
			sizeof(Bmp::BmpHeader::reserved) +
			sizeof(Bmp::BmpHeader::dataOffset);
	}

	std::vector<unsigned char> Bmp::BmpHeader::serialize() const
	{
		return std::vector<unsigned char>({
			'B','M',
			static_cast<unsigned char>(fileSize),
			static_cast<unsigned char>(fileSize >> 8),
			static_cast<unsigned char>(fileSize >> 16),
			static_cast<unsigned char>(fileSize >> 24),
			0,0,0,0, /// reserved
			static_cast<unsigned char>(dataOffset),
			static_cast<unsigned char>(dataOffset >> 8),
			static_cast<unsigned char>(dataOffset >> 16),
			static_cast<unsigned char>(dataOffset >> 24),
			});
	}

	// Info header

	size_t Bmp::InfoHeader::size() const
	{
		return
			sizeof(Bmp::InfoHeader::infoHeaderSize) +
			sizeof(Bmp::InfoHeader::width) +
			sizeof(Bmp::InfoHeader::height) +
			sizeof(Bmp::InfoHeader::planes) +
			sizeof(Bmp::InfoHeader::bpp) +
			sizeof(Bmp::InfoHeader::compression) +
			sizeof(Bmp::InfoHeader::imageSize) +
			sizeof(Bmp::InfoHeader::printResX) +
			sizeof(Bmp::InfoHeader::printResY) +
			sizeof(Bmp::InfoHeader::colorsUsed) +
			sizeof(Bmp::InfoHeader::colorsImportant) +
			this->paletteBytes.size();
	}

	std::vector<unsigned char> Bmp::InfoHeader::serialize() const
	{
		auto data = std::vector<unsigned char>({
			static_cast<unsigned char>(this->infoHeaderSize),
			static_cast<unsigned char>(this->infoHeaderSize >> 8),
			static_cast<unsigned char>(this->infoHeaderSize >> 16),
			static_cast<unsigned char>(this->infoHeaderSize >> 24),
			static_cast<unsigned char>(this->width),
			static_cast<unsigned char>(this->width >> 8),
			static_cast<unsigned char>(this->width >> 16),
			static_cast<unsigned char>(this->width >> 24),
			static_cast<unsigned char>(this->height),
			static_cast<unsigned char>(this->height >> 8),
			static_cast<unsigned char>(this->height >> 16),
			static_cast<unsigned char>(this->height >> 24),
			static_cast<unsigned char>(planes),
			static_cast<unsigned char>(planes >> 8),
			static_cast<unsigned char>(bpp),
			static_cast<unsigned char>(bpp >> 8),
			static_cast<unsigned char>(compression),
			static_cast<unsigned char>(compression >> 8),
			static_cast<unsigned char>(compression >> 16),
			static_cast<unsigned char>(compression >> 24),
			static_cast<unsigned char>(imageSize),
			static_cast<unsigned char>(imageSize >> 8),
			static_cast<unsigned char>(imageSize >> 16),
			static_cast<unsigned char>(imageSize >> 24),
			static_cast<unsigned char>(printResX),
			static_cast<unsigned char>(printResX >> 8),
			static_cast<unsigned char>(printResX >> 16),
			static_cast<unsigned char>(printResX >> 24),
			static_cast<unsigned char>(printResY),
			static_cast<unsigned char>(printResY >> 8),
			static_cast<unsigned char>(printResY >> 16),
			static_cast<unsigned char>(printResY >> 24),
			static_cast<unsigned char>(colorsUsed),
			static_cast<unsigned char>(colorsUsed >> 8),
			static_cast<unsigned char>(colorsUsed >> 16),
			static_cast<unsigned char>(colorsUsed >> 24),
			static_cast<unsigned char>(colorsImportant),
			static_cast<unsigned char>(colorsImportant >> 8),
			static_cast<unsigned char>(colorsImportant >> 16),
			static_cast<unsigned char>(colorsImportant >> 24),
			});

		return data;
	}

	// Bmp

	Bmp::Bmp() = default;

	Bmp::Bmp(
		unsigned int width,
		unsigned int height,
		unsigned int bytesPerPixel,
		PixelStorage pixelStorage,
		YStorage yStorage,
		std::vector<unsigned char> paletteBytes
	)
	{
		this->bytes.resize(width * height * bytesPerPixel, 0);
		this->update(width, height, bytesPerPixel * 8, paletteBytes);
	}

	Bmp::Bmp(
		const std::vector<unsigned char>& bytes,
		unsigned int width,
		unsigned int height,
		unsigned int bytesPerPixel,
		PixelStorage pixelStorage,
		YStorage yStorage,
		std::vector<unsigned char> paletteBytes
	)
		: bytes(bytes)
	{
		this->update(width, height, bytesPerPixel * 8, paletteBytes);

		if (pixelStorage == PixelStorage::Rgb)
			this->flipRB();

		if (yStorage == YStorage::TopBottom)
			this->flipTopDown();
	}

	Bmp::Bmp(
		const unsigned char* bytes,
		unsigned int width,
		unsigned int height,
		unsigned int bytesPerPixel,
		PixelStorage pixelStorage,
		YStorage yStorage,
		std::vector<unsigned char> paletteBytes
	)
		: Bmp(byteVector(bytes, width, height, bytesPerPixel), width, height, bytesPerPixel, pixelStorage, yStorage, paletteBytes)
	{
	}

	Bmp::Bmp(Bmp&& b) noexcept
		: fileHeader(std::move(b.fileHeader))
		, infoHeader(std::move(b.infoHeader))
		, bytes(std::move(b.bytes))
	{
		this->update();
	}

	Bmp::Bmp(BmpLoadResult&& b) noexcept
		: Bmp(std::move(*b.data))
	{
	}

	Bmp& Bmp::operator=(BmpLoadResult&& b) noexcept
	{
		this->fileHeader = b.data->fileHeader;
		this->infoHeader = b.data->infoHeader;
		this->bytes = std::move(b.data->bytes);
		this->update();
		return *this;
	}

	Bmp::Type Bmp::getType() const
	{
		return Type::BMP;
	}

	BmpLoadResult Bmp::load(std::istream& stream, LoadMode mode)
	{
		Bmp bmp;

		stream.read(&bmp.fileHeader.magic[0], bmp.fileHeader.magic.size());

		if (bmp.fileHeader.magic.at(0) != magic[0] || bmp.fileHeader.magic.at(1) != magic[1])
			return BmpLoadResult::Code::InvalidMagic;

		stream.read(reinterpret_cast<char*>(&bmp.fileHeader.fileSize), 4);
		stream.read(reinterpret_cast<char*>(&bmp.fileHeader.reserved), 4);
		stream.read(reinterpret_cast<char*>(&bmp.fileHeader.dataOffset), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.infoHeaderSize), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.width), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.height), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.planes), 2);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.bpp), 2);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.compression), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.imageSize), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.printResX), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.printResY), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.colorsUsed), 4);
		stream.read(reinterpret_cast<char*>(&bmp.infoHeader.colorsImportant), 4);

		if (mode == LoadMode::LoadPalette)
		{
			std::vector<unsigned char> paletteBytes;

			unsigned int pos = stream.tellg();
			unsigned int paletteSize = bmp.fileHeader.dataOffset - pos;
			paletteBytes.reserve(paletteSize);
			for (; pos < bmp.fileHeader.dataOffset; pos++)
			{
				unsigned char byte;
				stream.read(reinterpret_cast<char*>(&byte), 1);
				paletteBytes.push_back(byte);
			}

			bmp.setPalette(paletteBytes);
		}

		unsigned int byteCount = bmp.getWidth() * bmp.getHeight() * bmp.getBytesPerPixel();
		bmp.bytes.resize(byteCount, '\0');

		stream.seekg(bmp.fileHeader.dataOffset, std::ios_base::beg);
		stream.read(reinterpret_cast<char*>(&bmp.bytes[0]), byteCount);

		return bmp;
	}

	BmpLoadResult Bmp::load(const std::string& path, LoadMode mode)
	{
		std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
		if (!stream.is_open())
			return BmpLoadResult::Code::FileInaccessible;
		return Bmp::load(stream);
	}

	BmpLoadResult Bmp::fromString(const std::string& data)
	{
		std::stringstream stream(data, std::ios_base::in | std::ios_base::binary);
		return Bmp::load(stream);
	}

	void Bmp::setData(const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int bytesPerPixel)
	{
		this->bytes = bytes;
		this->update(width, height, bytesPerPixel);
	}

	bool Bmp::save(std::ostream& stream) const
	{
		auto fileHeaderBytes = this->fileHeader.serialize();
		auto infoHeaderBytes = this->infoHeader.serialize();

		stream.write(reinterpret_cast<char*>(fileHeaderBytes.data()), fileHeaderBytes.size());
		stream.write(reinterpret_cast<char*>(infoHeaderBytes.data()), infoHeaderBytes.size());

		for (unsigned char byte : this->getPalette())
		{
			stream.write(reinterpret_cast<char*>(&byte), 1);
		}

		for (unsigned int y = 0; y < this->getHeight(); y++)
		{
			for (unsigned int x = 0; x < this->getWidth(); x++)
			{
				auto pixel = this->Image::getPixel(x, y);
				stream << pixel->b;
				stream << pixel->g;
				stream << pixel->r;
			}
		}

		return true;
	}

	unsigned int Bmp::getWidth(unsigned int level) const
	{
		return this->infoHeader.width;
	}

	unsigned int Bmp::getHeight(unsigned int level) const
	{
		return this->infoHeader.height;
	}

	std::optional<unsigned char> Bmp::getByte(unsigned int x, unsigned int y, unsigned int /*layer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode) const
	{
		auto idx = this->getIndex(x, y, mode);
		if (!idx)
			return {};

		return this->bytes.at(*idx);
	}

	std::vector<unsigned char> Bmp::getBytes(unsigned int /*layer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode) const
	{
		return this->bytes;
	}

	std::optional<Pixel> Bmp::getPixel(unsigned int x, unsigned int y, unsigned int /*layer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode) const
	{
		auto idx = this->getIndex(x, y, mode);
		if (!idx)
			return {};

		Pixel pixel;
		pixel.r = this->bytes.at(*idx + 2);
		pixel.g = this->bytes.at(*idx + 1);
		pixel.b = this->bytes.at(*idx + 0);
		pixel.a = 255;

		return pixel;
	}
		
	std::vector<Pixel> Bmp::getPixels(unsigned int /*layer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode) const
	{
		std::vector<Pixel> result;
		for (unsigned int i = 0; i < this->bytes.size(); i++)
		{
			unsigned int byteidx = i % this->getBytesPerPixel();

			if (byteidx == 0)
			{
				result.emplace_back();
			}

			Pixel& newpixel = result.back();
			newpixel[byteidx] = this->bytes[i];
		}
		return result;
	}

	bool Bmp::setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int /*layer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode)
	{
		auto idx = this->getIndex(x, y, mode);
		if (!idx)
			return false;

		this->bytes.at(*idx) = byte;

		return true;
	}

	bool Bmp::setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int /*slayer*/, unsigned int /*face*/, unsigned int /*level*/, DataOption mode)
	{
		auto idx = this->getIndex(x, y, mode);
		if (!idx || *idx + 2 < this->bytes.size())
			return false;

		this->bytes.at(*idx + 0) = pixel.r;
		this->bytes.at(*idx + 1) = pixel.g;
		this->bytes.at(*idx + 2) = pixel.b;

		return true;
	}

	const std::vector<unsigned char>& Bmp::getPalette() const
	{
		return this->infoHeader.paletteBytes;
	}

	void Bmp::setPalette(const std::vector<unsigned char>& paletteBytes)
	{
		this->infoHeader.paletteBytes = paletteBytes;
		this->update();
	}

	unsigned int Bmp::getBytesPerPixel() const
	{
		return this->infoHeader.bpp / 8;
	}

	unsigned int Bmp::calculateDataSize(unsigned int width, unsigned int height, unsigned int bpp) const
	{
		return width * height * bpp / 8;
	}

	void Bmp::update(unsigned int width, unsigned int height, unsigned int bpp, const std::optional<std::vector<unsigned char>>& paletteBytes)
	{
		if (paletteBytes)
			this->infoHeader.paletteBytes = *paletteBytes;

		this->fileHeader.fileSize = this->fileHeader.size() + this->infoHeader.size() + this->calculateDataSize(width, height, bpp);
		this->fileHeader.dataOffset = this->fileHeader.size() + this->infoHeader.size();

		this->infoHeader.width = width;
		this->infoHeader.height = height;
		this->infoHeader.bpp = bpp;
		this->infoHeader.imageSize = this->bytes.size();
	}

	void Bmp::update()
	{
		this->update(this->getWidth(), this->getHeight(), this->getBytesPerPixel() * 8);
	}

	void Bmp::flipRB()
	{
		for (unsigned int i = 0; i < this->bytes.size(); i += 3)
		{
			std::swap(this->bytes.at(i), this->bytes.at(i + 2));
		}
	}

	void Bmp::flipTopDown()
	{
		bool halfMiddle = this->getHeight() % 2 != 0;
		unsigned int middle = std::floor(this->getHeight() / 2);

		for (unsigned int x = 0; x < this->getWidth(); x++)
		{
			for (unsigned int y = 0; y < middle; y++)
			{
				if (halfMiddle && y == middle)
					continue;
				auto idxSource = *this->getIndex(x, y);
				auto idxTarget = *this->getIndex(x, this->getHeight() - y - 1);
				std::swap(this->bytes.at(idxTarget + 0), this->bytes.at(idxSource + 0));
				std::swap(this->bytes.at(idxTarget + 1), this->bytes.at(idxSource + 1));
				std::swap(this->bytes.at(idxTarget + 2), this->bytes.at(idxSource + 2));
			}
		}
	}

	std::optional<size_t> Bmp::getIndex(unsigned int x, unsigned int y, DataOption mode) const
	{
		unsigned int idx = 0;
		if (mode == DataOption::Normal)
		{
			idx = x * this->getBytesPerPixel() + y * this->getWidth() * this->getBytesPerPixel();
		}
		else if (mode == DataOption::InvertY)
		{
			idx = x * this->getBytesPerPixel() + (this->getHeight() - y - 1) * this->getWidth() * this->getBytesPerPixel();
		}
		else
			assert(!"Invalid mode");

		if (idx >= this->bytes.size())
		{
			assert(!"Pixel index out of bounds");
			return {};
		}
		return idx;
	}
}