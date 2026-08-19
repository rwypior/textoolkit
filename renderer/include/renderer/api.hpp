#ifndef _h_textoolkit_renderer_api
#define _h_textoolkit_renderer_api

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include <variant>
#include <map>
#include <string>
#include <memory>

namespace textoolkit::renderer
{
	using RenderProperty = std::variant<bool, float, glm::vec3, glm::mat3, glm::mat4>;
	using RenderProperties = std::map<std::string, RenderProperty>;

	enum class RenderMode
	{
		Solid,
		Lines
	};

	void glCheckError2(const char* file, unsigned int line);
	std::string getErrorString(const unsigned int err);
	std::string getFramebufferError(const unsigned int code);
	GLenum translateRenderMode(textoolkit::renderer::RenderMode mode);

#define glCheckError() glCheckError2(__FILE__, __LINE__);

	class Context
	{
	public:
		virtual ~Context() = default;
		virtual bool setCurrent() = 0;
		virtual void swapBuffers() = 0;
		virtual bool isOk() const = 0;
	};

	class Api
	{
	public:
		Api(std::shared_ptr<Context> context);
		~Api();

		std::shared_ptr<Context> getContext();

	private:
		static bool initialized;
		std::shared_ptr<Context> context;
	};
}

#endif