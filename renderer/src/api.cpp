#include "renderer/api.hpp"
#include "renderer/util.hpp"
#include "common/util.hpp"
#include "common/logger.hpp"

namespace textoolkit::renderer
{
	bool Api::initialized = false;

	void glCheckError2(const char* file, unsigned int line)
	{
		if (GLuint glerr = glGetError())
		{
			char msg[128];
			unsigned int l = sprintf(msg, "OpenGL error %d - %s at %s:%d", glerr, getErrorString(glerr).c_str(), file, line);
			textoolkit::Logger::getLogger() << std::string(msg, l) << "\n";
			breakpoint();
		}
	}

	inline std::string getErrorString(const unsigned int err)
	{
		switch (err)
		{
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE";
		}
		return "UNKNOWN ERROR";
	}

	inline std::string getFramebufferError(const unsigned int code)
	{
		switch (code)
		{
		case GL_FRAMEBUFFER_COMPLETE: return "GL_FRAMEBUFFER_COMPLETE"; // 0X8CD5
		case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED"; // 0X8219
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; // 0X8CD6
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; // 0X8CD7
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; // 0X8CD8
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; // 0X8CDC
		case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED"; // 0X8CDD
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; // 0X8D56
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; // 0X8DA8
		}
		return "UNKNOWN ERROR";
	}

	GLenum translateRenderMode(textoolkit::renderer::RenderMode mode)
	{
		switch (mode)
		{
		case textoolkit::renderer::RenderMode::Lines:
			return GL_LINES;
		case textoolkit::renderer::RenderMode::Solid:
			return GL_TRIANGLES;
		}

		assert(!"Invalid render mode");
		return GL_TRIANGLES;
	}

	Api::Api(std::shared_ptr<Context> context)
		: context(std::move(context))
	{
		if (this->initialized)
			return;

		this->context->setCurrent();

		glewExperimental = true;
		auto result = glewInit();
		glCheckError();
		assert(result == GLEW_OK);

		initialized = true;
	}

	Api::~Api()
	{
		glCheckError(); // Sanity check
	}

	std::shared_ptr<Context> Api::getContext()
	{
		return this->context;
	}
}