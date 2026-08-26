#include "texture/texture.hpp"
#include "texture/accessor.hpp"

namespace textoolkit
{
	// Texture

	Texture::Texture() = default;

	Texture::Texture(Texture&& texture) noexcept
		: image(std::move(texture.image))
		, name(std::move(texture.name))
		, path(std::move(texture.path))
	{
	}

	Texture::Texture(std::shared_ptr<Image> image, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path("")
	{
	}

	Texture::Texture(std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: image(std::move(image))
		, name(name)
		, path(path)
	{
	}

	Texture::~Texture() = default;

	Texture& Texture::operator=(Texture&& texture) noexcept
	{
		this->image = std::move(texture.image);
		this->name = std::move(texture.name);
		this->path = std::move(texture.path);
		return *this;
	}

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

	const Image& Texture::getImage() const
	{
		assert(this->image);
		return *this->image;
	}

	bool Texture::hasImage() const
	{
		return !!this->image;
	}

	bool Texture::save(const std::string& path)
	{
		if (!this->image)
			return false;

		this->path = path;
		return this->image->save(path);
	}

	// Subtexture

	SubTexture::SubTexture()
		: SubTexture(std::make_unique<Texture>())
	{
	}

	SubTexture::SubTexture(SubTexture&& subtexture) noexcept
		: type(std::move(subtexture.type))
		, layer(std::move(subtexture.layer))
		, face(std::move(subtexture.face))
		, level(std::move(subtexture.level))
		, base(std::move(subtexture.base))
	{
	}

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name)
		: SubTexture(type, layer, face, level, image, name, std::make_unique<Texture>(image, name))
	{
	}

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name)
		: SubTexture(type, layer, face, level, image, path, name, std::make_unique<Texture>(image, name))
	{
	}

	SubTexture::SubTexture(std::unique_ptr<Texture>&& base)
		: base(std::move(base))
	{
	}

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name, std::unique_ptr<Texture>&& base)
		: type(type)
		, layer(layer)
		, face(face)
		, level(level)
		, base(std::move(base))
	{
	}

	SubTexture::SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name, std::unique_ptr<Texture>&& base)
		: type(type)
		, layer(layer)
		, face(face)
		, level(level)
		, base(std::move(base))
	{
	}

	SubTexture::~SubTexture() = default;

	SubTexture& SubTexture::operator=(SubTexture&& subtexture) noexcept
	{
		this->type = std::move(subtexture.type);
		this->layer = std::move(subtexture.layer);
		this->face = std::move(subtexture.face);
		this->level = std::move(subtexture.level);
		this->base = std::move(subtexture.base);
		return *this;
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

	Image& SubTexture::getImage()
	{
		return this->base->getImage();
	}

	bool SubTexture::hasImage() const
	{
		return this->base->hasImage();
	}

	void SubTexture::set(const SubTexture& texture)
	{
		std::unique_ptr<PixelAccessor> access;
		if (texture.getSize() == this->getSize())
			access = std::make_unique<SimpleAccessor>(*texture.base->image, this->layer, this->face, this->level);
		else
			access = std::make_unique<BicubicAccessor>(*texture.base->image, this->getSize().x, this->getSize().y, this->layer, this->face, this->level);

		if (pickDataOption(this->base->image->getStorageMode(), texture.base->image->getStorageMode()) == DataOption::InvertY)
			access->setSubAccessor(access->makeSubaccessor<InvertYAccessor>());

		for (unsigned int x = 0; x < this->getSize().x; x++)
		{
			for (unsigned int y = 0; y < this->getSize().y; y++)
			{
				this->base->image->setPixel(access->getPixel(x, y), x, y, this->layer, this->face, this->level);
			}
		}
	}

	glm::uvec2 SubTexture::getSize() const
	{
		return glm::uvec2(
			this->base->image->getWidth(this->level),
			this->base->image->getHeight(this->level)
		);
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