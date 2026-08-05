#include "common.hpp"
#include "common/pixel.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Common::Pixel::Converting to vec3", "[common][pixel]")
{
	textoolkit::Pixel pixel(0, 128, 255);
	auto u8vec3 = pixel.toVec3<glm::u8vec3>();
	auto uvec3 = pixel.toVec3<glm::uvec3>();
	auto vec3 = pixel.toVec3<glm::vec3>();

	static constexpr double eps = 0.0001;

	REQUIRE(u8vec3.r == pixel.r);
	REQUIRE(u8vec3.g == pixel.g);
	REQUIRE(u8vec3.b == pixel.b);
	REQUIRE(uvec3.r == 0);
	REQUIRE(uvec3.g == 2155905152);
	REQUIRE(uvec3.b == 4294967295);
	REQUIRE_THAT(vec3.r, Catch::Matchers::WithinRel(0.0, eps));
	REQUIRE_THAT(vec3.g, Catch::Matchers::WithinRel(0.501960814, eps));
	REQUIRE_THAT(vec3.b, Catch::Matchers::WithinRel(1.0, eps));
}