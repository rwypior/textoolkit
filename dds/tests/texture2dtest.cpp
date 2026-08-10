#include "common.hpp"
#include "dds/dds.hpp"
#include "bmp/bmp.hpp"

#include "utest_img.bmp.hpp"

#include <catch2/catch_all.hpp>

#include <gli/save_dds.hpp>

TEST_CASE("DDS::Texture2D::Create from scratch", "[DDS][texture2d]")
{
	textoolkit::Pixel expected(0, 128, 255);
	textoolkit::DDS dds = textoolkit::DDS::create2D(16, 16, textoolkit::Pixel(expected.r, expected.g, expected.b));

	auto a = dds.Image::getPixel(0, 0);
	auto b = dds.Image::getPixel(0, 15);
	auto c = dds.Image::getPixel(15, 0);
	auto d = dds.Image::getPixel(15, 15);

	REQUIRE(a == expected);
	REQUIRE(b == expected);
	REQUIRE(c == expected);
	REQUIRE(d == expected);

	// glm::u8vec3 expected(0, 128, 255);

	//auto& tex = dds.getTexture();
	//gli::texture2d tex = textoolkit::DDS::create2D(16, 16, textoolkit::Pixel(expected.r, expected.g, expected.b));

	/*auto a = tex.load<glm::u8vec3>(gli::texture::extent_type(0, 0, 0), gli::texture::size_type(0));
	auto b = tex.load<glm::u8vec3>(gli::texture::extent_type(0, 15), gli::texture::size_type(0));
	auto c = tex.load<glm::u8vec3>(gli::texture::extent_type(15, 0), gli::texture::size_type(0));
	auto d = tex.load<glm::u8vec3>(gli::texture::extent_type(15, 15), gli::texture::size_type(0));

	REQUIRE(a == expected);
	REQUIRE(b == expected);
	REQUIRE(c == expected);
	REQUIRE(d == expected);*/
}


TEST_CASE("DDS::Texture2D::Sanity test", "[DDS][texture2d]")
{
	glm::uvec2 extents(16, 16);

	gli::texture2d tex(
		gli::texture::format_type::FORMAT_RGB8_UINT_PACK8,
		extents
	);

	for (unsigned int x = 0; x < extents.x; x++)
	{
		for (unsigned int y = 0; y < extents.y; y++)
		{
			if (x < 8 && y < 8)
				tex.store(gli::texture2d::extent_type(x, y), 0, textoolkit::Pixel::blue().toVec3<glm::u8vec3>());
			else if (x < 13 && y < 8)
				tex.store(gli::texture2d::extent_type(x, y), 0, textoolkit::Pixel::white().toVec3<glm::u8vec3>());
			else if (y < 8)
				tex.store(gli::texture2d::extent_type(x, y), 0, textoolkit::Pixel::black().toVec3<glm::u8vec3>());
			else if (x < 8 && y > 8)
				tex.store(gli::texture2d::extent_type(x, y), 0, textoolkit::Pixel::red().toVec3<glm::u8vec3>());
			else
				tex.store(gli::texture2d::extent_type(x, y), 0, textoolkit::Pixel::green().toVec3<glm::u8vec3>());
		}
	}

	auto a = tex.load<glm::u8vec3>(gli::texture2d::extent_type(0, 0), gli::texture::size_type(0));
	auto b = tex.load<glm::u8vec3>(gli::texture2d::extent_type(8, 0), gli::texture::size_type(0));
	auto c = tex.load<glm::u8vec3>(gli::texture2d::extent_type(15, 0), gli::texture::size_type(0));
	auto d = tex.load<glm::u8vec3>(gli::texture2d::extent_type(0, 15), gli::texture::size_type(0));
	auto e = tex.load<glm::u8vec3>(gli::texture2d::extent_type(15, 15), gli::texture::size_type(0));
	
	REQUIRE(a == textoolkit::Pixel::blue().toVec3<glm::u8vec3>());
	REQUIRE(b == textoolkit::Pixel::white().toVec3<glm::u8vec3>());
	REQUIRE(c == textoolkit::Pixel::black().toVec3<glm::u8vec3>());
	REQUIRE(d == textoolkit::Pixel::red().toVec3<glm::u8vec3>());
	REQUIRE(e == textoolkit::Pixel::green().toVec3<glm::u8vec3>());
}

TEST_CASE("DDS::Texture2D::Create from BMP", "[DDS][texture2d]")
{
	auto bmpres = textoolkit::Bmp::fromString(std::string(resx::utest_img_bmp, resx::utest_img_bmp_size));
	REQUIRE(bmpres);

	textoolkit::DDS tex = textoolkit::DDS::create2D(*bmpres);

	auto a = tex.Image::getPixel(0, 0);
	auto b = tex.Image::getPixel(8, 0);
	auto c = tex.Image::getPixel(15, 0);
	auto d = tex.Image::getPixel(0, 15);
	auto e = tex.Image::getPixel(15, 15);

	REQUIRE(a == textoolkit::Pixel(0.0f, 0.0f, 255.0f));
	REQUIRE(b == textoolkit::Pixel(255.0f, 255.0f, 255.0f));
	REQUIRE(c == textoolkit::Pixel(0.0f, 0.0f, 0.0f));
	REQUIRE(d == textoolkit::Pixel(255.0f, 0.0f, 0.0f));
	REQUIRE(e == textoolkit::Pixel(0.0f, 255.0f, 0.0f));

	//gli::texture2d tex = textoolkit::DDS::create2D(*bmpres);

	//gli::save_dds(tex, "F:/test/test2.dds");
	//return;
	//for (unsigned int x = 0; x < bmpres->getWidth(); x++)
	//{
	//	for (unsigned int y = 0; y < bmpres->getHeight(); y++)
	//	{
	//		INFO(std::string("x=") + std::to_string(x) + "; y=" + std::to_string(y));
	//		auto pixelDds = tex.load<glm::u8vec3>(gli::texture2d::extent_type(x, y), gli::texture::size_type(0));
	//		auto pixelBmp = bmpres->Image::getPixel(x, y);
	//		REQUIRE(pixelDds == pixelBmp->toVec3<glm::u8vec3>());
	//	}
	//}

	//auto a = tex.load<glm::u8vec3>(gli::texture2d::extent_type(0, 0), gli::texture::size_type(0));
	//auto b = tex.load<glm::u8vec3>(gli::texture2d::extent_type(8, 0), gli::texture::size_type(0));
	//auto c = tex.load<glm::u8vec3>(gli::texture2d::extent_type(15, 0), gli::texture::size_type(0));
	//auto d = tex.load<glm::u8vec3>(gli::texture2d::extent_type(0, 15), gli::texture::size_type(0));
	//auto e = tex.load<glm::u8vec3>(gli::texture2d::extent_type(15, 15), gli::texture::size_type(0));
	//
	//REQUIRE(a == glm::u8vec3(0.0f, 0.0f, 255.0f));
	//REQUIRE(b == glm::u8vec3(255.0f, 255.0f, 255.0f));
	//REQUIRE(c == glm::u8vec3(0.0f, 0.0f, 0.0f));
	//REQUIRE(d == glm::u8vec3(255.0f, 0.0f, 0.0f));
	//REQUIRE(e == glm::u8vec3(0.0f, 255.0f, 0.0f));
}