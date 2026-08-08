#ifndef _h_common_result
#define _h_common_result

#include <optional>

namespace textoolkit
{
	template<typename T>
	struct Result
	{
		enum class Code
		{
			Ok,
			Failure,
			FileInaccessible,
			InvalidMagic
		};

		Result(Code code)
			: code(code)
		{
		}

		Result(T&& data)
			: code(Code::Ok)
			, data(std::move(data))
		{
		}

		operator bool() const
		{
			return this->code == Code::Ok;
		}

		bool operator==(Result b) const
		{
			return this->code == b.code && this->data == b.data;
		}

		bool operator==(Code code) const
		{
			return this->code == code;
		}

		operator const T&() const
		{
			return this->data;
		}

		operator T&()
		{
			return this->data;
		}

		T& operator*()
		{
			return *this->data;
		}

		const T& operator*() const
		{
			return *this->data;
		}

		T* operator->()
		{
			return &*this->data;
		}

		const T* operator->() const
		{
			return &*this->data;
		}

		Code code;
		std::optional<T> data;
	};
}

#endif