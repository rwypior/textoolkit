#include "gui/texture.hpp"

#include <wx/dcmemory.h>

#include <cassert>

namespace textoolkit
{
	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path("")
	{
	}

	Texture::Texture(std::shared_ptr<Image>&& image, const std::string& path, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path(path)
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
		assert(this->image);
		return this->image->save(path);
	}

	void Texture::commit()
	{
		assert(this->image);
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
	}

	bool Texture::commitAndSave(const std::string& path)
	{
		this->commit();
		return this->save(path);
	}

	// Sub texture

	SubTexture::SubTexture(Type type, unsigned int index, std::shared_ptr<Image>&& image, const std::string& name)
		: Texture(std::move(image), name)
		, type(type)
		, index(index)
	{
	}

	SubTexture::SubTexture(Type type, unsigned int index, std::shared_ptr<Image>&& image, const std::string& path, const std::string& name)
		: Texture(std::move(image), path, name)
		, type(type)
		, index(index)
	{
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