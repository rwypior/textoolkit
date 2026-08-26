#include "cmdline/main.hpp"
#include "cmdline/info.hpp"
#include "cmdline/import.hpp"
#include "cmdline/save.hpp"
#include "texture/texture.hpp"
#include "texture/textureloader.hpp"
#include "bmp/bmp.hpp"
#include "dds/dds.hpp"

#include <libcmdline/cmdline.h>

#include <string>
#include <iostream>

namespace
{
	cmdline::ArgumentEnablePred enableWhenArgumentEquals(const cmdline::Argument& arg, const std::string& value)
	{
		return [&arg, value]() {
			return arg.value == value;
		};
	}

	int finalize(const textoolkit::cmdline::CmdResult& result)
	{
		if (!result)
		{
			std::cerr << *result.data << std::endl;
			for (auto& warn : result.warnings)
				std::cout << "\t" << warn.warning << std::endl;
			return 1;
		}

		if (result.data)
			std::cout << *result.data << std::endl;

		return 0;
	}
}

int main(int argc, char** argv)
{
	cmdline::Parser parser;

	// Basic operation
	auto& argNew = parser.addSwitch("new", 'n', "Create a new texture");
	auto& argPath = parser.addArgument("path", "", cmdline::Req::required, "Load a new texture at given path", cmdline::enableWhenSwitchIsUnset(argNew));

	// New texture options
	auto& grpNew = parser.addGroup("New texture", "Options for creating a new texture", cmdline::enableWhenSwitchIsSet(*parser.getSwitch("new")));
	auto& argType = grpNew.addArgument("type", "", cmdline::Req::required, "Image type");
	argType.allowedValues.insert({"bmp", "dds"});
	auto& argWidth = grpNew.addOption("width", 'w', "", cmdline::Req::required, "Image width in pixels");
	auto& argHeight = grpNew.addOption("height", 'h', "", cmdline::Req::required, "Image height in pixels");
	auto& argDepth = grpNew.addOption("depth", 'd', "1", cmdline::Req::required, "Image depth in pixels");
	auto& argBpp = grpNew.addOption("bpp", cmdline::NoAbbr, "3", cmdline::Req::required, "Image bytes per pixel");
	auto& argTextureType = grpNew.addOption("texturetype", cmdline::NoAbbr, "Texture2D", cmdline::Req::required, "Image texture type, if applicable");
	argTextureType.allowedValues.insert({"Texture2D", "Texture3D", "TextureCube"});
	auto& argInternalFormat = grpNew.addOption("internalformat", cmdline::NoAbbr, "RGB8", cmdline::Req::required, "Image internal data format, if applicable");
	argInternalFormat.allowedValues.insert({"RGB8"});
	auto& argCompression = grpNew.addOption("compression", cmdline::NoAbbr, "None", cmdline::Req::required, "Image compression method, if applicable");
	argCompression.allowedValues.insert({"None", "DXT1", "DXT3", "DXT5"});
	auto& argLayers = grpNew.addOption("layers", cmdline::NoAbbr, "1", cmdline::Req::required, "Image layers count, if applicable");
	auto& argMipmaps = grpNew.addSwitch("mipmaps", cmdline::NoAbbr, "Generate mipmaps if applicable");

	// Get info options
	auto& grpInfo = parser.addGroup("Info", "Options for getting texture informations");
	auto& argInfo = grpInfo.addOption("info", 'i', "", cmdline::Req::optional, "Get information about the texture");
	argInfo.allowedValues.insert({"summary", "type", "extents", "layers", "faces", "levels"});

	// Texture operations
	auto& grpOper = parser.addGroup("Operations", "Options for performing operations on the texture");
	auto& argOperation = parser.addArgument("operation", "", cmdline::Req::optional, "Specify an operation to perform on the texture");
	argOperation.allowedValues.insert({"import"});
	auto& argImportPath = parser.addArgument("importpath", "", cmdline::Req::required, "Path to image to be imported", cmdline::enableWhenEquals(argOperation, "import"));
	auto& argImportLayer = parser.addOption("importlayer", cmdline::NoAbbr, "", cmdline::Req::required, "Layer index to which to import an image", cmdline::enableWhenEquals(argOperation, "import"));
	auto& argImportFace = parser.addOption("importface", cmdline::NoAbbr, "", cmdline::Req::required, "Face index to which to import an image", cmdline::enableWhenEquals(argOperation, "import"));
	auto& argImportLevel = parser.addOption("importlevel", cmdline::NoAbbr, "", cmdline::Req::required, "Level index to which to import an image", cmdline::enableWhenEquals(argOperation, "import"));
	auto& argSavePath = parser.addOption("save", 's', "", cmdline::Req::optional, "Path where to save the texture");

	// Parse the stuff
	auto res = parser.parse(argc, argv);

	if (parser.getSwitch("help")->on())
	{
		std::cout << parser.getHelp(cmdline::HelpStyle::WithGroups) << std::endl;
		return 0;
	}

	if (!res)
	{
		std::cout << res.errorStr() << std::endl;
		return 1;
	}

	if (argNew && argPath)
	{
		std::cerr << "Only one of arguments \"new\" and \"path\" must be given at a time" << std::endl;
		return 1;
	}

	// Create or load the texture
	std::unique_ptr<textoolkit::Texture> texture;

	if (argPath)
	{
		textoolkit::TextureLoader loader;
		std::string path = argPath.value;
		texture = loader.loadTexture(path);
	}
	else if (argNew.on())
	{
		std::string type = argType.value;
		unsigned int width = std::stoul(argWidth.value);
		unsigned int height = std::stoul(argHeight.value);
		unsigned int depth = std::stoul(argDepth.value);
		unsigned int bpp = std::stoul(argBpp.value);
		textoolkit::Image::TextureType texturetype = textoolkit::Image::translateTextureType(argTextureType.value);
		textoolkit::Image::TextureInternalFormat internalformat = textoolkit::Image::translateInternalFormat(argInternalFormat.value);
		textoolkit::Image::CompressionType compression = textoolkit::Image::translateCompression(argCompression.value);
		unsigned int layers = std::stoul(argLayers.value);
		bool generatemipmaps = argMipmaps.on();
		std::unique_ptr<textoolkit::Image> image;
		if (type == "bmp")
			image = std::make_unique<textoolkit::Bmp>(width, height, bpp);
		else if (type == "dds")
			image = std::make_unique<textoolkit::DDS>(texturetype, internalformat, compression, glm::vec3(width, height, depth), layers, generatemipmaps);
		else
		{
			std::cerr << "Invalid image type \"" << type << "\"" << std::endl;
			return 1;
		}

		texture = std::make_unique<textoolkit::Texture>(std::move(image), "New texture");
	}

	// Info-related stuff
	if (argInfo)
	{
		std::string property = argInfo.value;
		textoolkit::cmdline::Info info(*texture, property);
		auto result = info();
		auto code = finalize(result);
		if (code != 0)
			return code;
	}

	// Operations-related stuff
	if (argOperation && argOperation.value == "import")
	{
		const auto& importpath = argImportPath.value;
		const auto& importlayer = std::stoul(argImportLayer.value);
		const auto& importface = std::stoul(argImportFace.value);
		const auto& importlevel = std::stoul(argImportLevel.value);
		textoolkit::cmdline::Import imp(*texture, importpath, importlayer, importface, importlevel);
		auto result = imp();
		auto code = finalize(result);
		if (code != 0)
			return code;
	}

	// Save texture
	if (argSavePath)
	{
		textoolkit::cmdline::Save save(*texture, argSavePath.value);
		auto result = save();
		auto code = finalize(result);
		if (code != 0)
			return code;
	}

	return 0;
}