#ifndef _h_common_result
#define _h_common_result

#include <optional>
#include <vector>
#include <string>
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
			InvalidMagic,
			NothingLoaded
		};

		struct Warning
		{
			std::string warning;
			std::optional<unsigned int> lineNumber = {};

			Warning(const std::string& warning, std::optional<unsigned int> lineNumber = {})
				: warning(warning)
				, lineNumber(lineNumber)
			{
			}

			Warning(const Warning& b)
				: warning(b.warning)
				, lineNumber(b.lineNumber)
			{
			}

			std::string toString() const
			{
				return (this->lineNumber ? (std::to_string(*this->lineNumber) + ": ") : "") + this->warning;
			}
		};

		using Warnings = std::vector<Warning>;

		Result(Code code)
			: code(code)
		{
		}
		
		Result(Code code, Warnings&& warnings)
			: code(code)
			, warnings(std::move(warnings))
		{
		}

		Result(T&& data)
			: code(Code::Ok)
			, data(std::move(data))
		{
		}

		Result(T&& data, Warnings&& warnings)
			: code(Code::Ok)
			, data(std::move(data))
			, warnings(std::move(warnings))
		{
		}

		Result(Code code, T&& data, Warnings&& warnings = {})
			: code(code)
			, data(std::move(data))
			, warnings(std::move(warnings))
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
		Warnings warnings;
	};
}

#endif