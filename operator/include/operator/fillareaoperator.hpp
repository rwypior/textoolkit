#ifndef _h_textoolkit_operator_fillareaoperator
#define _h_textoolkit_operator_fillareaoperator

#include "operator.hpp"

#include <memory>

namespace textoolkit
{
	class Filler;
	class Shape;

	class FillAreaOperator : public Operator
	{
	public:
		FillAreaOperator(std::shared_ptr<Shape>&& shape, std::shared_ptr<Filler>&& filler);

		virtual OperatorResult apply(Image& image, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const override;

	private:
		std::shared_ptr<Shape> shape;
		std::shared_ptr<Filler> filler;
	};
}

#endif