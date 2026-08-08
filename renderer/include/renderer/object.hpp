#ifndef _h_textoolkit_renderer_renderobject
#define _h_textoolkit_renderer_renderobject

#include "renderer/util.hpp"

#include <events/events.h>

#include <glm/vec3.hpp>

#include <memory>
#include <variant>
#include <map>
#include <string>

namespace textoolkit::renderer
{
	class Model;

	class ObjectRenderData
	{
	public:
		virtual ~ObjectRenderData() = default;
		virtual void update(const Model& model) = 0;
	};

	class Object
	{
		event_binding_container;

	public:
		static constexpr char propertyLightingEnabled[] = "lightingenabled";
		static constexpr char propertyLocation[] = "location";
		static constexpr char propertyModelMatrix[] = "modelmatrix";

		Event<> modelChanged;
		Event<> locationChanged;

		Object();

		ObjectRenderData* getData() const;
		void setData(std::unique_ptr<ObjectRenderData>&& data);

		Model* getModel() const;
		void setModel(std::shared_ptr<Model>&& model);

		glm::vec3 getLocation() const;
		void setLocation(const glm::vec3 location);

		const std::map<std::string, RenderProperty>& getProperties() const;
		void setProperty(const std::string& name, RenderProperty&& property);
		template<typename T>
		T getProperty(const std::string& name) const
		{
			auto it = this->properties.find(name);
			if (it == this->properties.end())
				return {};
			return std::get<T>(it->second);
		}

		const std::string& getShader() const;
		void setShader(const std::string& shader);

		RenderMode getRenderMode();
		void setRenderMode(RenderMode mode);

		bool isUploaded() const;
		bool updateRequired() const;

	protected:
		std::unique_ptr<ObjectRenderData> data;
		std::shared_ptr<Model> model = nullptr;
		RenderProperties properties{};
		std::string shader = "default";
		RenderMode renderMode = RenderMode::Solid;

		bool needsUpdate = false;

		void updateModelMatrix();
	};
}

#endif