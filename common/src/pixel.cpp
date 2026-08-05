#include "common/pixel.hpp"

namespace textoolkit
{
	Pixel::Pixel()
		: rgb(0)
	{
	}

	Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}

	Pixel::Pixel(unsigned int rgb)
		: rgb(rgb)
	{
	}

	bool Pixel::operator==(const Pixel& b) const
	{
		return this->rgb == b.rgb;
	}

	/// 0 - r
	/// 1 - g
	/// 2 - b
	/// 3 - a
	unsigned char& Pixel::operator[](unsigned int i)
	{
		return const_cast<unsigned char&>(const_cast<const Pixel*>(this)->operator[](i));
	}

	const unsigned char& Pixel::operator[](unsigned int i) const
	{
		switch (i)
		{
		case 0: return this->r;
		case 1: return this->g;
		case 2: return this->b;
		case 3: return this->a;
		default:
			assert(!"Invalid component index");
			static unsigned char blank = 0;
			return blank;
		}
	}

	Pixel Pixel::white()
	{
		return Pixel(255, 255, 255);
	}

	Pixel Pixel::black()
	{
		return Pixel(0, 0, 0);
	}

	Pixel Pixel::red()
	{
		return Pixel(255, 0, 0);
	}

	Pixel Pixel::green()
	{
		return Pixel(0, 255, 0);
	}

	Pixel Pixel::blue()
	{
		return Pixel(0, 0, 255);
	}

	Pixel Pixel::yellow()
	{
		return Pixel(255, 255, 0);
	}

	Pixel Pixel::magenta()
	{
		return Pixel(255, 0, 255);
	}

	Pixel Pixel::cyan()
	{
		return Pixel(0, 255, 255);
	}
}