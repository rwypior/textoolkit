#include "common.hpp"
#include "common/rgbimage.hpp"
#include "common/shape.hpp"
#include "common/filler.hpp"
#include "operator/fillareaoperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Operator::FillArea::Basic fill", "[operator][fillarea]")
{
	textoolkit::RgbImage image(16, 16);
	
	textoolkit::FillAreaOperator oper1(textoolkit::Rectangle::create(0, 8, 8, 15), textoolkit::SolidFiller::create(textoolkit::Pixel::red()));
	textoolkit::FillAreaOperator oper2(textoolkit::Rectangle::create(8, 8, 15, 15), textoolkit::SolidFiller::create(textoolkit::Pixel::green()));
	textoolkit::FillAreaOperator oper3(textoolkit::Rectangle::create(0, 0, 8, 8), textoolkit::SolidFiller::create(textoolkit::Pixel::blue()));
	textoolkit::FillAreaOperator oper4(textoolkit::Rectangle::create(8, 0, 11, 8), textoolkit::SolidFiller::create(textoolkit::Pixel::white()));
	textoolkit::FillAreaOperator oper5(textoolkit::Rectangle::create(12, 0, 15, 8), textoolkit::SolidFiller::create(textoolkit::Pixel::black()));

	oper1.apply(image);
	oper2.apply(image);
	oper3.apply(image);
	oper4.apply(image);
	oper5.apply(image);

	REQUIRE(image.Image::getPixel(1, 9) == textoolkit::Pixel::red());
	REQUIRE(image.Image::getPixel(9, 9) == textoolkit::Pixel::green());
	REQUIRE(image.Image::getPixel(1, 1) == textoolkit::Pixel::blue());
	REQUIRE(image.Image::getPixel(9, 1) == textoolkit::Pixel::white());
	REQUIRE(image.Image::getPixel(13, 1) == textoolkit::Pixel::black());
}