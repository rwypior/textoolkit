#ifndef _h_textoolkit_renderer_light
#define _h_textoolkit_renderer_light

#include "renderer/object.hpp"

#include <glm/vec3.hpp>

namespace textoolkit::renderer
{
	// TODO - maybe at some point change it to deferred shading

	class Light
	{
	public:
		void setDirection(const glm::vec3& direction);
		const glm::vec3& getDirection() const;

		void setColor(const glm::vec3& color);
		glm::vec3 getColor() const;

	private:
		glm::vec3 direction = glm::normalize(glm::vec3(0.3f, 0.3f, 1.0f));
		glm::vec3 color{1.0f};
	};
}

#endif