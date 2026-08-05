#ifndef _h_textoolkit_common_filler
#define _h_textoolkit_common_filler

#include "pixel.hpp"

#include <glm/vec2.hpp>

#include <memory>

namespace textoolkit
{
	class Filler
	{
	public:
		virtual ~Filler() = default;
		virtual Pixel getColor(const glm::uvec2& point) const = 0;
	};

	class SolidFiller : public Filler
	{
	public:
		SolidFiller(const Pixel& color);

		static std::shared_ptr<SolidFiller> create(const Pixel& color);

		virtual Pixel getColor(const glm::uvec2& point) const override;

	private:
		Pixel color;
	};
}

#endif