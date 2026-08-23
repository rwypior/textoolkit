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

	Pixel& Pixel::operator=(const Pixel& b)
	{
		this->r = b.r;
		this->g = b.g;
		this->b = b.b;
		this->b = b.a;
		return *this;
	}

	Pixel Pixel::operator-(const Pixel& b) const
	{
		return Pixel(
			std::max(0, static_cast<int>(this->r) - static_cast<int>(b.r)),
			std::max(0, static_cast<int>(this->g) - static_cast<int>(b.g)),
			std::max(0, static_cast<int>(this->b) - static_cast<int>(b.b)),
			std::max(0, static_cast<int>(this->r) - static_cast<int>(b.r))
		);
	}

	Pixel Pixel::operator-(int c) const
	{
		return *this - Pixel(c, c, c, c);
	}

	Pixel& Pixel::operator-=(const Pixel& b)
	{
		return *this = *this - b;
	}

	Pixel& Pixel::operator-=(int c)
	{
		return *this = *this - Pixel(c, c, c, c);
	}

	Pixel Pixel::operator+(const Pixel& b) const
	{
		return Pixel(
			std::min(255, static_cast<int>(this->r) + static_cast<int>(b.r)),
			std::min(255, static_cast<int>(this->g) + static_cast<int>(b.g)),
			std::min(255, static_cast<int>(this->b) + static_cast<int>(b.b)),
			std::min(255, static_cast<int>(this->r) + static_cast<int>(b.r))
		);
	}

	Pixel Pixel::operator+(int c) const
	{
		return *this + Pixel(c, c, c, c);
	}

	Pixel& Pixel::operator+=(const Pixel& b)
	{
		return *this = *this + b;
	}

	Pixel& Pixel::operator+=(int c)
	{
		return *this = *this + Pixel(c, c, c, c);
	}

	Pixel Pixel::operator*(const Pixel& b) const
	{
		return Pixel(
			std::min(255, static_cast<int>(this->r) * static_cast<int>(b.r)),
			std::min(255, static_cast<int>(this->g) * static_cast<int>(b.g)),
			std::min(255, static_cast<int>(this->b) * static_cast<int>(b.b)),
			std::min(255, static_cast<int>(this->r) * static_cast<int>(b.r))
		);
	}

	Pixel Pixel::operator*(int c) const
	{
		return *this * Pixel(c, c, c, c);
	}

	Pixel& Pixel::operator*=(const Pixel& b)
	{
		return *this = *this * b;
	}

	Pixel& Pixel::operator*=(int c)
	{
		return *this = *this * Pixel(c, c, c, c);
	}

	Pixel Pixel::operator/(const Pixel& b) const
	{
		return Pixel(
			std::min(255, static_cast<int>(static_cast<double>(this->r) / static_cast<double>(b.r))),
			std::min(255, static_cast<int>(static_cast<double>(this->g) / static_cast<double>(b.g))),
			std::min(255, static_cast<int>(static_cast<double>(this->b) / static_cast<double>(b.b))),
			std::min(255, static_cast<int>(static_cast<double>(this->r) / static_cast<double>(b.r)))
		);
	}

	Pixel Pixel::operator/(int c) const
	{
		return *this / Pixel(c, c, c, c);
	}

	Pixel& Pixel::operator/=(const Pixel& b)
	{
		return *this = *this / b;
	}

	Pixel& Pixel::operator/=(int c)
	{
		return *this = *this / Pixel(c, c, c, c);
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