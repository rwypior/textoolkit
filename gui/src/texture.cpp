#include "gui/texture.hpp"

#include <wx/dcmemory.h>
#include <wx/rawbmp.h>

#include <cassert>

namespace textoolkit
{
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

	SubTexture::SubTexture(Type type, unsigned int index, std::shared_ptr<Image> image, const std::string& name)
		: Texture(std::move(image), name)
		, type(type)
		, index(index)
	{
	}

	SubTexture::SubTexture(Type type, unsigned int index, std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: Texture(std::move(image), path, name)
		, type(type)
		, index(index)
	{
	}

	SubTexture SubTexture::createLayer(Texture& texture, unsigned int layer)
	{
		return SubTexture(Type::Layer, layer, texture.image, texture.name);
	}

	SubTexture SubTexture::createFace(Texture& texture, unsigned int face)
	{
		return SubTexture(Type::Face, face, texture.image, texture.name);
	}

	SubTexture SubTexture::createLevel(Texture& texture, unsigned int level)
	{
		return SubTexture(Type::Level, level, texture.image, texture.name);
	}

	glm::uvec2 SubTexture::getSize() const
	{
		return glm::uvec2(
			this->image->getWidth(this->index),
			this->image->getHeight(this->index)
		);
	}

	void SubTexture::updateBitmap()
	{
		if (!this->image)
			return;

		auto bitmapsize = this->bitmap.GetSize();
		if (bitmapsize.x != this->image->getWidth() || bitmapsize.y != this->image->getHeight())
			this->bitmap = wxBitmap(this->image->getWidth(), this->image->getHeight(), 32);

		switch (this->type)
		{
		case Type::Base:
			this->setBitmapData(this->bitmap);
			break;
		case Type::Face:
			this->setBitmapData(this->bitmap, 0, this->index, 0);
			break;
		case Type::Layer:
			this->setBitmapData(this->bitmap, this->index, 0, 0);
			break;
		case Type::Level:
			this->setBitmapData(this->bitmap, 0, 0, this->index);
			break;
		}
	}

	SubTexture::Type SubTexture::getType() const
	{
		return this->type;
	}

	unsigned int SubTexture::getIndex() const
	{
		return this->index;
	}
}