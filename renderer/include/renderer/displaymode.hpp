#ifndef _h_textoolkit_renderer_displaymode
#define _h_textoolkit_renderer_displaymode

#include <string>
#include <memory>

namespace textoolkit::renderer
{
	struct DisplayMode
	{
		std::string name;
		std::string model;
		std::string shader;

		DisplayMode(const std::string& name, const std::string& model, const std::string& shader)
			: name(name)
			, model(model)
			, shader(shader)
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