#ifndef _h_textoolkit_gui_texture
#define _h_textoolkit_gui_texture

#include "common/image.hpp"

#include <glm/vec2.hpp>

#include <wx/bitmap.h>

#include <memory>
#include <string>

namespace textoolkit
{
	class Texture
	{
		friend class SubTexture;

	public:
		Texture();
		Texture(std::shared_ptr<Image>&& image, const std::string& name);
		Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name);
		virtual ~Texture();

		std::string getName() const;
		void setName(const std::string& name);
		std::string getPath() const;
		Image::Type getType() const;

		Image& getImage();
		wxBitmap& getBitmap();

		virtual bool save(const std::string& path); // Save current image to file
		virtual void updateBitmap(); // Update wxBitmap with data from texture
		virtual void commit(); // Copy data from bitmap to image
		virtual bool commitAndSave(const std::string& path);

	protected:
		using NoUpdateTag = std::nullptr_t;
		Texture(std::shared_ptr<Image>&& image, const std::string& name, NoUpdateTag);
		Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name, NoUpdateTag);

		void setBitmapData(wxBitmap& bmp, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);

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
			Base,
			Layer,
			Face,
			Level
		};

	public:
		SubTexture();
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name);
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name);

		static SubTexture createLayer(Texture& texture, unsigned int layer);
		static SubTexture createFace(Texture& texture, unsigned int layer, unsigned int face);
		static SubTexture createLevel(Texture& texture, unsigned int layer, unsigned int face, unsigned int level);

		glm::uvec2 getSize() const;

		virtual void updateBitmap() override;

		Type getType() const;
		unsigned int getLayer() const;
		unsigned int getFace() const;
		unsigned int getLevel() const;

	private:
		Type type = Type::Face;
		unsigned int layer = 0;
		unsigned int face = 0;
		unsigned int level = 0;
	};
}

#endif