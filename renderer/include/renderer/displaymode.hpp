#ifndef _h_textoolkit_renderer_displaymode
#define _h_textoolkit_renderer_displaymode

#include <string>
#include <memory>
#include <unordered_set>

namespace textoolkit::renderer
{
	struct DisplayMode
	{
		std::string name;
		std::string model;
		std::string shader;
		std::unordered_set<std::string> defaultFor;

		DisplayMode(const std::string& name, const std::string& model, const std::string& shader, const std::unordered_set<std::string>& defaultFor = {})
			: name(name)
			, model(model)
			, shader(shader)
			, defaultFor(defaultFor)
		{
		}
		
		DisplayMode() = default;
		DisplayMode(DisplayMode&& b) = default;
		DisplayMode(const DisplayMode& b) = default;
		DisplayMode& operator=(DisplayMode&& b) = default;
		DisplayMode& operator=(const DisplayMode& b) = default;
	};
}

#endif