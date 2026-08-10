#include "common/rgbimage.hpp"

#include <cassert>

namespace textoolkit
{
	RgbImage::RgbImage(unsigned int baseWidth, unsigned int baseHeight, unsigned int layers, unsigned int faces, unsigned int levels, const Pixel& baseColor)
		: baseWidth(baseWidth)
		, baseHeight(baseHeight)
	{
		this->allocate(baseWidth, baseHeight, layers, faces, levels, baseColor);
	}

	RgbImage::Type RgbImage::getType() const
	{
		return Type::RgbImage;
	}

	bool RgbImage::save(std::ostream& stream) const
	{
		assert(!"Unsupported, first convert to another format");
		return false;
	}

	void RgbImage::allocate(unsigned int baseWidth, unsigned int baseHeight, unsigned int layers, unsigned int faces, unsigned int levels, const Pixel& baseColor)
	{
		this->baseWidth = baseWidth;
		this->baseHeight = baseHeight;

		this->layers.resize(layers);
		for (auto& layer : this->layers)
		{
			layer.resize(faces);
			for (auto& face : layer)
			{
				face.resize(levels);

				unsigned int levelIdx = 0;
				for (auto& level : face)
				{
					level.resize(this->calculateSize(levelIdx++));
					this->fill(level, baseColor);
				}
			}
		}
	}

	void RgbImage::fill(const Pixel& baseColor)
	{
		for (auto& layer : this->layers)
		{
			this->fill(layer, baseColor);
		}
	}

	std::optional<unsigned char> RgbImage::getByte(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		auto idx = this->getIndex(x, y, layer, face, level, mode);
		if (!idx)
			return {};
		return this->getData(layer, face, level)->at(*idx);
	}

	std::vector<unsigned char> RgbImage::getBytes(unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		auto data = this->getData(layer, face, level);
		if (!data)
			return {};
		return *data;
	}

	std::optional<Pixel> RgbImage::getPixel(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		auto idx = this->getIndex(x, y, layer, face, level, mode);
		const auto size = this->getSize(layer, face, level);
		if (!idx || *idx + 2 >= size)
			return {};
		auto data = this->getData(layer, face, level);
		Pixel result;
		result.r = data->at(*idx + 0);
		result.g = data->at(*idx + 1);
		result.b = data->at(*idx + 2);
		result.a = 255;
		return result;
	}

	std::vector<Pixel> RgbImage::getPixels(unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		std::vector<Pixel> result;
		auto data = this->getData(layer, face, level);
		if (!data)
			return {};
		
		for (unsigned int i = 0; i < data->size(); i++)
		{
			unsigned int byteidx = i % bytesPerPixel;

			if (byteidx == 0)
			{
				result.emplace_back();
			}

			Pixel& newpixel = result.back();
			newpixel[byteidx] = data->at(i);
		}
		return result;
	}

	bool RgbImage::setByte(unsigned char byte, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode)
	{
		auto idx = this->getIndex(x, y, layer, face, level, mode);
		if (!idx)
			return false;

		auto data = this->getData(layer, face, level);
		data->at(*idx) = byte;

		return true;
	}

	bool RgbImage::setPixel(Pixel pixel, unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode)
	{
		auto idx = this->getIndex(x, y, layer, face, level, mode);
		if (!idx || *idx + 2 >= this->getSize(layer, face, level))
			return false;

		auto data = this->getData(layer, face, level);
		data->at(*idx + 0) = pixel.r;
		data->at(*idx + 1) = pixel.g;
		data->at(*idx + 2) = pixel.b;

		return true;
	}

	unsigned int RgbImage::getWidth(unsigned int level) const
	{
		return this->baseWidth / std::pow(2, level);
	}

	unsigned int RgbImage::getHeight(unsigned int level) const
	{
		return this->baseHeight / std::pow(2, level);
	}

	unsigned int RgbImage::getLayers() const
	{
		return this->layers.size();
	}

	unsigned int RgbImage::getFaces() const
	{
		if (this->layers.empty())
			return 0;
		return this->layers.front().size();
	}

	unsigned int RgbImage::getLevels() const
	{
		if (this->layers.empty())
			return 0;
		auto& l = this->layers.front();
		if (l.empty())
			return 0;
		return l.size();
	}

	const RgbImage::DataContainer* RgbImage::getData(unsigned int layer, unsigned int face, unsigned int level) const
	{
		if (layer >= this->layers.size())
			return nullptr;

		auto& pLayer = this->layers.at(layer);
		if (face >= pLayer.size())
			return nullptr;

		auto& pFace = pLayer.at(face);
		if (level >= pFace.size())
			return nullptr;

		return &pFace.at(level);
	}

	RgbImage::DataContainer* RgbImage::getData(unsigned int layer, unsigned int face, unsigned int level)
	{
		return const_cast<DataContainer*>(const_cast<const RgbImage*>(this)->getData(layer, face, level));
		//return const_cast<RgbImage*>(this)->getData(layer, face, level);
	}

	size_t RgbImage::getSize(unsigned int layer, unsigned int face, unsigned int level) const
	{
		auto data = this->getData(layer, face, level);
		if (!data)
			return 0;
		return data->size();
	}

	size_t RgbImage::calculateSize(unsigned int level) const
	{
		return this->getWidth(level) * this->getHeight(level) * bytesPerPixel;
	}

	std::optional<size_t> RgbImage::getIndex(unsigned int x, unsigned int y, unsigned int layer, unsigned int face, unsigned int level, DataOption mode) const
	{
		static constexpr unsigned int bpp = 3;
		unsigned int idx = 0;
		if (mode == DataOption::Normal)
		{
			idx = x * bpp + y * this->getWidth(level) * bpp;
		}
		else if (mode == DataOption::InvertY)
		{
			idx = x * bpp + (this->getHeight(level) - y - 1) * this->getWidth(level) * bpp;
		}
		else
			assert(!"Invalid mode");

		if (idx >= this->getSize(layer, face, level))
		{
			assert(!"Pixel index out of bounds");
			return {};
		}
		return idx;
	}

	void RgbImage::fill(Level& level, const Pixel& baseColor)
	{
		for (unsigned int i = 0; i < level.size(); i++)
		{
			unsigned int pixelIdx = i % bytesPerPixel;
			level.at(i) = baseColor[pixelIdx];
		}
	}

	void RgbImage::fill(Face& face, const Pixel& baseColor)
	{
		for (auto& level : face)
		{
			this->fill(level, baseColor);
		}
	}

	void RgbImage::fill(Layer& layer, const Pixel& baseColor)
	{
		for (auto& face : layer)
		{
			this->fill(face, baseColor);
		}
	}
}