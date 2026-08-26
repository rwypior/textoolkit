#include "cmdline/import.hpp"
#include "texture/texture.hpp"
#include "texture/textureloader.hpp"

namespace textoolkit::cmdline
{
	Import::Import(Texture& texture, const std::string& path, unsigned int layer, unsigned int face, unsigned int level)
		: texture(texture)
		, path(path)
		, layer(layer)
		, face(face)
		, level(level)
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
		
		target.set(source);

		return "Import successful";
	}
}