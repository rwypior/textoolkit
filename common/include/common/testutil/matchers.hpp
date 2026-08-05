#ifndef _h_common_testutil_matchers
#define _h_common_testutil_matchers

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

struct Vec3Matcher : Catch::Matchers::MatcherGenericBase
{
    Vec3Matcher(glm::vec3 const& vec, float epsilon = 0.0001f)
        : vec{ vec }
        , epsilon(epsilon)
    {
    }

    bool match(glm::vec3 const& other) const
    {
        return glm::all(glm::epsilonEqual(vec, other, epsilon));
    }

    std::string describe() const override
    {
        std::stringstream ss;
        ss << "Equals " << vec;
        return ss.str();
    }

private:
    glm::vec3 vec;
    float epsilon = 0.0001f;
};

inline Vec3Matcher EqualsVec3(const glm::vec3& vec)
{
    return Vec3Matcher(vec);
}

inline Vec3Matcher EqualsVec3(float x, float y, float z)
{
    return Vec3Matcher(glm::vec3(x, y, z));
}

inline Vec3Matcher EqualsVec3(float n)
{
    return Vec3Matcher(glm::vec3(n));
}

#endif