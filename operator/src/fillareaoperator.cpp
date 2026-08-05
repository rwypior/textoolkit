#include "operator/fillareaoperator.hpp"
#include "common/shape.hpp"
#include "common/image.hpp"
#include "common/filler.hpp"

namespace textoolkit
{
	FillAreaOperator::FillAreaOperator(std::shared_ptr<Shape>&& shape, std::shared_ptr<Filler>&& filler)
		: shape(std::move(shape))
		, filler(std::move(filler))
	{
	}

	OperatorResult FillAreaOperator::apply(Image& image, unsigned int layer, unsigned int face, unsigned int level) const
	{
		auto bbox = this->shape->getBoundingBox();
		for (unsigned int y = bbox.getBottom(); y < bbox.getTop(); y++)
		{
			for (unsigned int x = bbox.getLeft(); x < bbox.getRight(); x++)
			{
				glm::uvec2 pt(x, y);
				if (this->shape->contains(pt))
					image.setPixel(this->filler->getColor(pt), x, y, layer, face, level);
			}
		}

		return OperatorResult::Code::Ok;
	}
}