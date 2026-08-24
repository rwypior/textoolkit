#ifndef _h_textoolkit_common_texture
#define _h_textoolkit_common_texture

#include "common/image.hpp"

#include <glm/vec2.hpp>

#include <string>
#include <memory>

namespace textoolkit
{
	class Texture
	{
		friend class SubTexture;

	public:
		Texture();
		Texture(Texture&& texture) noexcept;
		Texture(std::shared_ptr<Image> image, const std::string& name);
		Texture(std::shared_ptr<Image> image, const std::string& path, const std::string& name);
		virtual ~Texture();

		Texture& operator=(Texture&& texture) noexcept;

		std::string getName() const;
		void setName(const std::string& name);
		std::string getPath() const;
		Image::Type getType() const;

		Image& getImage();
		bool hasImage() const;

		virtual bool save(const std::string& path); // Save current image to file

	protected:
		std::shared_ptr<Image> image;
		std::string name;
		std::string path;
	};

	class SubTexture
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
		SubTexture(SubTexture&& subtexture) noexcept;
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name);
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name);
		virtual ~SubTexture();

		SubTexture& operator=(SubTexture&& subtexture) noexcept;

		static SubTexture createLayer(Texture& texture, unsigned int layer);
		static SubTexture createFace(Texture& texture, unsigned int layer, unsigned int face);
		static SubTexture createLevel(Texture& texture, unsigned int layer, unsigned int face, unsigned int level);

		Image& getImage();
		bool hasImage() const;

		/// Copy data from another texture into this subtexture
		virtual void set(const SubTexture& texture);

		glm::uvec2 getSize() const;

		Type getType() const;
		unsigned int getLayer() const;
		unsigned int getFace() const;
		unsigned int getLevel() const;

	protected:
		SubTexture(std::unique_ptr<Texture>&& base);
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& name, std::unique_ptr<Texture>&& base);
		SubTexture(Type type, unsigned int layer, unsigned int face, unsigned int level, std::shared_ptr<Image> image, const std::string& path, const std::string& name, std::unique_ptr<Texture>&& base);

		Type type = Type::Face;
		unsigned int layer = 0;
		unsigned int face = 0;
		unsigned int level = 0;
		std::unique_ptr<Texture> base;
	};
}

#endif