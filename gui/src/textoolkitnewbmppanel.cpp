#include "gui/textoolkitnewbmppanel.hpp"

namespace textoolkit
{
	TexToolkitnewBmpPanel::TexToolkitnewBmpPanel(wxWindow* parent)
		:
		newBmpPanel(parent)
	{
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
