#ifndef _h_textoolkit_common_shape
#define _h_textoolkit_common_shape

#include <glm/vec2.hpp>

#include <memory>

namespace textoolkit
{
	class Rectangle;

	class Shape
	{
	public:
		virtual ~Shape() = default;
		virtual bool contains(const glm::uvec2& point) const = 0;
		virtual Rectangle getBoundingBox() const = 0;
	};

	class Rectangle : public Shape
	{
	public:
		Rectangle(const glm::uvec2& bottomleft, const glm::uvec2& topright);
		Rectangle(const Rectangle& b);
		Rectangle(Rectangle&& b);

		static std::shared_ptr<Rectangle> create(const glm::uvec2& bottomleft, const glm::uvec2& topright);
		static std::shared_ptr<Rectangle> create(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top);

		virtual bool contains(const glm::uvec2& point) const override;
		virtual Rectangle getBoundingBox() const override;

		glm::uvec2 getBottomLeft() const;
		glm::uvec2 getTopRight() const;
		size_t getLeft() const;
		size_t getRight() const;
		size_t getTop() const;
		size_t getBottom() const;
		size_t getWidth() const;
		size_t getHeight() const;
		size_t getArea() const;

	private:
		glm::uvec2 bottomleft;
		glm::uvec2 topright;
	};
}

#endif