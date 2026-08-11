#ifndef _h_textoolkit_gui_texture
#define _h_textoolkit_gui_texture

#include "common/image.hpp"

#include <wx/bitmap.h>

#include <memory>
#include <string>

namespace textoolkit
{
	class Texture
	{
	public:
		Texture(std::shared_ptr<Image>&& image, const std::string& name);
		Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name);
		~Texture();

		std::string getName() const;
		void setName(const std::string& name);
		std::string getPath() const;
		Image::Type getType() const;

		Image& getImage();
		wxBitmap& getBitmap();

		bool save(const std::string& path); // Save current image to file
		void updateBitmap(); // Update wxBitmap with data from texture
		void commit(); // Copy data from bitmap to image
		bool commitAndSave(const std::string& path);

	private:
		std::shared_ptr<Image> image;
		std::string name;
		std::string path;
		wxBitmap bitmap;
	};

	class SubTexture : public Texture
	{
	public:
		enum class Type
		{
			Face,
			Layer
		};

	public:
		SubTexture(Type type, unsigned int index, std::shared_ptr<Image>&& image, const std::string& name);
		SubTexture(Type type, unsigned int index, std::shared_ptr<Image>&& image, const std::string& path, const std::string& name);

		Type getType() const;
		unsigned int getIndex() const;

	private:
		Type type = Type::Face;
		unsigned int index = 0;
	};
}

#endif