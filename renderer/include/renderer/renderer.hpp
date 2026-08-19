#ifndef _h_textoolkit_renderer_renderer
#define _h_textoolkit_renderer_renderer

#include "renderer/api.hpp"
#include "renderer/camera.hpp"
#include "renderer/light.hpp"

#include <events/events.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <cassert>
#include <variant>

namespace textoolkit::renderer
{
	class Model;
	class Object;

	class UniformData
	{
	public:
		~UniformData() = default;
	};

	class Uniform
	{
		friend class Shader;

		class Impl;

	public:
		class Visitor
		{
		public:
			Visitor(Uniform& uniform)
				: uniform(uniform)
			{
			}

			template<typename T>
			void operator()(const T&)
			{
				assert(!"Unsupported type");
			}

			template<>
			void operator()(const float& val)
			{
				uniform.set(val);
			}

			template<>
			void operator()(const bool& val)
			{
				uniform.set(val);
			}

			template<>
			void operator()(const glm::vec3& vec)
			{
				uniform.set(vec);
			}

			template<>
			void operator()(const glm::mat3& vec)
			{
				uniform.set(vec);
			}

			template<>
			void operator()(const glm::mat4& vec)
			{
				uniform.set(vec);
			}

		private:
			Uniform& uniform;
		};

	public:
		Uniform(const UniformData& data);

		void set(float val);
		void set(bool val);
		void set(const glm::vec3& vec);
		void set(const glm::mat3& mtx);
		void set(const glm::mat4& mtx);
		Visitor getSetter();

	private:
		std::unique_ptr<Impl> impl;
	};

	class Shader
	{
		class Impl;

	public:
		static constexpr char NoShader[] = "";

		enum class ShaderTypes
		{
			Vertex = 0x01,
			Fragment = 0x02,
			Geometry = 0x04
		};

	public:
		Shader();
		Shader(Shader&& shader);
		Shader(
			const std::string& vert,
			const std::string& frag,
			const std::string& geom = NoShader
		);
		~Shader();

		Shader& operator=(Shader&& shader);

		void use();

		bool hasUniform(const std::string& name) const;
		Uniform* getUniform(const std::string& name) const;

	private:
		std::unique_ptr<Impl> impl;
	};

	class RendererPriorityHandler
	{
	public:
		bool operator()(const Object* a, const Object* b) const;
	};

	class Renderer
	{
		event_binding_container;

	public:
		static constexpr char propertyView[] = "viewmatrix";
		static constexpr char propertyProjection[] = "projectionmatrix";
		static constexpr char propertyPerspective[] = "perspectivematrix";
		static constexpr char propertyOrtho[] = "orthomatrix";
		static constexpr char propertyGlobalLightColor[] = "globallightcolor";
		static constexpr char propertyLightColor[] = "lightcolor";
		static constexpr char propertyLightDirection[] = "lightdirection";

	public:
		Event<Renderer&> rendererReady;

		Renderer(std::shared_ptr<Context> context);
		~Renderer();

		bool loadShaders(const std::string& shadersDir);
		bool loadShader(const std::string& sdrPath);

		void render();
		Shader* getShader(const std::string& name);

		void enqueue(Object* object);

		void upload(Model& model) const;
		void update(Model& model) const;

		void upload(Object& object) const;
		void update(Object& object) const;

		void setFieldOfView(float fov);
		void setNearPlane(float near);
		void setFarPlane(float far);
		void setViewportSize(const glm::uvec2& wh);

		glm::vec3 getCameraPosition() const;
		void setCameraPosition(const glm::vec3& position);
		glm::vec3 getCameraLookTarget() const;
		void setCameraLookTarget(const glm::vec3& lookTarget);
		glm::vec3 getCameraUpVector() const;
		void setCameraUpVector(const glm::vec3& up);

		glm::vec3 getLightColor() const;
		void setLightColor(const glm::vec3& color);
		glm::vec3 getLightDirection() const;
		void setLightDirection(const glm::vec3& direction);

	private:
		std::shared_ptr<Context> context;
		std::priority_queue<Object*, std::vector<Object*>, RendererPriorityHandler> renderQueue;
		std::unordered_map<std::string, Shader> shaders;
		RenderProperties properties{};
		Camera camera;
		Light light;

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float aspectRatio = 1.3f;
		glm::uvec2 viewportSize = {800, 600};

		bool updateRequired = true;

		void update();
	};
}

#endif