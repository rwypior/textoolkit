#include "common.hpp"
#include "utest_img.bmp.hpp"
#include "bmp/bmp.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("BMP::BMP::Basic loading", "[bmp]")
{
	auto result = textoolkit::bmp::Bmp::fromString(std::string(resx::utest_img_bmp, resx::utest_img_bmp_size));
	REQUIRE(result);

	auto expectedBegin = std::begin(resx::utest_img_bmp) + 138;
	auto expectedEnd = std::begin(resx::utest_img_bmp) + resx::utest_img_bmp_size;
	std::vector<unsigned char> expected(expectedBegin, expectedEnd);

	textoolkit::bmp::Bmp bmp = std::move(result);
	const auto& bytes = bmp.Image::getBytes();

	REQUIRE(bmp.getWidth() == 16);
	REQUIRE(bmp.getHeight() == 16);
	REQUIRE(bmp.getWidth() * bmp.getHeight() * bmp.getBytesPerPixel() == 768);
	REQUIRE(bytes == expected);
}

TEST_CASE("BMP::BMP::Getting pixels", "[bmp]")
{
	auto result = textoolkit::bmp::Bmp::fromString(std::string(resx::utest_img_bmp, resx::utest_img_bmp_size));
	REQUIRE(result);

	textoolkit::bmp::Bmp bmp = std::move(result);
	REQUIRE(*bmp.Image::getPixel(0, 0) == textoolkit::Pixel(0, 0, 255));
	REQUIRE(*bmp.Image::getPixel(8, 0) == textoolkit::Pixel(255, 255, 255));
	REQUIRE(*bmp.Image::getPixel(15, 0) == textoolkit::Pixel(0, 0, 0));
	REQUIRE(*bmp.Image::getPixel(0, 15) == textoolkit::Pixel(255, 0, 0));
	REQUIRE(*bmp.Image::getPixel(15, 15) == textoolkit::Pixel(0, 255, 0));
}

TEST_CASE("BMP::BMP::Getting pixels inverted Y", "[bmp]")
{
	auto result = textoolkit::bmp::Bmp::fromString(std::string(resx::utest_img_bmp, resx::utest_img_bmp_size));
	REQUIRE(result);

	textoolkit::bmp::Bmp bmp = std::move(result);

	auto inv = textoolkit::DataOption::InvertY;
	REQUIRE(*bmp.Image::getPixel(0, 0, inv) == textoolkit::Pixel(255, 0, 0));
	REQUIRE(*bmp.Image::getPixel(15, 0, inv) == textoolkit::Pixel(0, 255, 0));
	REQUIRE(*bmp.Image::getPixel(0, 15, inv) == textoolkit::Pixel(0, 0, 255));
	REQUIRE(*bmp.Image::getPixel(8, 15, inv) == textoolkit::Pixel(255, 255, 255));
	REQUIRE(*bmp.Image::getPixel(15, 15, inv) == textoolkit::Pixel(0, 0, 0));
}

TEST_CASE("BMP::BMP::Basic saving", "[bmp]")
{
	auto result = textoolkit::bmp::Bmp::fromString(std::string(resx::utest_img_bmp, resx::utest_img_bmp_size));
	REQUIRE(result);

	textoolkit::bmp::Bmp bmp = std::move(result);

	std::ostringstream stream(std::ios_base::out);
	bmp.save(stream);
	std::string readBytes = stream.str();

	auto expectedBegin = std::begin(resx::utest_img_bmp);
	auto expectedEnd = std::begin(resx::utest_img_bmp) + resx::utest_img_bmp_size;
	std::vector<unsigned int> expected(expectedBegin, expectedEnd);
	std::vector<unsigned int> bytes(readBytes.begin(), readBytes.end());

	REQUIRE(bytes == expected);
}