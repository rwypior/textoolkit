#include "renderer/compass.hpp"
#include "renderer/model.hpp"
#include "common/util.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace textoolkit::renderer
{
	Compass::Compass()
		: Object("compass")
	{
		this->setLineColor(LineType::X, glm::vec3(0.7f, 0.0f, 0.0f));
		this->setLineColor(LineType::Y, glm::vec3(0.0f, 0.7f, 0.0f));
		this->setLineColor(LineType::Z, glm::vec3(0.0f, 0.4f, 0.8f));

		this->model = std::make_unique<Model>();
		this->model->setPoints({
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.0f)
		});

		this->properties[propertyLightingEnabled] = false;
		this->renderMode = RenderMode::Lines;
		this->updateModelMatrix();
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

	void Compass::setSize(float size)
	{
		this->size = size;
		this->updateModelMatrix();
	}

	void Compass::setOffset(float x, float y)
	{
		this->offsetX = x;
		this->offsetY = y;
		this->updateModelMatrix();
	}

	void Compass::setProjectionInfo(const glm::vec2& viewport, float /*nearPlane*/, float /*farPlane*/)
	{
		if (viewport == this->viewport)
			return;

		this->viewport = viewport;
		this->updateModelMatrix();
	}

	std::string Compass::getShaderOverride() const
	{
		return "compass";
	}

	void Compass::updateModelMatrix()
	{
		const float offsetX = this->viewport.x * 0.5f - this->offsetX;
		const float offsetY = this->viewport.y * 0.5f - this->offsetY;

		this->properties[propertyModelMatrix] = 
			glm::translate(glm::vec3(offsetX, offsetY, 0.0f)) *
			glm::toMat4(orient(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-this->direction.x, this->direction.y, this->direction.z))) *
			glm::scale(glm::vec3(this->size));
	}
}