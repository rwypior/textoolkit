#include "renderer/renderer.hpp"
#include "renderer/model.hpp"
#include "renderer/object.hpp"
#include "renderer/util.hpp"
#include "common/result.hpp"
#include "common/util.hpp"
#include "common/logger.hpp"
#include "common/textoolkit.hpp"

#include <iniparser/iniparser.hpp>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <fstream>
#include <vector>
#include <cassert>

namespace
{
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

	inline void glCheckError()
	{
		if (GLuint glerr = glGetError())
		{
			char msg[128];
			unsigned int l = sprintf(msg, "OpenGL error %d - %s at %s:%d", glerr, getErrorString(glerr).c_str(), __FILE__, __LINE__);
			textoolkit::Logger::getLogger() << std::string(msg, l) << "\n";
			breakpoint();
		}
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
}

namespace textoolkit::renderer
{
	// Uniform impl

	class GLUniformData : public UniformData
	{
	public:
		GLUniformData(GLuint id)
			: id(id)
		{
		}

		GLuint id;
	};

	class Uniform::Impl
	{
		friend class Uniform;

	public:
		Impl(const UniformData& data)
			: id(static_cast<const GLUniformData&>(data).id)
		{
		}

		GLuint getId() const
		{
			return this->id;
		}

	private:
		GLuint id;
	};

	// Uniform

	Uniform::Uniform(const UniformData& data)
		: impl(std::make_unique<Impl>(data))
	{
	}

	void Uniform::set(float val)
	{
		glUniform1f(this->impl->getId(), val);
	}

	void Uniform::set(const glm::vec3& vec)
	{
		glUniform3fv(this->impl->getId(), 1, &vec[0]);
	}

	Uniform::Visitor Uniform::getSetter()
	{
		return Visitor(*this);
	}

	// Shader impl

	class Shader::Impl
	{
		friend class Shader;

	public:
		enum class LoadResult
		{
			Ok,
			CannotOpen,
			CompilationFailure
		};
		
		struct CompileInfo
		{
			GLint code;
			std::string details;
		};

		struct CompileResult : Result<CompileInfo>
		{
			using Result::Result;

			CompileResult(GLint code, const std::string details)
				: Result({code, details})
			{
			}
		};

	public:
		Impl(
			const std::string& vert,
			const std::string& frag,
			const std::string& geom = NoShader
		)
		{
			glCheckError();

			this->load(GL_VERTEX_SHADER, vert);
			this->load(GL_FRAGMENT_SHADER, frag);

			if (!geom.empty())
				this->load(GL_GEOMETRY_SHADER, geom);

			glLinkProgram(this->program);

			GLint usedUniforms, usedAttributes;
			glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &usedUniforms);
			glGetProgramiv(this->program, GL_ACTIVE_ATTRIBUTES, &usedAttributes);

			glCheckError();

			GLint loglength;

			glValidateProgram(this->program);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &loglength);

			GLchar* buffer = new GLchar[loglength];
			glGetProgramInfoLog(this->program, loglength, NULL, &buffer[0]);

			if (loglength > 0)
				textoolkit::Logger::getLogger() << std::string(buffer, loglength) << "\n";

			delete[] buffer;

			this->loadUniforms();

			this->presentShaders |=
				static_cast<unsigned int>(ShaderTypes::Vertex) |
				static_cast<unsigned int>(ShaderTypes::Fragment);

			if (!geom.empty())
				this->presentShaders |= static_cast<unsigned int>(ShaderTypes::Geometry);
		}

		LoadResult load(GLuint type, const std::string& path)
		{
			GLuint shader = glCreateShader(type);

			std::ifstream stream(path, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

			if (!stream.is_open())
				return LoadResult::CannotOpen;

			std::streampos length = stream.tellg();
			stream.seekg(0, std::ios_base::beg);
			std::string source(static_cast<size_t>(length), '\0');
			stream.read(&source[0], length);
			stream.close();

			const char* cSource = source.c_str();
			glShaderSource(shader, 1, &cSource, NULL);
			glCompileShader(shader);
			if (!this->checkCompileStatus(shader))
				return LoadResult::CompilationFailure;

			glAttachShader(this->program, shader);

			glCheckError();

			return LoadResult::Ok;
		}

		void loadUniforms()
		{
			GLint count = 0;
			glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &count);
			for (GLint i = 0; i < count; i++)
			{
				GLchar name[256];
				GLint size = 0;
				GLenum type;
				glGetActiveUniform(this->program, (GLuint)i, sizeof(name), NULL, &size, &type, name);
				GLUniformData data(glGetUniformLocation(this->program, name));
				this->uniforms[name] = std::make_unique<Uniform>(data);
			}
		}

		CompileResult checkCompileStatus(const GLuint index)
		{
			GLint status;
			GLint loglength;

			glGetShaderiv(index, GL_COMPILE_STATUS, &status);
			glGetShaderiv(index, GL_INFO_LOG_LENGTH, &loglength);
			GLchar* buffer = new GLchar[loglength];
			glGetShaderInfoLog(index, loglength, NULL, buffer);

			if (status == GL_FALSE)
				return CompileResult(status, buffer);

			delete[] buffer;
			return CompileResult(CompileResult::Code::Ok);
		}

	private:
		GLuint program;
		unsigned int presentShaders = 0x00;
		std::unordered_map<std::string, std::unique_ptr<Uniform>> uniforms;
	};

	// Shader

	Shader::Shader() = default;

	Shader::Shader(Shader&& shader)
		: impl(std::move(shader.impl))
	{
	}

	Shader::Shader(
		const std::string& vert,
		const std::string& frag,
		const std::string& geom
	)
		: impl(std::make_unique<Impl>(vert, frag, geom))
	{
	}

	Shader::~Shader() = default;

	Shader& Shader::operator=(Shader&& shader)
	{
		this->impl = std::move(shader.impl);
		return *this;
	}

	void Shader::use()
	{
		glUseProgram(this->impl->program);
		glCheckError();
	}

	bool Shader::hasUniform(const std::string& name) const
	{
		return this->getUniform(name);
	}

	Uniform* Shader::getUniform(const std::string& name) const
	{
		if (this->impl->uniforms.find(name) != this->impl->uniforms.end())
			return this->impl->uniforms.at(name).get();

		return nullptr;
	}

	// Model data

	class OpenglModelData : public ModelRenderData
	{
	public:
		OpenglModelData()
		{
			glGenBuffers(1, &this->vertexBuffer);
			glGenBuffers(1, &this->indexBuffer);
			glCheckError();
		}

		~OpenglModelData()
		{
			glDeleteBuffers(1, &this->vertexBuffer);
			glDeleteBuffers(1, &this->indexBuffer);
		}

		void update(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices) override
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
			if (points.size() == 0)
				glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_DYNAMIC_DRAW);
			glCheckError();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
			if (indices.size() == 0)
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_DYNAMIC_DRAW);
			glCheckError();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glCheckError();
		}

		GLuint getVertexBuffer() const
		{
			return this->vertexBuffer;
		}

		GLuint getNormalBuffer() const
		{
			return this->normalBuffer;
		}

		GLuint getUvBuffer() const
		{
			return this->uvBuffer;
		}

		GLuint getIndexBuffer() const
		{
			return this->indexBuffer;
		}

	private:
		GLuint vertexBuffer = -1;
		GLuint normalBuffer = -1;
		GLuint uvBuffer = -1;
		GLuint indexBuffer = -1;
	};

	// Object data

	class OpenglObjectData : public ObjectRenderData
	{
	public:
		OpenglObjectData()
		{
			glGenVertexArrays(1, &this->vao);
			glCheckError();
		}

		~OpenglObjectData()
		{
			glDeleteVertexArrays(1, &this->vao);
			glCheckError();
		}

		void update(const Model& model) override
		{
			ModelRenderData* data = model.getData();
			if (!data)
			{
				assert(!"Model is not uploaded");
				return;
			}

			auto gldata = static_cast<OpenglModelData*>(data);

			glBindVertexArray(this->vao);
			glCheckError();

			// Bind buffers
			glBindBuffer(GL_ARRAY_BUFFER, gldata->getVertexBuffer());
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glCheckError();

			if (gldata->getNormalBuffer() != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, gldata->getNormalBuffer());
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glCheckError();
			}

			if (gldata->getUvBuffer() != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, gldata->getUvBuffer());
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
				glCheckError();
			}

			// Bind ibo
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gldata->getIndexBuffer());
			glCheckError();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glCheckError();
		}

		GLuint getVao() const
		{
			return this->vao;
		}

	private:
		GLuint vao;
	};

	// Renderer priority

	bool RendererPriorityHandler::operator()(const Object* a, const Object* b) const
	{
		return false;
	}

	// Renderer

	Renderer::Renderer()
	{
		this->properties[propertyProjection] = glm::mat4(1.0f);
		this->properties[propertyView] = this->camera.getViewMatrix();
		this->properties[propertyProjection] = glm::mat4(1.0f);
		this->properties[propertyGlobalLightColor] = glm::vec3(0.7f);
		this->properties[propertyLightColor] = this->light.getColor();
		this->properties[propertyLightDirection] = this->light.getDirection();
	}

	bool Renderer::loadShader(const std::string& sdrPath)
	{
		Ini::Parser parser;
		Ini::Model model;
		Ini::Result result = parser.readFile(sdrPath, model);
		if (result.code != Ini::Result::Code::OK)
			return false;

		auto it = model.findSection("shader");
		if (it == model.sections.end())
			return false;

		auto& section = it->second;

		auto nameIt = section.find("name");
		if (nameIt == section.end())
			return false;

		std::string name = nameIt->second->toString();

		std::string vert = Shader::NoShader;
		std::string geom = Shader::NoShader;
		std::string frag = Shader::NoShader;

		auto itVert = section.find("vertex");
		if (itVert != section.end())
			vert = itVert->second->toString();
		
		auto itGeom = section.find("geometry");
		if (itGeom != section.end())
			geom = itGeom->second->toString();
		
		auto itFrag = section.find("fragment");
		if (itFrag != section.end())
			frag = itFrag->second->toString();

		this->shaders[name] = Shader(vert, frag, geom);

		return true;
	}

	void Renderer::update()
	{
		if (this->camera.updateRequired())
		{
			this->camera.update();
			this->properties[propertyView] = this->camera.getViewMatrix();
		}

		if (this->updateRequired)
		{
			this->properties[propertyProjection] = glm::perspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);

			glViewport(0, 0, this->viewportSize.x, this->viewportSize.y);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glClearStencil(0);

			this->updateRequired = false;
		}

		this->properties[propertyLightColor] = this->light.getColor();
		this->properties[propertyLightDirection] = this->light.getDirection();
	}

	void Renderer::render()
	{
		this->initialize();
		this->update();

		while (!this->renderQueue.empty())
		{
			Object* object = this->renderQueue.top();
			this->renderQueue.pop();

			// Get object data
			if (!object->isUploaded())
				this->upload(*object);

			if (object->updateRequired())
				this->update(*object);

			auto model = object->getModel();
			if (!model)
			{
				assert(!"Object has no model assigned");
				continue;
			}

			if (!model->isUploaded())
				this->upload(*model);

			if (model->updateRequired())
				this->update(*model);

			// Set shader
			const std::string& shaderName = object->getShader();
			auto shader = this->getShader(shaderName);
			if (!shader)
			{
				assert(!"Object has no valid shader assigned");
				continue;
			}

			shader->use();

			// Set uniforms
			const auto& properties = object->getProperties();
			for (const auto& container : { this->properties, properties })
			{
				for (const auto& [name, prop] : container)
				{
					auto uniform = shader->getUniform(name);
					std::visit(uniform->getSetter(), prop);
				}
			}

			// Render
			auto renderMode = translateRenderMode(object->getRenderMode());
			auto data = static_cast<OpenglObjectData*>(object->getData());
			glBindVertexArray(data->getVao());
			glDrawElements(renderMode, model->getIndices().size(), GL_UNSIGNED_INT, 0);
			glCheckError();
		}
	}

	Shader* Renderer::getShader(const std::string& name)
	{
		auto it = this->shaders.find(name);
		if (it == this->shaders.end())
			return nullptr;
		return &it->second;
	}

	void Renderer::enqueue(Object* object)
	{ 
		this->renderQueue.push(object);
	}

	void Renderer::initialize()
	{
		if (this->isInitialized())
			return;

		glewExperimental = true;
		auto result = glewInit();
		assert(result == GLEW_OK);
		this->initialized = true;
		this->rendererReady(*this);
	}

	bool Renderer::isInitialized() const
	{
		return this->initialized;
	}

	void Renderer::upload(Model& model) const
	{
		auto data = std::make_unique<OpenglModelData>();
		data->update(model.getVertices(), model.getIndices());
		model.setData(std::move(data));
	}

	void Renderer::update(Model& model) const
	{
		auto data = model.getData();
		if (!data)
		{
			assert(!"Model is not uploaded");
			return;
		}
		
		auto gldata = static_cast<OpenglModelData*>(data);
		gldata->update(model.getVertices(), model.getIndices());
	}

	void Renderer::upload(Object& object) const
	{
		auto model = object.getModel();
		if (!model)
		{
			assert(!"Object has no model");
			return;
		}

		auto data = std::make_unique<OpenglObjectData>();
		data->update(*object.getModel());
		object.setData(std::move(data));
	}

	void Renderer::update(Object& object) const
	{
		auto data = object.getData();
		if (!data)
		{
			assert(!"Object is not uploaded");
			return;
		}

		auto model = object.getModel();
		if (!model)
		{
			assert(!"Object has no model");
			return;
		}

		auto gldata = static_cast<OpenglObjectData*>(data);
		gldata->update(*model);
	}

	void Renderer::setFieldOfView(float fov)
	{
		this->fov = fov;
		this->updateRequired = true;
	}

	void Renderer::setNearPlane(float near)
	{
		this->nearPlane = near;
		this->updateRequired = true;
	}

	void Renderer::setFarPlane(float far)
	{
		this->farPlane = far;
		this->updateRequired = true;
	}

	void Renderer::setViewportSize(const glm::uvec2& wh)
	{
		this->viewportSize = wh;
		this->aspectRatio = static_cast<float > (wh.x) / static_cast<float>(wh.y);
		this->updateRequired = true;
	}

	glm::vec3 Renderer::getCameraPosition() const
	{
		return this->camera.getPosition();
	}

	void Renderer::setCameraPosition(const glm::vec3& position)
	{
		this->camera.setPosition(position);
	}

	glm::vec3 Renderer::getCameraLookTarget() const
	{
		return this->camera.getLookTarget();
	}

	void Renderer::setCameraLookTarget(const glm::vec3& lookTarget)
	{
		this->camera.setLookTarget(lookTarget);
	}

	glm::vec3 Renderer::getCameraUpVector() const
	{
		return this->camera.getUpVector();
	}

	void Renderer::setCameraUpVector(const glm::vec3& up)
	{
		this->camera.setUpVector(up);
	}

	glm::vec3 Renderer::getLightColor() const
	{
		return this->light.getColor();
	}

	void Renderer::setLightColor(const glm::vec3& color)
	{
		this->light.setColor(color);
	}

	glm::vec3 Renderer::getLightDirection() const
	{
		return this->light.getDirection();
	}

	void Renderer::setLightDirection(const glm::vec3& direction)
	{
		this->light.setDirection(direction);
	}
}