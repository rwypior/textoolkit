#include "gui/texture.hpp"
#include "texture/accessor.hpp"

#include <wx/dcmemory.h>
#include <wx/rawbmp.h>

#include <cassert>

namespace textoolkit
{
	// Gui texture

	GuiTexture::GuiTexture()
		: Texture()
	{
	}

	GuiTexture::GuiTexture(Texture&& texture) noexcept
		: Texture(std::move(texture))
	{
	}

	GuiTexture::GuiTexture(GuiTexture&& texture) noexcept
		: Texture(std::move(texture))
		, bitmap(std::move(texture.bitmap))
	{
	}

	GuiTexture::GuiTexture(std::shared_ptr<Image> image, const std::string& name)
		: Texture(std::move(image), name)
	{
		this->updateBitmap();
	}

	GuiTexture::GuiTexture(std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: Texture(std::move(image), path, name)
	{
		this->updateBitmap();
	}

	GuiTexture::GuiTexture(std::shared_ptr<Image> image, const std::string& name, NoUpdateTag)
		: Texture(std::move(image), name)
	{
	}

	GuiTexture::GuiTexture(std::shared_ptr<Image> image, const std::string& path, const std::string& name, NoUpdateTag)
		: Texture(std::move(image), path, name)
	{
	}

	wxBitmap& GuiTexture::getBitmap()
	{
		return this->bitmap;
	}

	void GuiTexture::updateBitmap()
	{
		if (!this->image)
			return;

		auto bitmapsize = this->bitmap.GetSize();
		if (bitmapsize.x != this->image->getWidth() || bitmapsize.y != this->image->getHeight())
			this->bitmap = wxBitmap(this->image->getWidth(), this->image->getHeight(), 32);

		this->setBitmapData(this->bitmap);
	}

	void GuiTexture::setBitmapData(wxBitmap& bmp, unsigned int layer, unsigned int face, unsigned int level)
	{
		wxAlphaPixelData data(bmp);
		wxAlphaPixelData::Iterator datait(data);

		for (unsigned int x = 0; x < bmp.GetWidth(); x++)
		{
			for (unsigned int y = 0; y < bmp.GetHeight(); y++)
			{
				auto color = this->image->getPixel(x, y, layer, face, level, textoolkit::DataOption::InvertY);

				datait.MoveTo(data, x, y);
				datait.Red() = color->r;
				datait.Green() = color->g;
				datait.Blue() = color->b;
				datait.Alpha() = color->a;
			}
		}
	}

	void GuiTexture::commit()
	{
		if (!this->image)
			return;

		wxMemoryDC dc;
		dc.SelectObject(this->bitmap);
		for (unsigned int x = 0; x < this->bitmap.GetWidth(); x++)
		{
			for (unsigned int y = 0; y < this->bitmap.GetHeight(); y++)
			{
				wxColour pixel;
				if (dc.GetPixel(x, y, &pixel))
				{
					this->image->setPixel(Pixel(pixel.Red(), pixel.Green(), pixel.Red()), x, y);
				}
			}
		}
		dc.SelectObject(wxNullBitmap);
	}

	bool GuiTexture::commitAndSave(const std::string& path)
	{
		this->commit();
		return this->save(path);
	}

	// GUI subtexture

	GuiSubTexture::GuiSubTexture()
		: SubTexture(std::make_unique<GuiTexture>())
	{
	}

	GuiSubTexture::GuiSubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name)
		: SubTexture(type, layer, face, level, image, name, std::make_unique<GuiTexture>(image, name, GuiTexture::NoUpdateTag()))
	{
		this->updateBitmap();
	}

	GuiSubTexture::GuiSubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: SubTexture(type, layer, face, level, image, path, name, std::make_unique<GuiTexture>(image, path, name, GuiTexture::NoUpdateTag()))
	{
		this->updateBitmap();
	}

	GuiSubTexture::GuiSubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image)
		: SubTexture(type, layer, face, level, image, "", "", std::make_unique<GuiTexture>(image, "", "", GuiTexture::NoUpdateTag()))
	{
	}

	GuiSubTexture GuiSubTexture::createLayer(GuiTexture& texture, unsigned int layer)
	{
		return GuiSubTexture(Type::Layer, layer, 0, 0, texture.image, texture.name);
	}

	GuiSubTexture GuiSubTexture::createFace(GuiTexture& texture, unsigned int layer, unsigned int face)
	{
		return GuiSubTexture(Type::Face, layer, face, 0, texture.image, texture.name);
	}

	GuiSubTexture GuiSubTexture::createLevel(GuiTexture& texture, unsigned int layer, unsigned int face, unsigned int level)
	{
		return GuiSubTexture(Type::Level, layer, face, level, texture.image, texture.name);
	}

	GuiSubTexture GuiSubTexture::createInternalLayer(GuiTexture& texture, unsigned int layer)
	{
		return GuiSubTexture(Type::Layer, layer, 0, 0, texture.image);
	}

	GuiSubTexture GuiSubTexture::createInternalFace(GuiTexture& texture, unsigned int layer, unsigned int face)
	{
		return GuiSubTexture(Type::Face, layer, face, 0, texture.image);
	}

	GuiSubTexture GuiSubTexture::createInternalLevel(GuiTexture& texture, unsigned int layer, unsigned int face, unsigned int level)
	{
		return GuiSubTexture(Type::Level, layer, face, level, texture.image);
	}

	wxBitmap& GuiSubTexture::getBitmap()
	{
		auto& base = *static_cast<GuiTexture*>(this->base.get());
		return base.getBitmap();
	}

	void GuiSubTexture::updateBitmap()
	{
		auto& base = *static_cast<GuiTexture*>(this->base.get());

		if (!base.hasImage())
			return;

		auto bitmapsize = base.bitmap.GetSize();
		if (bitmapsize.x != base.image->getWidth(this->level) || bitmapsize.y != base.image->getHeight(this->level))
			base.bitmap = wxBitmap(base.image->getWidth(this->level), base.image->getHeight(this->level), 32);

		base.setBitmapData(base.bitmap, this->layer, this->face, this->level);
	}

	void GuiSubTexture::commit()
	{
		auto& base = *static_cast<GuiTexture*>(this->base.get());
		base.commit();
	}

	bool GuiSubTexture::commitAndSave(const std::string& path)
	{
		auto& base = *static_cast<GuiTexture*>(this->base.get());
		return base.commitAndSave(path);
	}

	void GuiSubTexture::set(const SubTexture& texture)
	{
		SubTexture::set(texture);
		this->updateBitmap();
	}
}