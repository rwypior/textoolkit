#include "renderer/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace textoolkit::renderer
{
	Camera::Camera()
		: position(glm::vec3(0.0f, 1.0f, -2.0f))
		, lookTarget(0.0f)
		, upVector(0.0f, 1.0f, 0.0f)
		, matrix(1.0f)
	{
	}

	bool Camera::updateRequired() const
	{
		return this->needsRequired;
	}

	void Camera::update()
	{
		this->matrix = glm::lookAt(this->position, this->lookTarget, this->upVector);
		this->needsRequired = false;
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		this->position = position;
		this->needsRequired = true;
	}

	const glm::vec3& Camera::getPosition() const
	{
		return this->position;
	}

	void Camera::setLookTarget(const glm::vec3& lookTarget)
	{
		this->lookTarget = lookTarget;
		this->needsRequired = true;
	}

	const glm::vec3& Camera::getLookTarget() const
	{
		return this->lookTarget;
	}

	void Camera::setUpVector(const glm::vec3& upVector)
	{
		this->upVector = upVector;
		this->needsRequired = true;
	}

	glm::vec3 Camera::getDirection() const
	{
		return this->lookTarget - this->position;
	}

	const glm::vec3& Camera::getUpVector() const
	{
		return this->upVector;
	}

	const glm::mat4& Camera::getViewMatrix()
	{
		return this->matrix;
	}
}