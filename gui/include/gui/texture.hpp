#ifndef _h_textoolkit_gui_texture
#define _h_textoolkit_gui_texture

#include "common/image.hpp"
#include "common/texture.hpp"

#include <glm/vec2.hpp>

#include <wx/bitmap.h>

#include <memory>
#include <string>

namespace textoolkit
{
	class GuiTexture : public Texture
	{
		friend class GuiSubTexture;

	public:
		struct NoUpdateTag {};

		GuiTexture();
		GuiTexture(Texture&& texture) noexcept;
		GuiTexture(GuiTexture&& texture) noexcept;
		GuiTexture(std::shared_ptr<Image> image, const std::string& name);
		GuiTexture(std::shared_ptr<Image> image, const std::string& path, const std::string& name);
		GuiTexture(std::shared_ptr<Image> image, const std::string& name, NoUpdateTag);
		GuiTexture(std::shared_ptr<Image> image, const std::string& path, const std::string& name, NoUpdateTag);

		wxBitmap& getBitmap();
		virtual void updateBitmap(); // Update wxBitmap with data from texture
		virtual void commit(); // Copy data from bitmap to image
		virtual bool commitAndSave(const std::string& path);

	private:
		void setBitmapData(wxBitmap& bmp, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);

		wxBitmap bitmap;
	};

	class GuiSubTexture : public SubTexture
	{
	public:
		GuiSubTexture();
		GuiSubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name);
		GuiSubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name);

		wxBitmap& getBitmap();
		virtual void updateBitmap(); // Update wxBitmap with data from texture
		virtual void commit(); // Copy data from bitmap to image
		virtual bool commitAndSave(const std::string& path);

		static GuiSubTexture createLayer(GuiTexture& texture, unsigned int layer);
		static GuiSubTexture createFace(GuiTexture& texture, unsigned int layer, unsigned int face);
		static GuiSubTexture createLevel(GuiTexture& texture, unsigned int layer, unsigned int face, unsigned int level);

		virtual void set(const SubTexture& texture) override;
	};
}

#endif