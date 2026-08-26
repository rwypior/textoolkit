#include "texture/accessor.hpp"
#include "common/image.hpp"
#include "common/pixel.hpp"

#include <glm/common.hpp>

#include <array>
#include <algorithm>

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
		const float srcxreal = (static_cast<float>(x)) * ratiox - 0.5f;
		const float srcyreal = (static_cast<float>(y)) * ratioy - 0.5f;
		const float srcx = std::floor(srcxreal);
		const float srcy = std::floor(srcyreal);
		const float srcxfract = srcxreal - srcx;
		const float srcyfract = srcyreal - srcy;

		std::array<glm::vec4, 16> samples;
		for (int sampley = -1; sampley < 3; sampley++)
		{
			for (int samplex = -1; samplex < 3; samplex++)
			{
				int idx = (sampley + 1) * 4 + (samplex + 1);
				int idxx = srcx + samplex;
				int idxy = srcy + sampley;
				samples[idx] = this->sample(idxx, idxy).toVec4<glm::vec4, false>();
			}
		}

		auto cubic = [](glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, float fract) {
			const auto s1 = a / -2.0f + b * 3.0f / 2.0f - c * 3.0f / 2.0f + d / 2.0f;
			const auto s2 = a - b * 5.0f / 2.0f + c * 2.0f - d / 2.0f;
			const auto s3 = -a / 2.0f + c / 2.0f;
			const auto s4 = b;
			return s1 * std::pow(fract, 3.0f) + s2 * std::pow(fract, 2.0f) + s3 * fract + d;
		};

		const auto s1 = cubic(samples[0], samples[1], samples[2], samples[3], srcxfract);
		const auto s2 = cubic(samples[4], samples[5], samples[6], samples[7], srcxfract);
		const auto s3 = cubic(samples[8], samples[9], samples[10], samples[11], srcxfract);
		const auto s4 = cubic(samples[12], samples[13], samples[14], samples[15], srcxfract);
		const auto result = glm::clamp(cubic(s1, s2, s3, s4, srcyfract), glm::vec4(0.0f), glm::vec4(255.0f));
		
		return Pixel(result.r, result.g, result.b, result.a);
	}

	Pixel BicubicAccessor::sample(int x, int y) const
	{
		x = std::clamp(x, 0, static_cast<int>(this->subAccessor->getImage().getWidth() - 1));
		y = std::clamp(y, 0, static_cast<int>(this->subAccessor->getImage().getHeight() - 1));

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