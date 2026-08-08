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

	private:
		void updateModelMatrix();

		glm::vec3 direction;
	};
}

#endif