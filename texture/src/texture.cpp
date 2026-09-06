#include "texture/texture.hpp"
#include "texture/accessor.hpp"

#include <glm/vector_relational.hpp>

namespace textoolkit
{
	// Interpolation

	std::map<Interpolation, std::string> getInterpolationMap()
	{
		return {
			{ Interpolation::Bicubic, "Bicubic" },
			{ Interpolation::NearestNeighbors, "Nearest neighbors" }
		};
	}

	std::map<std::string, Interpolation> getInterpolationMapStr()
	{
		std::map<std::string, Interpolation> result;
		for (auto [i, s] : getInterpolationMap())
		{
			result[s] = i;
		}
		return result;
	}

	std::string translateInterpolation(Interpolation interpolation)
	{
		auto m = getInterpolationMap();
		auto it = m.find(interpolation);
		if (it != m.end())
			return it->second;
		assert(!"Invalid interpolation");
		return "";
	}

	Interpolation translateInterpolation(const std::string& interpolation)
	{
		auto m = getInterpolationMapStr();
		auto it = m.find(interpolation);
		if (it != m.end())
			return it->second;
		assert(!"Invalid interpolation");
		return Interpolation::None;
	}

	InterpolationMinMag::InterpolationMinMag(Interpolation minInterpolation, Interpolation magInterpolation)
		: minInterpolation(minInterpolation)
		, magInterpolation(magInterpolation)
	{
	}

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

	void SubTexture::set(const SubTexture& texture, InterpolationMinMag interpolation)
	{
		std::unique_ptr<PixelAccessor> access;

		Scaling scaling = Scaling::None;
		if (glm::any(glm::greaterThan(texture.getSize(), this->getSize())))
			scaling = Scaling::Min;
		else if (glm::any(glm::lessThan(texture.getSize(), this->getSize())))
			scaling = Scaling::Mag;

		if (scaling == Scaling::None)
			access = std::make_unique<SimpleAccessor>(*texture.base->image, this->layer, this->face, this->level);
		else if (scaling == Scaling::Min)
			switch (interpolation.minInterpolation)
			{
			case Interpolation::NearestNeighbors:
				access = std::make_unique<NearestNeighborAccessor>(*texture.base->image, this->getSize().x, this->getSize().y, this->layer, this->face, this->level);
				break;
			case Interpolation::Bicubic:
				access = std::make_unique<BicubicAccessor>(*texture.base->image, this->getSize().x, this->getSize().y, this->layer, this->face, this->level);
				break;
			}
		else
			switch (interpolation.magInterpolation)
			{
			case Interpolation::NearestNeighbors:
				access = std::make_unique<NearestNeighborAccessor>(*texture.base->image, this->getSize().x, this->getSize().y, this->layer, this->face, this->level);
				break;
			case Interpolation::Bicubic:
				access = std::make_unique<BicubicAccessor>(*texture.base->image, this->getSize().x, this->getSize().y, this->layer, this->face, this->level);
				break;
			}

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