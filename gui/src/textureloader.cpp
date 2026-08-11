#include "gui/textureloader.hpp"
#include "gui/texture.hpp"
#include "bmp/bmp.hpp"
#include "dds/dds.hpp"

#include <filesystem>
#include <algorithm>

namespace textoolkit
{
	TextureLoader::TextureLoader()
	{
		this->descriptors.emplace_back("Bitmap", "Bitmap (*.bmp)|*.bmp");
		this->descriptors.emplace_back("Direct draw surface", "Direct draw surface (*.dds)|*.dds");
		this->descriptors.emplace_back("All images", "All images(*.bmp;*.dds)|*.bmp;*.dds");
	}

	std::unique_ptr<Texture> TextureLoader::loadTexture(const std::string& path) const
	{
		std::filesystem::path syspath = path;
		std::string ext = syspath.extension().string();
		std::string name = syspath.filename().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), [](char c) { return std::tolower(c); });

		if (ext == ".bmp")
			return this->loadBmp(path, name);
		else if (ext == ".dds")
			return this->loadDds(path, name);

		return nullptr;
	}

	std::unique_ptr<Texture> TextureLoader::loadBmp(const std::string& path, const std::string& name) const
	{
		auto res = Bmp::load(path);
		auto bmp = std::make_unique<Bmp>(std::move(*res.data));
		return std::make_unique<Texture>(std::move(bmp), path, name);
	}

	std::unique_ptr<Texture> TextureLoader::loadDds(const std::string& path, const std::string& name) const
	{
		auto res = DDS::load(path);
		auto dds = std::make_unique<DDS>(std::move(*res.data));
		return std::make_unique<Texture>(std::move(dds), path, name);
	}

	std::string TextureLoader::getWildcardString() const
	{
		std::string result;
		for (auto& description : this->descriptors)
		{
			result += description.wildcard + "|";
		}
		if (!result.empty())
			result.pop_back();
		return result;
	}

	unsigned int TextureLoader::getFilterCount() const
	{
		return this->descriptors.size();
	}

	unsigned int TextureLoader::getFilterIndexAll() const
	{
		return this->descriptors.size() - 1;
	}
}