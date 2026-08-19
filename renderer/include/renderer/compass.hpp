#ifndef _h_textoolkit_renderer_compass
#define _h_textoolkit_renderer_compass

#include "object.hpp"

namespace textoolkit::renderer
{
	class Compass : public Object
	{
	public:
		static constexpr char propertyColors[] = "colors";

	public:
		enum class LineType
		{
			X,
			Y,
			Z
		};

	public:
		Compass();
		~Compass();

		void setLineColor(LineType type, const glm::vec3& color);
		glm::vec3 getLineColor(LineType type) const;

		void setCameraDirection(const glm::vec3& dir);

		void setSize(float size);
		void setOffset(float x, float y);
		virtual void setProjectionInfo(const glm::vec2& viewport, float nearPlane, float farPlane) override;

	private:
		void updateModelMatrix();

		glm::vec3 direction = glm::vec3(0.0f);
		glm::vec2 viewport = glm::vec2(0.0f);
		float size = 50.0f;
		float offsetX = 80.0f;
		float offsetY = 80.0f;
	};
}

#endif