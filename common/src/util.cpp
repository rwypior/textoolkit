#include "common/util.hpp"

#include <glm/gtx/norm.hpp>

#include <regex>

namespace
{
	struct FaceSetter
	{
		FaceSetter(std::smatch& match)
			: match(match)
		{
		}

		template<typename T>
		textoolkit::model::obj::Face operator()(T&& face)
		{
			constexpr unsigned int faceSize = T::value_type::size();
			unsigned int componentIdx = 0;
			unsigned int vertexIdx = 0;
			for (unsigned int i = 1; i < this->match.size(); i++, componentIdx++)
			{
				if (componentIdx == faceSize)
				{
					componentIdx = 0;
					vertexIdx++;
				}

				auto& el = this->match[i];
				face.at(vertexIdx).get(componentIdx) = std::stoul(el) - 1;
			}
			return face;
		}

		template<>
		textoolkit::model::obj::Face operator()(textoolkit::model::obj::FaceErr&& err)
		{
			return err;
		}

		std::smatch& match;
	};
}

namespace textoolkit
{
	glm::quat orient(glm::vec3 source, glm::vec3 target)
	{
		source = glm::normalize(source);
		target = glm::normalize(target);

		float cosTheta = glm::dot(source, target);
		glm::vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f)
		{
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), source);
			if (glm::length2(rotationAxis) < 0.01)
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), source);

			rotationAxis = normalize(rotationAxis);
			return glm::angleAxis(glm::radians(180.0f), rotationAxis);
		}

		rotationAxis = glm::cross(source, target);

		float s = glm::sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);
	}

	void ltrim(std::string& s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
			}));
	}

	void rtrim(std::string& s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	void trim(std::string& s)
	{
		rtrim(s);
		ltrim(s);
	}

	std::string ltrimmed(std::string s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
			}));
		return s;
	}

	std::string rtrimmed(std::string s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
			}).base(), s.end());
		return s;
	}

	std::string trimmed(std::string s)
	{
		return ltrimmed(rtrimmed(s));
	}

	bool startsWith(const std::string& haystack, const std::string& needle)
	{
		return haystack.compare(0, needle.length(), needle) == 0;
	}

	namespace model::obj
	{
		std::optional<glm::vec3> extractVec3(const std::string& line, unsigned int pos)
		{
			std::regex regex(R"REGEX((?:v|vn|vt) (\-?\d+\.?\d+) (\-?\d+\.?\d+) (\-?\d+\.?\d+))REGEX");
			std::smatch match;
			if (!std::regex_search(line, match, regex))
				return {};
			glm::vec3 result;
			unsigned int i = 0;
			for (unsigned int i = 0; i < decltype(result)::length(); i++)
			{
				try
				{
					result[i] = std::stod(match[i + 1].str());
				}
				catch (...)
				{
					return {};
				}
			}
			return result;
		}
		
		std::optional<glm::vec2> extractVec2(const std::string& line, unsigned int pos)
		{
			std::regex regex(R"REGEX((?:v|vn|vt) (\-?\d+\.?\d+) (\-?\d+\.?\d+))REGEX");
			std::smatch match;
			if (!std::regex_search(line, match, regex))
				return {};
			glm::vec2 result;
			unsigned int i = 0;
			for (unsigned int i = 0; i < decltype(result)::length(); i++)
			{
				try
				{
					result[i] = std::stod(match[i + 1].str());
				}
				catch (...)
				{
					return {};
				}
			}
			return result;
		}

		Face extractFace(const std::string& line, unsigned int pos)
		{
			std::regex regex(R"REGEX(f (\d+(?:\/\d+(?:\/\d+)?)? ?){4})REGEX");
			if (std::regex_search(line, regex))
				return "Only triangulated faces are supported";
			
			std::smatch match;
			
			regex = std::regex(R"REGEX(f (\d+)\/(\d+)\/(\d+) (\d+)\/(\d+)\/(\d+) (\d+)\/(\d+)\/(\d+)$)REGEX");
			if (std::regex_search(line, match, regex))
				return std::visit(FaceSetter(match), Face(FaceVUN()));

			regex = std::regex(R"REGEX(f (\d+)\/(\d+) (\d+)\/(\d+) (\d+)\/(\d+)$)REGEX");
			if (std::regex_search(line, match, regex))
				return std::visit(FaceSetter(match), Face(FaceVU()));
			
			regex = std::regex(R"REGEX(f (\d+)\/\/(\d+) (\d+)\/\/(\d+) (\d+)\/\/(\d+)$)REGEX");
			if (std::regex_search(line, match, regex))
				return std::visit(FaceSetter(match), Face(FaceVN()));

			regex = std::regex(R"REGEX(f (\d+) (\d+) (\d+))REGEX");
			if (std::regex_search(line, match, regex))
				return std::visit(FaceSetter(match), Face(FaceV()));

			return "Invalid face specification";
		}
	}
}