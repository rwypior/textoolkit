#include "common/accessor.hpp"
#include "common/image.hpp"
#include "common/pixel.hpp"

#include <array>

namespace textoolkit
{
	// Passthrough accessor

	SimpleAccessor::SimpleAccessor(const Image& image, unsigned int layer, unsigned int face, unsigned int level)
		: image(&image)
		, layer(layer)
		, face(face)
		, level(level)
		, subAccessor(nullptr)
	{
	}

	Pixel SimpleAccessor::getPixel(unsigned int x, unsigned int y) const
	{
		if (this->subAccessor)
			return this->subAccessor->getPixel(x, y);

		auto pixel = this->image->getPixel(x, y, this->layer, this->face, this->level);
		if (pixel)
			return *pixel;
		return Pixel(0);
	}

	PixelAccessor& SimpleAccessor::setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor)
	{
		this->subAccessor = std::move(subAccessor);
		return *this;
	}

	const Image& SimpleAccessor::getImage() const
	{
		return *this->image;
	}

	unsigned int SimpleAccessor::getLayer() const
	{
		return this->layer;
	}

	unsigned int SimpleAccessor::getFace() const
	{
		return this->face;
	}

	unsigned int SimpleAccessor::getLevel() const
	{
		return this->level;
	}

	PixelAccessor& SimpleAccessor::setImage(const Image& image)
	{
		this->image = &image;
		return *this;
	}

	PixelAccessor& SimpleAccessor::setLayer(unsigned int layer)
	{
		this->layer = layer;
		return *this;
	}

	PixelAccessor& SimpleAccessor::setFace(unsigned int face)
	{
		this->face = face;
		return *this;
	}

	PixelAccessor& SimpleAccessor::setLevel(unsigned int level)
	{
		this->level = level;
		return *this;
	}

	// Invert Y accessor

	InvertYAccessor::InvertYAccessor(const Image& image, unsigned int layer, unsigned int face, unsigned int level)
		: subAccessor(std::make_unique<SimpleAccessor>(image, layer, face, level))
	{
	}

	Pixel InvertYAccessor::getPixel(unsigned int x, unsigned int y) const
	{
		assert(this->subAccessor && "This accessor must have a subaccessor");

		unsigned int y2 = this->subAccessor->getImage().getHeight() - y - 1;
		return this->subAccessor->getPixel(x, y2);
	}

	PixelAccessor& InvertYAccessor::setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor)
	{
		this->subAccessor = std::move(subAccessor);
		return *this;
	}

	const Image& InvertYAccessor::getImage() const
	{
		return this->subAccessor->getImage();
	}

	unsigned int InvertYAccessor::getLayer() const
	{
		return this->subAccessor->getLayer();
	}

	unsigned int InvertYAccessor::getFace() const
	{
		return this->subAccessor->getFace();
	}

	unsigned int InvertYAccessor::getLevel() const
	{
		return this->subAccessor->getLevel();
	}

	PixelAccessor& InvertYAccessor::setImage(const Image& image)
	{
		this->subAccessor->setImage(image);
		return *this;
	}

	PixelAccessor& InvertYAccessor::setLayer(unsigned int layer)
	{
		this->subAccessor->setLayer(layer);
		return *this;
	}

	PixelAccessor& InvertYAccessor::setFace(unsigned int face)
	{
		this->subAccessor->setFace(face);
		return *this;
	}

	PixelAccessor& InvertYAccessor::setLevel(unsigned int level)
	{
		this->subAccessor->setLevel(level);
		return *this;
	}

	// Bicubic accessor

	BicubicAccessor::BicubicAccessor(const Image& image, unsigned int scaledWidth, unsigned int scaledHeight, unsigned int layer, unsigned int face, unsigned int level)
		: subAccessor(std::make_unique<SimpleAccessor>(image, layer, face, level))
		, scaledWidth(scaledWidth)
		, scaledHeight(scaledHeight)
	{
	}

	Pixel BicubicAccessor::getPixel(unsigned int x, unsigned int y) const
	{
		assert(this->subAccessor && "This accessor must have a subaccessor");

		const float ratiox = static_cast<float>(this->subAccessor->getImage().getWidth()) / static_cast<float>(this->scaledWidth);
		const float ratioy = static_cast<float>(this->subAccessor->getImage().getHeight()) / static_cast<float>(this->scaledHeight);
		const unsigned int srcx = static_cast<unsigned int>(x * ratiox);
		const unsigned int srcy = static_cast<unsigned int>(y * ratioy);
		const float dx = ratiox * x - srcx;
		const float dy = ratioy * y - srcy;

		Pixel result;
		std::array<Pixel, 4> samples;
		for (int sample = -1; sample < 3; sample++)
		{
			int samplex = srcy + sample;

			auto pixel0 = this->sample(samplex, srcy);
			auto pixel1 = this->sample(samplex, srcy - 1) - pixel0;
			auto pixel2 = this->sample(samplex, srcy + 1) - pixel0;
			auto pixel3 = this->sample(samplex, srcy + 2) - pixel0;
			Pixel a = pixel1 / -3.0 + pixel2 - pixel3 / 6.0;
			Pixel b = pixel1 / 2.0 + pixel2 / 2.0;
			Pixel c = pixel1 / -6.0 - pixel2 / 2.0 + pixel3 / 6.0;
			samples[sample + 1] =
				pixel0 +
				a * std::pow(dx, 1) +
				b * std::pow(dx, 2) +
				c * std::pow(dx, 3);

			pixel0 = samples[1];
			pixel1 = samples[0] - samples[1];
			pixel2 = samples[2] - samples[1];
			pixel3 = samples[3] - samples[1];
			a = pixel1 / -3.0 + pixel2 - pixel3 / 6.0;
			b = pixel1 / 2.0 + pixel2 / 2.0;
			c = pixel1 / -6.0 - pixel2 / 2.0 + pixel3 / 6.0;
			result = 
				pixel0 +
				a * std::pow(dy, 1) +
				b * std::pow(dy, 2) +
				c * std::pow(dy, 3);
		}
		return result;
	}

	Pixel BicubicAccessor::sample(unsigned int x, unsigned int y) const
	{
		return this->subAccessor->getPixel(x, y);
	}

	PixelAccessor& BicubicAccessor::setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor)
	{
		this->subAccessor = std::move(subAccessor);
		return *this;
	}

	const Image& BicubicAccessor::getImage() const
	{
		return this->subAccessor->getImage();
	}

	unsigned int BicubicAccessor::getLayer() const
	{
		return this->subAccessor->getLayer();
	}

	unsigned int BicubicAccessor::getFace() const
	{
		return this->subAccessor->getFace();
	}

	unsigned int BicubicAccessor::getLevel() const
	{
		return this->subAccessor->getLevel();
	}

	PixelAccessor& BicubicAccessor::setImage(const Image& image)
	{
		this->subAccessor->setImage(image);
		return *this;
	}

	PixelAccessor& BicubicAccessor::setLayer(unsigned int layer)
	{
		this->subAccessor->setLayer(layer);
		return *this;
	}

	PixelAccessor& BicubicAccessor::setFace(unsigned int face)
	{
		this->subAccessor->setFace(face);
		return *this;
	}

	PixelAccessor& BicubicAccessor::setLevel(unsigned int level)
	{
		this->subAccessor->setLevel(level);
		return *this;
	}
}