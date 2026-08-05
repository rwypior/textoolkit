#include "common/shape.hpp"

namespace textoolkit
{
	Rectangle::Rectangle(const glm::uvec2& bottomleft, const glm::uvec2& topright)
		: bottomleft(bottomleft)
		, topright(topright)
	{
	}

	Rectangle::Rectangle(const Rectangle& b)
		: bottomleft(b.bottomleft)
		, topright(b.topright)
	{
	}

	Rectangle::Rectangle(Rectangle&& b)
		: bottomleft(std::move(b.bottomleft))
		, topright(std::move(b.topright))
	{
	}

	std::shared_ptr<Rectangle> Rectangle::create(const glm::uvec2& bottomleft, const glm::uvec2& topright)
	{
		return std::make_shared<Rectangle>(bottomleft, topright);
	}

	std::shared_ptr<Rectangle> Rectangle::create(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top)
	{
		return Rectangle::create(glm::uvec2(left, bottom), glm::uvec2(right, top));
	}

	bool Rectangle::contains(const glm::uvec2& point) const
	{
		return
			point.x >= bottomleft.x && point.x <= topright.x &&
			point.y >= bottomleft.y && point.y <= topright.y;
	}

	Rectangle Rectangle::getBoundingBox() const
	{
		return *this;
	}

	glm::uvec2 Rectangle::getBottomLeft() const
	{
		return this->bottomleft;
	}

	glm::uvec2 Rectangle::getTopRight() const
	{
		return this->topright;
	}

	size_t Rectangle::getLeft() const
	{
		return this->bottomleft.x;
	}

	size_t Rectangle::getRight() const
	{
		return this->topright.x;
	}

	size_t Rectangle::getTop() const
	{
		return this->topright.y;
	}

	size_t Rectangle::getBottom() const
	{
		return this->bottomleft.y;
	}

	size_t Rectangle::getWidth() const
	{
		return this->getRight() - this->getLeft();
	}

	size_t Rectangle::getHeight() const
	{
		return this->getTop() - this->getBottom();
	}

	size_t Rectangle::getArea() const
	{
		return this->getWidth() * this->getHeight();
	}
}