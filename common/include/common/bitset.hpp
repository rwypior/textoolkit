#ifndef _h_textoolkit_common_bitset
#define _h_textoolkit_common_bitset

#include <bitset>
#include <type_traits>
#include <initializer_list>

namespace textoolkit
{
	template<typename Enum, unsigned int Count, typename IndexType = unsigned int>
	class EnumBitset : public std::bitset<Count>
	{
	public:
		EnumBitset(std::initializer_list<Enum> il = {})
		{
			for (auto e : il)
				this->set(e);
		}

		constexpr bool operator[](Enum pos) const
		{
			return this->bitset::operator[](static_cast<IndexType>(pos));
		}

		bool test(Enum pos) const
		{
			return this->bitset::test(static_cast<IndexType>(pos));
		}

		EnumBitset& set(Enum pos, bool value = true)
		{
			this->bitset::set(static_cast<IndexType>(pos), value);
			return *this;
		}

		EnumBitset& reset(Enum pos)
		{
			this->bitset::reset(static_cast<IndexType>(pos));
			return *this;
		}

		EnumBitset& flip(Enum pos)
		{
			this->bitset::flip(static_cast<IndexType>(pos));
			return *this;
		}

		static EnumBitset default(bool value = false)
		{
			EnumBitset<Enum, Count> result;
			value ? result.bitset::set() : result.bitset::reset();
			return result;
		}
	};
}

#endif