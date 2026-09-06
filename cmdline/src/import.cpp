#include "cmdline/import.hpp"
#include "texture/textureloader.hpp"

namespace textoolkit::cmdline
{
	Import::Import(Texture& texture, const std::string& path, unsigned int layer, unsigned int face, unsigned int level, InterpolationMinMag interpolation)
		: texture(texture)
		, path(path)
		, layer(layer)
		, face(face)
		, level(level)
		, interpolation(interpolation)
	{
	}

	CmdResult Import::operator()()
	{
		TextureLoader loader;

		auto sourceTexture = loader.loadTexture(this->path);
		if (!sourceTexture)
			return CmdResult(CmdResult::Code::Failure, "Unable to open imported texture");

		auto source = SubTexture::createLevel(*sourceTexture, 0, 0, 0);
		auto target = SubTexture::createLevel(this->texture, this->layer, this->face, this->level);
		
		target.set(source, this->interpolation);

		return "Import successful";
	}
}