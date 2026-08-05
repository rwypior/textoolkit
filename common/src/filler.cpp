#include "common/filler.hpp"

namespace textoolkit
{
	SolidFiller::SolidFiller(const Pixel& color)
		: color(color)
	{
	}

	std::shared_ptr<SolidFiller> SolidFiller::create(const Pixel& color)
	{
		return std::make_shared<SolidFiller>(color);
	}

	Pixel SolidFiller::getColor(const glm::uvec2& /*point*/) const
	{
		return this->color;
	}
}