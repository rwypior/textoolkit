#ifndef _h_textoolkit_gui_textureloader
#define _h_textoolkit_gui_textureloader

#include <string>
#include <memory>
#include <vector>

namespace textoolkit
{
	class Texture;

	class TextureLoader
	{
	public:
		struct TextureDescription
		{
			std::string name;
			std::string wildcard;

			TextureDescription(const std::string& name, const std::string& wildcard)
				: name(name)
				, wildcard(wildcard)
			{
			}
		};

	public:
		TextureLoader();

		std::unique_ptr<Texture> loadTexture(const std::string& path) const;

		std::string getWildcardString() const;
		unsigned int getFilterCount() const;
		unsigned int getFilterIndexAll() const;

	private:
		std::unique_ptr<Texture> loadBmp(const std::string& path, const std::string& name) const;
		std::unique_ptr<Texture> loadDds(const std::string& path, const std::string& name) const;

		std::vector<TextureDescription> descriptors;
	};
}

#endif