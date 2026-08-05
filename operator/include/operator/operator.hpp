#ifndef _h_textoolkit_operator_operator
#define _h_textoolkit_operator_operator

#include "common/result.hpp"

#include <string>

namespace textoolkit
{
	class Image;

	class OperatorResultData
	{
	public:
		virtual ~OperatorResultData() = default;
	};

	struct OperatorResult : Result<OperatorResultData>
	{
		std::string info;

		using Result::Result;

		OperatorResult(Code code, const std::string& info)
			: Result(code)
			, info(info)
		{
		}
	};

	class Operator
	{
	public:
		virtual ~Operator() = default;
		virtual OperatorResult apply(Image& image, unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0) const = 0;
	};
}

#endif