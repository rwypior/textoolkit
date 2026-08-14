#include "gui/texture.hpp"

#include <wx/dcmemory.h>
#include <wx/rawbmp.h>

#include <cassert>

namespace textoolkit
{
	Texture::Texture() = default;

	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path("")
	{
		this->updateBitmap();
	}

	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path(path)
	{
		this->updateBitmap();
	}

	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name, NoUpdateTag)
		: image(std::move(image))
		, name(name)
		, path(path)
	{
	}

	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& name, NoUpdateTag)
		: image(std::move(image))
		, name(name)
		, path("")
	{
	}

	Texture::~Texture() = default;

	std::string Texture::getName() const
	{
		return this->name;
	}

	void Texture::setName(const std::string& name)
	{
		this->name = name;
	}

	std::string Texture::getPath() const
	{
		return this->path;
	}

	Image::Type Texture::getType() const
	{
		assert(this->image);
		return this->image->getType();
	}

	Image& Texture::getImage()
	{
		assert(this->image);
		return *this->image;
	}

	wxBitmap& Texture::getBitmap()
	{
		return this->bitmap;
	}

	bool Texture::save(const std::string& path)
	{
		if (!this->image)
			return false;

		return this->image->save(path);
	}

	void Texture::updateBitmap()
	{
		if (!this->image)
			return;

		auto bitmapsize = this->bitmap.GetSize();
		if (bitmapsize.x != this->image->getWidth() || bitmapsize.y != this->image->getHeight())
			this->bitmap = wxBitmap(this->image->getWidth(), this->image->getHeight(), 32);

		this->setBitmapData(this->bitmap);
	}

	void Texture::setBitmapData(wxBitmap& bmp, unsigned int layer, unsigned int face, unsigned int level)
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

	void Texture::commit()
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

	bool Texture::commitAndSave(const std::string& path)
	{
		this->commit();
		return this->save(path);
	}

	// Sub texture

	SubTexture::SubTexture() = default;

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name)
		: Texture(std::move(image), name, NoUpdateTag{})
		, type(type)
		, layer(layer)
		, face(face)
		, level(level)
	{
		this->updateBitmap();
	}

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: Texture(std::move(image), path, name, NoUpdateTag{})
		, type(type)
		, layer(layer)
		, face(face)
		, level(level)
	{
		this->updateBitmap();
	}

	SubTexture SubTexture::createLayer(Texture& texture, unsigned int layer)
	{
		return SubTexture(Type::Layer, layer, 0, 0, texture.image, texture.name);
	}

	SubTexture SubTexture::createFace(Texture& texture, unsigned int layer, unsigned int face)
	{
		return SubTexture(Type::Face, layer, face, 0, texture.image, texture.name);
	}

	SubTexture SubTexture::createLevel(Texture& texture, unsigned int layer, unsigned int face, unsigned int level)
	{
		return SubTexture(Type::Level, layer, face, level, texture.image, texture.name);
	}

	glm::uvec2 SubTexture::getSize() const
	{
		return glm::uvec2(
			this->image->getWidth(this->level),
			this->image->getHeight(this->level)
		);
	}

	void SubTexture::updateBitmap()
	{
		if (!this->image)
			return;

		auto bitmapsize = this->bitmap.GetSize();
		if (bitmapsize.x != this->image->getWidth(this->level) || bitmapsize.y != this->image->getHeight())
			this->bitmap = wxBitmap(this->image->getWidth(this->level), this->image->getHeight(this->level), 32);

		this->setBitmapData(this->bitmap, this->layer, this->face, this->level);
	}

	SubTexture::Type SubTexture::getType() const
	{
		return this->type;
	}

	unsigned int SubTexture::getLayer() const
	{
		return this->layer;
	}

	unsigned int SubTexture::getFace() const
	{
		return this->face;
	}

	unsigned int SubTexture::getLevel() const
	{
		return this->level;
	}
}