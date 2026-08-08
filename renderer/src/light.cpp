#include "renderer/light.hpp"

namespace textoolkit::renderer
{
	void Light::setDirection(const glm::vec3& direction)
	{
		this->direction = direction;
	}

	const glm::vec3& Light::getDirection() const
	{
		return this->direction;
	}

	void Light::setColor(const glm::vec3& color)
	{
		this->color = color;
	}

	glm::vec3 Light::getColor() const
	{
		return this->color;
	}
}