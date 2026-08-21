#include "renderer/renderer.hpp"
#include "renderer/model.hpp"
#include "renderer/object.hpp"
#include "renderer/api.hpp"
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
#include <filesystem>

namespace
{
	GLenum translateImageTarget(textoolkit::Image::TextureType type)
	{
		switch (type)
		{
		case textoolkit::Image::TextureType::TextureCube: return GL_TEXTURE_CUBE_MAP;
		case textoolkit::Image::TextureType::Texture2D: return GL_TEXTURE_2D;
		}

		return GL_TEXTURE_2D;
	}

	GLenum translateFormat(textoolkit::Image::InfoMode mode, unsigned int format)
	{
		if (mode == textoolkit::Image::InfoMode::Opengl)
			return static_cast<GLenum>(format);

		switch (static_cast<textoolkit::Image::TextureFormat>(format))
		{
		case textoolkit::Image::TextureFormat::Rgb: return GL_RGB;
		}

		assert(!"Unsupported format");
		return GL_RGB;
	}

	GLenum translateInternalFormat(textoolkit::Image::InfoMode mode, unsigned int format)
	{
		if (mode == textoolkit::Image::InfoMode::Opengl)
			return static_cast<GLenum>(format);

		switch (static_cast<textoolkit::Image::TextureInternalFormat>(format))
		{
		case textoolkit::Image::TextureInternalFormat::Rgb8: return GL_RGB8;
		}

		assert(!"Unsupported internal format");
		return GL_RGB8;
	}

	GLenum translateDataType(textoolkit::Image::InfoMode mode, unsigned int dataType)
	{
		if (mode == textoolkit::Image::InfoMode::Opengl)
			return static_cast<GLenum>(dataType);

		switch (static_cast<textoolkit::Image::DataType>(dataType))
		{
		case textoolkit::Image::DataType::UnsignedChar: return GL_UNSIGNED_BYTE;
		}

		assert(!"Unsupported data type");
		return GL_UNSIGNED_BYTE;
	}

	GLenum translateCubeFace(textoolkit::renderer::CubeFace face)
	{
		switch (face)
		{
		case textoolkit::renderer::CubeFace::PositiveX: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case textoolkit::renderer::CubeFace::NegativeX: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case textoolkit::renderer::CubeFace::PositiveY: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case textoolkit::renderer::CubeFace::NegativeY: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case textoolkit::renderer::CubeFace::PositiveZ: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case textoolkit::renderer::CubeFace::NegativeZ: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		}

		assert(!"Invalid cubemap face");
		return 0;
	}

	GLenum translateWrapping(textoolkit::renderer::Wrapping wrap)
	{
		switch (wrap)
		{
		case textoolkit::renderer::Wrapping::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case textoolkit::renderer::Wrapping::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		case textoolkit::renderer::Wrapping::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		case textoolkit::renderer::Wrapping::Repeat:
			return GL_REPEAT;
		case textoolkit::renderer::Wrapping::MirroredClampToEdge:
			return GL_MIRROR_CLAMP_TO_EDGE;
		}

		assert(!"Unsupported mirroring");
		return GL_CLAMP_TO_EDGE;
	}

	GLenum translateFilteringMin(textoolkit::renderer::FilteringMin filter)
	{
		switch (filter)
		{
		case textoolkit::renderer::FilteringMin::Nearest:
			return GL_NEAREST;
		case textoolkit::renderer::FilteringMin::Linear:
			return GL_LINEAR;
		case textoolkit::renderer::FilteringMin::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case textoolkit::renderer::FilteringMin::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case textoolkit::renderer::FilteringMin::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case textoolkit::renderer::FilteringMin::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		}

		assert(!"Unsupported filtering");
		return GL_NEAREST;
	}

	GLenum translateFilteringMag(textoolkit::renderer::FilteringMag filter)
	{
		switch (filter)
		{
		case textoolkit::renderer::FilteringMag::Nearest:
			return GL_NEAREST;
		case textoolkit::renderer::FilteringMag::Linear:
			return GL_LINEAR;
		}

		assert(!"Unsupported filtering");
		return GL_NEAREST;
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
		glCheckError();
	}
	
	void Uniform::set(int val)
	{
		glUniform1i(this->impl->getId(), val);
		glCheckError();
	}
	
	void Uniform::set(bool val)
	{
		glUniform1i(this->impl->getId(), val);
		glCheckError();
	}

	void Uniform::set(const glm::vec3& vec)
	{
		glUniform3fv(this->impl->getId(), 1, &vec[0]);
		glCheckError();
	}

	void Uniform::set(const glm::mat3& mtx)
	{
		glUniformMatrix3fv(this->impl->getId(), 1, false, &mtx[0][0]);
		glCheckError();
	}

	void Uniform::set(const glm::mat4& mtx)
	{
		glUniformMatrix4fv(this->impl->getId(), 1, false, &mtx[0][0]);
		glCheckError();
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
			: program(glCreateProgram())
		{
			glCheckError();

			this->load(GL_VERTEX_SHADER, vert);
			this->load(GL_FRAGMENT_SHADER, frag);

			if (!geom.empty())
				this->load(GL_GEOMETRY_SHADER, geom);

			glLinkProgram(this->program);
			glCheckError();

			GLint usedUniforms, usedAttributes;
			glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &usedUniforms);
			glGetProgramiv(this->program, GL_ACTIVE_ATTRIBUTES, &usedAttributes);
			glCheckError();

			GLint loglength;

			glValidateProgram(this->program);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &loglength);
			glCheckError();

			GLchar* buffer = new GLchar[loglength];
			glGetProgramInfoLog(this->program, loglength, NULL, &buffer[0]);
			glCheckError();
			
			if (loglength > 0)
				textoolkit::Logger::getLogger() << std::string(buffer, loglength);

			delete[] buffer;

			this->loadUniforms();

			this->presentShaders |=
				static_cast<unsigned int>(ShaderTypes::Vertex) |
				static_cast<unsigned int>(ShaderTypes::Fragment);

			if (!geom.empty())
				this->presentShaders |= static_cast<unsigned int>(ShaderTypes::Geometry);
		}

		~Impl()
		{
			GLsizei count = 0;
			GLuint shaders[5];
			glGetAttachedShaders(this->program, sizeof(shaders) / sizeof(shaders[0]), &count, &shaders[0]);
			glCheckError();

			for (GLsizei i = 0; i < count; i++)
			{
				const GLuint& shader = shaders[i];

				glDetachShader(this->program, shader);
				glCheckError();

				glDeleteShader(shader);
				glCheckError();
			}

			glDeleteProgram(this->program);
			glCheckError();
		}

		LoadResult load(GLuint type, const std::string& path)
		{
			textoolkit::Logger::getLogger() << "Compiling " << path;

			GLuint shader = glCreateShader(type);
			glCheckError();

			std::ifstream stream(path, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

			if (!stream.is_open())
			{
				textoolkit::Logger::getLogger() << "Cannot open shader at " << path;
				return LoadResult::CannotOpen;
			}

			std::streampos length = stream.tellg();
			stream.seekg(0, std::ios_base::beg);
			std::string source(static_cast<size_t>(length), '\0');
			stream.read(&source[0], length);
			stream.close();

			const char* cSource = source.c_str();
			glShaderSource(shader, 1, &cSource, NULL);
			glCheckError();
			glCompileShader(shader);
			glCheckError();

			GLint loglength;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
			glCheckError();

			GLchar* buffer = new GLchar[loglength];
			glGetShaderInfoLog(shader, loglength, NULL, &buffer[0]);
			glCheckError();

			if (loglength > 0)
			{
				textoolkit::Logger::getLogger() << "Compilation finished with errors:";
				textoolkit::Logger::getLogger() << std::string(buffer, loglength);
			}

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
				std::string namelower = name;
				std::transform(namelower.begin(), namelower.end(), namelower.begin(), [](const auto& c) { return std::tolower(c); });
				this->uniforms[namelower] = std::make_unique<Uniform>(data);
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
		std::string namelower = name;
		std::transform(namelower.begin(), namelower.end(), namelower.begin(), [](const auto& c) { return std::tolower(c); });

		auto it = this->impl->uniforms.find(namelower);
		if (it != this->impl->uniforms.end())
			return it->second.get();

		return nullptr;
	}

	// Texture impl
	
	class Texture::Impl
	{
	public:
		Impl(const Image& image)
			: target(translateImageTarget(image.getTextureType()))
		{
			glGenTextures(1, &this->textureId);
			glCheckError();

			this->cubemapAlignment = {
				CubeFace::PositiveX,
				CubeFace::NegativeX,
				CubeFace::PositiveY,
				CubeFace::NegativeY,
				CubeFace::PositiveZ,
				CubeFace::NegativeZ
			};

			this->change(image);
		}

		void change(const Image& image)
		{
			glBindTexture(this->target, this->textureId);
			glCheckError();

			glTexParameteri(this->target, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(this->target, GL_TEXTURE_MAX_LEVEL, image.getLevels() - 1);
			glTexParameteriv(this->target, GL_TEXTURE_SWIZZLE_RGBA, &image.getSwizzles()[0]);
			glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glCheckError();

			switch (this->target)
			{
			case GL_TEXTURE_2D:
				this->setData2D(image);
				break;
			case GL_TEXTURE_CUBE_MAP:
				this->setDataCube(image);				
				break;
			default:
				assert(!"Unsupported texture target");
				break;
			}
		}

		void bind()
		{
			glBindTexture(this->target, this->textureId);
			glCheckError();
		}

		void setCubemapAlignment(const CubemapAlignment& alignment)
		{
			this->cubemapAlignment = alignment;
		}

		void setWrappingS(Wrapping wrap)
		{
			this->bind();
			glTexParameteri(this->target, GL_TEXTURE_WRAP_S, translateWrapping(wrap));
		}

		void setWrappingT(Wrapping wrap)
		{
			this->bind();
			glTexParameteri(this->target, GL_TEXTURE_WRAP_T, translateWrapping(wrap));
		}

		void setFilterMin(FilteringMin filter)
		{
			this->bind();
			glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, translateFilteringMin(filter));
		}

		void setFilterMag(FilteringMag filter)
		{
			this->bind();
			glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, translateFilteringMag(filter));
		}

	private:
		void setData2D(const Image& image)
		{
			GLenum internalFormat = translateInternalFormat(image.getInfoMode(), image.getInternalFormat());
			glTexStorage2D(this->target, image.getLevels(), internalFormat, image.getWidth(), image.getHeight());
			glCheckError();
			for (unsigned int level = 0; level < image.getLevels(); level++)
			{
				auto width = image.getWidth(level);
				auto height = image.getHeight(level);
				if (image.isCompressed())
					glCompressedTexSubImage2D(this->target, level, 0, 0, width, height, internalFormat, image.getSize(0, 0, level), image.getBytesPtr(0, 0, level));
				else
					glTexSubImage2D(this->target, level, 0, 0, width, height, GL_RGBA, translateDataType(image.getInfoMode(), image.getDataType()), image.getBytesPtr(0, 0, level));
				glCheckError();
			}
		}

		void setData3D(const Image& image)
		{
			GLenum internalFormat = translateInternalFormat(image.getInfoMode(), image.getInternalFormat());
			glTexStorage3D(this->target, image.getLevels(), internalFormat, image.getWidth(), image.getHeight(), image.getLayers());
			glCheckError();
			for (unsigned int level = 0; level < image.getLevels(); level++)
			{
				auto width = image.getWidth(level);
				auto height = image.getHeight(level);
				auto depth = image.getDepth(level);
				if (image.isCompressed())
					glCompressedTexSubImage3D(this->target, level, 0, 0, 0, width, height, depth, internalFormat, image.getSize(0, 0, level), image.getBytesPtr(0, 0, level));
				else
					glTexSubImage3D(this->target, level, 0, 0, 0, width, height, depth, internalFormat, translateDataType(image.getInfoMode(), image.getDataType()), image.getBytesPtr(0, 0, level));
				glCheckError();
			}
		}

		void setDataCube(const Image& image)
		{
			GLenum internalFormat = translateInternalFormat(image.getInfoMode(), image.getInternalFormat());
			glTexStorage2D(this->target, image.getLevels(), internalFormat, image.getWidth(), image.getHeight());
			glCheckError();
			for (unsigned int face = 0; face < image.getFaces(); face++)
			{
				auto target = translateCubeFace(this->cubemapAlignment.at(face));
				for (unsigned int level = 0; level < image.getLevels(); level++)
				{
					auto width = image.getWidth(level);
					auto height = image.getHeight(level);
					if (image.isCompressed())
						glCompressedTexSubImage2D(target, level, 0, 0, width, height, internalFormat, image.getSize(0, face, level), image.getBytesPtr(0, face, level));
					else
						glTexSubImage2D(
							target, 
							level, 
							0, 0, 
							width, height, 
							translateFormat(image.getInfoMode(), image.getFormat()), 
							translateDataType(image.getInfoMode(), image.getDataType()), 
							image.getBytesPtr(0, face, level)
						);
					glCheckError();
				}
			}
		}

		GLuint textureId;
		GLenum target;
		CubemapAlignment cubemapAlignment;
	};

	// Texture

	Texture::Texture(const Image& image)
		: impl(std::make_unique<Impl>(image))
	{
	}

	void Texture::change(const Image& image)
	{
		this->impl->change(image);
	}

	void Texture::bind() const
	{
		this->impl->bind();
	}

	void Texture::setCubemapAlignment(const CubemapAlignment& alignment)
	{
		this->impl->setCubemapAlignment(alignment);
	}

	void Texture::setWrappingS(Wrapping wrap)
	{
		this->impl->setWrappingS(wrap);
	}

	void Texture::setWrappingT(Wrapping wrap)
	{
		this->impl->setWrappingT(wrap);
	}

	void Texture::setFilterMin(FilteringMin filter)
	{
		this->impl->setFilterMin(filter);
	}

	void Texture::setFilterMag(FilteringMag filter)
	{
		this->impl->setFilterMag(filter);
	}

	// Sampler impl
	
	class Sampler::Impl
	{
	public:
		Impl(GLint id)
			: id(id)
		{
		}

		void setTexture(const Texture& texture)
		{
			glActiveTexture(this->id);
			glCheckError();

			texture.bind();
		}

		int getAddress()
		{
			return this->id - GL_TEXTURE0;
		}

	private:
		GLint id;
	};

	// Sampler

	Sampler::Sampler(unsigned int samplerId)
		: impl(std::make_unique<Impl>(samplerId))
	{
	}

	void Sampler::setTexture(const Texture& texture)
	{
		this->impl->setTexture(texture);
	}

	int Sampler::getAddress()
	{
		return this->impl->getAddress();
	}

	// Model data

	class OpenglModelData : public ModelRenderData
	{
	public:
		OpenglModelData(std::shared_ptr<Context> context)
			: context(std::move(context))
		{
			glGenBuffers(1, &this->vertexBuffer);
			glGenBuffers(1, &this->uvBuffer);
			glGenBuffers(1, &this->normalBuffer);
			glGenBuffers(1, &this->indexBuffer);
			glCheckError();
		}

		~OpenglModelData()
		{
			glCheckError();
			glDeleteBuffers(1, &this->indexBuffer);
			glDeleteBuffers(1, &this->normalBuffer);
			glDeleteBuffers(1, &this->uvBuffer);
			glDeleteBuffers(1, &this->vertexBuffer);
			glCheckError();
		}

		void update(
			const std::vector<glm::vec3>& vertices, 
			const std::vector<glm::vec2>& uvs,
			const std::vector<glm::vec3>& normals,
			const std::vector<unsigned int>& indices
		) override
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
			if (vertices.size() == 0)
				glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
			glCheckError();

			glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
			if (uvs.size() == 0)
				glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_DYNAMIC_DRAW);
			glCheckError();
			
			glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
			if (normals.size() == 0)
				glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);
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
		std::shared_ptr<Context> context; // Store it so it doesn't get deleted before model data
		GLuint vertexBuffer = -1;
		GLuint normalBuffer = -1;
		GLuint uvBuffer = -1;
		GLuint indexBuffer = -1;
	};

	// Object data

	class OpenglObjectData : public ObjectRenderData
	{
	public:
		OpenglObjectData(std::shared_ptr<Context> context)
			: context(std::move(context))
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
		std::shared_ptr<Context> context; // Store it so it doesn't get deleted before object data
		GLuint vao;
	};

	// Renderer priority

	bool RendererPriorityHandler::operator()(const Object* a, const Object* b) const
	{
		return false;
	}

	// Renderer

	Renderer::Renderer(std::shared_ptr<Context> context)
		: context(std::move(context))
		, sampler(GL_TEXTURE0)
	{
		glCheckError(); // Sanity check

		this->properties[propertyProjection] = glm::mat4(1.0f);
		this->properties[propertyOrtho] = glm::mat4(1.0f);
		this->properties[propertyView] = this->camera.getViewMatrix();
		this->properties[propertyGlobalLightColor] = glm::vec3(0.7f);
		this->properties[propertyLightColor] = this->light.getColor();
		this->properties[propertyLightDirection] = this->light.getDirection();
		this->properties[propertyTexColor] = this->sampler.getAddress();
	}

	Renderer::~Renderer()
	{
		glCheckError(); // Sanity check
	}

	void Renderer::setImage(const Image& image)
	{
		this->texture = std::make_unique<Texture>(image);
		this->updateRequired = true;
	}

	bool Renderer::loadShaders(const std::string& shadersDir)
	{
		bool result = true;
		for (const auto& entry : std::filesystem::directory_iterator(shadersDir))
		{
			const auto& path = entry.path();
			if (path.extension() == ".sdr")
				result &= this->loadShader(path.string());
		}
		return result;
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

		auto dir = std::filesystem::path(sdrPath).parent_path();
		std::string name = nameIt->second->toString();

		std::string vert = Shader::NoShader;
		std::string geom = Shader::NoShader;
		std::string frag = Shader::NoShader;

		auto itVert = section.find("vertex");
		if (itVert != section.end())
			vert = std::filesystem::path(dir).append(trimmed(itVert->second->toString())).string();
		
		auto itGeom = section.find("geometry");
		if (itGeom != section.end())
			geom = std::filesystem::path(dir).append(trimmed(itGeom->second->toString())).string();
		
		auto itFrag = section.find("fragment");
		if (itFrag != section.end())
			frag = std::filesystem::path(dir).append(trimmed(itFrag->second->toString())).string();

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
			const float w = static_cast<float>(this->viewportSize.x) * 0.5f;
			const float h = static_cast<float>(this->viewportSize.y) * 0.5f;

			this->properties[propertyProjection] = glm::perspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);
			this->properties[propertyOrtho] = glm::ortho(-w, w, -h, h, -20.0f, 20.0f);

			glViewport(0, 0, this->viewportSize.x, this->viewportSize.y);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glClearStencil(0);
			glCheckError();

			this->updateRequired = false;
		}

		this->properties[propertyLightColor] = this->light.getColor();
		this->properties[propertyLightDirection] = this->light.getDirection();
	}

	void Renderer::render()
	{
		this->context->setCurrent();

		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
		glCheckError();

		this->update();

		while (!this->renderQueue.empty())
		{
			Object* object = this->renderQueue.top();
			this->renderQueue.pop();

			// Get object data
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

			if (!object->isUploaded())
				this->upload(*object);

			if (object->updateRequired())
				this->update(*object);

			object->setProjectionInfo(this->viewportSize, this->nearPlane, this->farPlane);

			// Set shader
			std::string shaderName = this->displayMode.shader;
			std::string shaderOverride = object->getShaderOverride();
			if (!shaderOverride.empty())
				shaderName = shaderOverride;
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
					if (!uniform)
					{
						Logger::getLogger() << Logger::Uniq(LOGGER_UNIQUE_ID()) << "Shader " << shaderName << ": Cannot find uniform name " << name;
						continue;
					}
					std::visit(uniform->getSetter(), prop);
				}
			}

			// Render
			auto renderMode = translateRenderMode(object->getRenderMode());
			auto data = static_cast<OpenglObjectData*>(object->getData());
			glBindVertexArray(data->getVao());
			glCheckError();

			this->sampler.setTexture(*this->texture);
			glDrawElements(renderMode, model->getIndices().size(), GL_UNSIGNED_INT, 0);
			glCheckError();

			glBindVertexArray(0); // Or else it crashes
			glCheckError();
		}

		glFlush();
		glCheckError();

		this->context->swapBuffers();
		glCheckError();
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

	void Renderer::upload(Model& model) const
	{
		auto data = std::make_unique<OpenglModelData>(this->context);
		data->update(model.getVertices(), model.getUVs(), model.getNormals(), model.getIndices());
		model.setData(std::move(data));
		model.clearUpdateStatus();
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
		gldata->update(model.getVertices(), model.getUVs(), model.getNormals(), model.getIndices());
		model.clearUpdateStatus();
	}

	void Renderer::upload(Object& object) const
	{
		auto model = object.getModel();
		if (!model)
		{
			assert(!"Object has no model");
			return;
		}

		auto data = std::make_unique<OpenglObjectData>(this->context);
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

	glm::vec3 Renderer::getCameraDirection() const
	{
		return this->camera.getDirection();
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

	void Renderer::setDisplayMode(const DisplayMode& mode)
	{
		this->displayMode = mode;
		this->update();
	}

	void Renderer::setWrappingS(Wrapping wrap)
	{
		this->texture->setWrappingS(wrap);
	}

	void Renderer::setWrappingT(Wrapping wrap)
	{
		this->texture->setWrappingT(wrap);
	}

	void Renderer::setFilterMin(FilteringMin filter)
	{
		this->texture->setFilterMin(filter);
	}

	void Renderer::setFilterMag(FilteringMag filter)
	{
		this->texture->setFilterMag(filter);
	}

	void Renderer::setShowWireframe(bool show)
	{

	}
}