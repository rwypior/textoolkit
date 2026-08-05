#ifndef _h_common_testutil_printers
#define _h_common_testutil_printers

#include "common/pixel.hpp"

#include <ostream>
#include <glm/vec3.hpp>

#include <optional>

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& value)
{
    return os << "(" << value.x << ", " << value.y << ", " << value.z << ")";
}

inline std::ostream& operator<<(std::ostream& os, const glm::u8vec3& value)
{
    return os << "(" << value.x << ", " << value.y << ", " << value.z << ")";
}

inline std::ostream& operator<<(std::ostream& os, const textoolkit::Pixel& value)
{
    return 
        os << "(" << 
        static_cast<unsigned int>(value.r) << ", " << 
        static_cast<unsigned int>(value.g) << ", " << 
        static_cast<unsigned int>(value.b) << ", " << 
        static_cast<unsigned int>(value.a) << ")";
}

inline std::ostream& operator<<(std::ostream& os, const std::optional<textoolkit::Pixel>& value)
{
    if (value)
        return operator<<(os, *value);
    return os << "[Empty pixel]";
}

#endif