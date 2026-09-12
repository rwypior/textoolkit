#include "gui/textoolkitnewbmppanel.hpp"
#include "bmp/bmp.hpp"

namespace textoolkit
{
	TexToolkitnewBmpPanel::TexToolkitnewBmpPanel(wxWindow* parent)
		:
		newBmpPanel(parent)
	{
	}

	std::unique_ptr<textoolkit::GuiTexture> TexToolkitnewBmpPanel::createTexture()
	{
		auto bmp = std::make_shared<textoolkit::Bmp>(this->getTextureWidth(), this->getTextureHeight());
		return std::make_unique<textoolkit::GuiTexture>(std::move(bmp), this->getTextureName());
	}

	std::string TexToolkitnewBmpPanel::getTextureName() const
	{
		return this->nameEdit->GetValue().ToStdString();
	}

	unsigned int TexToolkitnewBmpPanel::getTextureWidth() const
	{
		return this->widthEdit->GetValue();
	}

	unsigned int TexToolkitnewBmpPanel::getTextureHeight() const
	{
		return this->heightEdit->GetValue();
	}
}
