#ifndef _h_textoolkit_common_util
#define _h_textoolkit_common_util

#include "common/textoolkit.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <optional>
#include <array>
#include <variant>
#include <tuple>
#include <functional>

#ifdef TEXTOOLKIT_WINDOWS
#	define breakpoint() __debugbreak()
#else
#	define breakpoint() raise(SIGTRAP)
#endif

#define STRINGIFY_IMPL(WHAT) #WHAT
#define STRINGIFY(WHAT) STRINGIFY_IMPL(WHAT)

namespace textoolkit
{
	glm::quat orient(glm::vec3 source, glm::vec3 target);

	void ltrim(std::string& s);
	void rtrim(std::string& s);
	void trim(std::string& s);
	std::string ltrimmed(std::string s);
	std::string rtrimmed(std::string s);
	std::string trimmed(std::string s);
	std::vector<std::string> split(const std::string& string, const std::string& delimiter);

	bool startsWith(const std::string& haystack, const std::string& needle);

	namespace model::obj
	{
		struct IndexVUN
		{
			unsigned int vertex;
			unsigned int uv;
			unsigned int normal;

			static constexpr unsigned int size()
			{
				return 3;
			}

			unsigned int& get(unsigned int i)
			{
				return std::array<std::reference_wrapper<unsigned int>, 3>{vertex, uv, normal}.at(i);
			}
		};

		struct IndexVU
		{
			unsigned int vertex;
			unsigned int uv;

			static constexpr unsigned int size()
			{
				return 2;
			}

			unsigned int& get(unsigned int i)
			{
				return std::array<std::reference_wrapper<unsigned int>, 2>{vertex, uv}.at(i);
			}
		};

		struct IndexVN
		{
			unsigned int vertex;
			unsigned int normal;

			static constexpr unsigned int size()
			{
				return 2;
			}

			unsigned int& get(unsigned int i)
			{
				return std::array<std::reference_wrapper<unsigned int>, 2>{vertex, normal}.at(i);
			}
		};

		struct IndexV
		{
			unsigned int vertex;

			static constexpr unsigned int size()
			{
				return 1;
			}

			unsigned int& get(unsigned int i)
			{
				return std::array<std::reference_wrapper<unsigned int>, 1>{vertex}.at(i);
			}
		};

		using FaceVUN = std::array<IndexVUN, 3>;
		using FaceVU = std::array<IndexVU, 3>;
		using FaceVN = std::array<IndexVN, 3>;
		using FaceV = std::array<IndexV, 3>;
		using FaceErr = std::string;
		using Face = std::variant<FaceVUN, FaceVU, FaceVN, FaceV, FaceErr>;

		std::optional<glm::vec3> extractVec3(const std::string& line, unsigned int pos = 0);
		std::optional<glm::vec2> extractVec2(const std::string& line, unsigned int pos = 0);
		Face extractFace(const std::string& line, unsigned int pos = 0);
	}
}

#endif