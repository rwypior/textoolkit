#include "cmdline/save.hpp"
#include "texture/texture.hpp"
#include "texture/textureloader.hpp"
#include <iostream>
namespace textoolkit::cmdline
{
	Save::Save(Texture& texture, const std::string& path)
		: texture(texture)
		, path(path)
	{
	}

	CmdResult Save::operator()()
	{
		bool result = this->texture.save(this->path);
		
		if (result)
			return "Texture saved to " + this->path;

		return CmdResult(CmdResult::Code::FileInaccessible, "Unable to save the texture");
	}
}