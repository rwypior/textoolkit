#include "renderer/compass.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace textoolkit::renderer
{
	Compass::Compass()
	{
		this->setLineColor(LineType::X, glm::vec3(0.7f, 0.0f, 0.0f));
		this->setLineColor(LineType::Y, glm::vec3(0.0f, 0.7f, 0.0f));
		this->setLineColor(LineType::Z, glm::vec3(0.0f, 0.0f, 0.7f));

		this->properties[propertyLightingEnabled] = false;
	}

	Compass::~Compass() = default;

	void Compass::setLineColor(LineType type, const glm::vec3& color)
	{
		glm::mat3 mtx = this->getProperty<glm::mat3>(propertyColors);
		switch (type)
		{
		case LineType::X: mtx[0] = color; break;
		case LineType::Y: mtx[1] = color; break;
		case LineType::Z: mtx[2] = color; break;
		default:
			assert(!"Invalid line type");
		}
		this->setProperty(propertyColors, mtx);
	}

	glm::vec3 Compass::getLineColor(LineType type) const
	{
		glm::mat3 mtx = this->getProperty<glm::mat3>(propertyColors);
		switch (type)
		{
		case LineType::X: return mtx[0];
		case LineType::Y: return mtx[1];
		case LineType::Z: return mtx[2];
		}
		assert(!"Invalid line type");
		return {};
	}

	void Compass::setCameraDirection(const glm::vec3& dir)
	{
		this->direction = -dir;
		this->updateModelMatrix();
	}

	void Compass::updateModelMatrix()
	{
		auto& model = std::get<glm::mat4>(this->properties[propertyModelMatrix]);
		model = glm::mat4(1.0f) * glm::toMat4(glm::quat(this->direction));
	}
}