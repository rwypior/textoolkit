#include "cmdline/info.hpp"
#include "common/util.hpp"
#include "common/image.hpp"
#include "texture/texture.hpp"

#include <algorithm>
#include <sstream>

namespace textoolkit::cmdline
{
	InfoProperty translateProperty(const std::string& property)
	{
		std::string p = trimmed(property);
		std::transform(p.begin(), p.end(), p.begin(), [](char c) { return std::tolower(c); });
		if (property == "summary")
			return InfoProperty::Summary;
		if (property == "type")
			return InfoProperty::Type;
		if (property == "extents")
			return InfoProperty::Extents;
		if (property == "layers")
			return InfoProperty::Layers;
		if (property == "faces")
			return InfoProperty::Faces;
		if (property == "levels")
			return InfoProperty::Levels;
		return InfoProperty::Invalid;
	}

	Info::Info(const Texture& texture, const std::string& property)
		: texture(texture)
		, property(translateProperty(property))
	{
	}

	CmdResult Info::operator()()
	{
		if (this->property == InfoProperty::Summary)
		{
			return (std::stringstream()
				<< "Type: " << this->get(InfoProperty::Type) << "\n"
				<< "Extents: " << this->get(InfoProperty::Extents) << "\n"
				<< "Layers: " << this->get(InfoProperty::Layers) << "\n"
				<< "Faces: " << this->get(InfoProperty::Faces) << "\n"
				<< "Levels: " << this->get(InfoProperty::Levels) << "\n"
				).str();
		}
		else
			return this->get(this->property);

		return CmdResult(CmdResult::Code::Failure, "Unrecognized info property");
	}

	std::string Info::get(InfoProperty property)
	{
		switch (property)
		{
		case InfoProperty::Type:
			return Image::translateTextureType(this->texture.getImage().getTextureType());
		case InfoProperty::Extents:
			return (
				std::stringstream() <<
				this->texture.getImage().getWidth() << "px x " <<
				this->texture.getImage().getHeight() << "px x " <<
				this->texture.getImage().getDepth() << "px"
				).str();
		case InfoProperty::Layers:
			return std::to_string(this->texture.getImage().getLayers());
		case InfoProperty::Faces:
			return std::to_string(this->texture.getImage().getFaces());
		case InfoProperty::Levels:
			return std::to_string(this->texture.getImage().getLevels());
		}
		return "";
	}
}