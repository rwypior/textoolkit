#ifndef _h_textoolkit_texture_accessor
#define _h_textoolkit_texture_accessor

#include <memory>

namespace textoolkit
{
	class Image;
	union Pixel;

	class PixelAccessor
	{
	public:
		virtual ~PixelAccessor() = default;
		virtual Pixel getPixel(unsigned int x, unsigned int y) const = 0;
		virtual PixelAccessor& setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor) = 0;
		virtual const Image& getImage() const = 0;
		virtual unsigned int getLayer() const = 0;
		virtual unsigned int getFace() const = 0;
		virtual unsigned int getLevel() const = 0;
		virtual PixelAccessor& setImage(const Image& image) = 0;
		virtual PixelAccessor& setLayer(unsigned int layer) = 0;
		virtual PixelAccessor& setFace(unsigned int face) = 0;
		virtual PixelAccessor& setLevel(unsigned int level) = 0;

		template<typename T>
		std::unique_ptr<T> makeSubaccessor()
		{
			return std::make_unique<T>(this->getImage(), this->getLayer(), this->getFace(), this->getLevel());
		}
	};

	/// Simple pass-through pixel accessor
	class SimpleAccessor : public PixelAccessor
	{
	public:
		SimpleAccessor(const Image& image, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);

		virtual Pixel getPixel(unsigned int x, unsigned int y) const override;
		virtual PixelAccessor& setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor) override;
		virtual const Image& getImage() const override;
		virtual unsigned int getLayer() const override;
		virtual unsigned int getFace() const override;
		virtual unsigned int getLevel() const override;
		virtual PixelAccessor& setImage(const Image& image) override;
		virtual PixelAccessor& setLayer(unsigned int layer) override;
		virtual PixelAccessor& setFace(unsigned int face) override;
		virtual PixelAccessor& setLevel(unsigned int level) override;

	private:
		const Image* image;
		unsigned int layer;
		unsigned int face;
		unsigned int level;
		std::unique_ptr<PixelAccessor> subAccessor;
	};

	/// Returns pixels with inverted Y coordinate
	class InvertYAccessor : public PixelAccessor
	{
	public:
		InvertYAccessor(const Image& image, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);

		virtual Pixel getPixel(unsigned int x, unsigned int y) const override;
		virtual PixelAccessor& setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor) override;
		virtual const Image& getImage() const override;
		virtual unsigned int getLayer() const override;
		virtual unsigned int getFace() const override;
		virtual unsigned int getLevel() const override;
		virtual PixelAccessor& setImage(const Image& image) override;
		virtual PixelAccessor& setLayer(unsigned int layer) override;
		virtual PixelAccessor& setFace(unsigned int face) override;
		virtual PixelAccessor& setLevel(unsigned int level) override;

	private:
		std::unique_ptr<PixelAccessor> subAccessor;
	};

	/// Accesses image pixels as if the image was scaled to given size
	/// using bicubic interpolation
	class BicubicAccessor : public PixelAccessor
	{
	public:
		BicubicAccessor(const Image& image, unsigned int scaledWidth, unsigned int scaledHeight, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);

		virtual Pixel getPixel(unsigned int x, unsigned int y) const override;
		virtual PixelAccessor& setSubAccessor(std::unique_ptr<PixelAccessor>&& subAccessor) override;
		virtual const Image& getImage() const override;
		virtual unsigned int getLayer() const override;
		virtual unsigned int getFace() const override;
		virtual unsigned int getLevel() const override;
		virtual PixelAccessor& setImage(const Image& image) override;
		virtual PixelAccessor& setLayer(unsigned int layer) override;
		virtual PixelAccessor& setFace(unsigned int face) override;
		virtual PixelAccessor& setLevel(unsigned int level) override;

	private:
		Pixel sample(int x, int y) const;

		unsigned int scaledWidth;
		unsigned int scaledHeight;
		std::unique_ptr<PixelAccessor> subAccessor;
	};
}

#endif