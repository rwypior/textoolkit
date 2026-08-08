#ifndef _h_textoolkit_renderer_util
#define _h_textoolkit_renderer_util

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include <variant>
#include <map>
#include <string>

namespace textoolkit::renderer
{
	using RenderProperty = std::variant<bool, float, glm::vec3, glm::mat3, glm::mat4>;
	using RenderProperties = std::map<std::string, RenderProperty>;

	enum class RenderMode
	{
		Solid,
		Lines
	};
}

#endif