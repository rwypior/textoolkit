#ifndef _h_textoolkit_renderer_camera
#define _h_textoolkit_renderer_camera

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace textoolkit::renderer
{
	class Camera
	{
	public:
		Camera();

		bool updateRequired() const;
		void update();

		void setPosition(const glm::vec3& position);
		const glm::vec3& getPosition() const;

		void setLookTarget(const glm::vec3& lookTarget);
		const glm::vec3& getLookTarget() const;

		void setUpVector(const glm::vec3& upVector);
		const glm::vec3& getUpVector() const;

		glm::vec3 getDirection() const;

		const glm::mat4& getViewMatrix();

	private:
		bool needsRequired = true;

		glm::vec3 position;
		glm::vec3 lookTarget;
		glm::vec3 upVector;

		glm::mat4 matrix;
	};
}

#endif