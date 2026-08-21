#include "renderer/object.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace textoolkit::renderer
{
	Object::Object(const std::string& name)
		: name(name)
	{
		this->properties[propertyLocation] = glm::vec3(0.0f);
		this->properties[propertyModelMatrix] = glm::mat4(1.0f);
		this->properties[propertyLightingEnabled] = true;
	}

	Object::~Object() = default;

	const std::string& Object::getName() const
	{
		return this->name;
	}

	ObjectRenderData* Object::getData() const
	{
		return this->data.get();
	}

	void Object::setData(std::unique_ptr<ObjectRenderData>&& data)
	{
		this->data = std::move(data);
	}

	Model* Object::getModel() const
	{
		return this->model.get();
	}

	void Object::setModel(std::shared_ptr<Model>&& model)
	{
		this->model = std::move(model);
		this->modelChanged();
		this->needsUpdate = true;
	}

	glm::vec3 Object::getLocation() const
	{
		return std::get<glm::vec3>(this->properties.at(propertyLocation));
	}

	void Object::setLocation(const glm::vec3 location)
	{
		this->properties[propertyLocation] = location;
		this->updateModelMatrix();
		this->locationChanged();
	}

	const std::map<std::string, RenderProperty>& Object::getProperties() const
	{
		return this->properties;
	}

	void Object::setProperty(const std::string& name, RenderProperty&& property)
	{
		this->properties[name] = std::move(property);
	}

	RenderMode Object::getRenderMode()
	{
		return this->renderMode;
	}

	void Object::setRenderMode(RenderMode mode)
	{
		this->renderMode = mode;
	}

	bool Object::isUploaded() const
	{
		return !!this->data;
	}

	bool Object::updateRequired() const
	{
		return this->needsUpdate;
	}

	void Object::updateModelMatrix()
	{
		glm::mat4 model(1.0f);
		glm::translate(model, this->getProperty<glm::vec3>(propertyLocation));
		this->setProperty(propertyModelMatrix, model);
	}
}