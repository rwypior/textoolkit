#ifndef _h_common_pixel
#define _h_common_pixel

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cassert>
#include <numeric>
#include <type_traits>

namespace textoolkit
{
	union Pixel
	{
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		unsigned int rgb;

		Pixel();
		Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		Pixel(unsigned int rgb);

		bool operator==(const Pixel& b) const;

		/// 0 - r
		/// 1 - g
		/// 2 - b
		/// 3 - a
		unsigned char& operator[](unsigned int i);
		const unsigned char& operator[](unsigned int i) const;

		template<typename T = glm::vec3, bool scale = true>
		T toVec3() const
		{
			static constexpr decltype(r) mCurrent = std::numeric_limits<decltype(r)>::max();
			static constexpr auto isFloatingPoint = []() constexpr {
				return std::is_floating_point<T::value_type>::value;
			};
			static constexpr T::value_type mTarget = 
				scale ? 
				(isFloatingPoint() ? static_cast<T::value_type>(1.0) : std::numeric_limits<T::value_type>::max()) :
				static_cast<T::value_type>(mCurrent);

			return T(
				static_cast<T::value_type>(static_cast<double>(this->r) / mCurrent * mTarget), 
				static_cast<T::value_type>(static_cast<double>(this->g) / mCurrent * mTarget), 
				static_cast<T::value_type>(static_cast<double>(this->b) / mCurrent * mTarget)
			);
		}

		template<typename T = glm::vec4, bool scale = true>
		T toVec4() const
		{
			static constexpr decltype(r) mCurrent = std::numeric_limits<decltype(r)>::max();
			static constexpr auto isFloatingPoint = []() constexpr {
				return std::is_floating_point<T::value_type>::value;
			};
			static constexpr T::value_type mTarget =
				scale ?
				(isFloatingPoint() ? static_cast<T::value_type>(1.0) : std::numeric_limits<T::value_type>::max()) :
				static_cast<T::value_type>(mCurrent);

			return T(
				static_cast<T::value_type>(static_cast<double>(this->r) / mCurrent * mTarget), 
				static_cast<T::value_type>(static_cast<double>(this->g) / mCurrent * mTarget), 
				static_cast<T::value_type>(static_cast<double>(this->b) / mCurrent * mTarget),
				static_cast<T::value_type>(static_cast<double>(this->a) / mCurrent * mTarget)
			);
		}

		Pixel& operator=(const glm::vec3& b);
		Pixel& operator=(const glm::vec4& b);
		Pixel& operator=(const Pixel& b);
		Pixel operator-(const Pixel& b) const;
		Pixel operator-(int c) const;
		Pixel& operator-=(const Pixel& b);
		Pixel& operator-=(int c);
		Pixel operator+(const Pixel& b) const;
		Pixel operator+(int c) const;
		Pixel& operator+=(const Pixel& b);
		Pixel& operator+=(int c);
		Pixel operator*(const Pixel& b) const;
		Pixel operator*(int c) const;
		Pixel& operator*=(const Pixel& b);
		Pixel& operator*=(int c);
		Pixel operator/(const Pixel& b) const;
		Pixel operator/(int c) const;
		Pixel& operator/=(const Pixel& b);
		Pixel& operator/=(int c);

		static Pixel white();
		static Pixel black();
		static Pixel red();
		static Pixel green();
		static Pixel blue();
		static Pixel yellow();
		static Pixel magenta();
		static Pixel cyan();
	};
}

#endif