#include "gui/textoolkitnewddspanel.hpp"

namespace textoolkit
{
	TexToolkitnewDdsPanel::TexToolkitnewDdsPanel(wxWindow* parent)
		:
		newDdsPanel(parent)
	{
		this->updateWidgetStates();

		this->typeChoice->Bind(wxEVT_CHOICE, &TexToolkitnewDdsPanel::typeChangedEvent, this);
		this->mipmapsCheckbox->Bind(wxEVT_CHECKBOX, &TexToolkitnewDdsPanel::generateMipmapsCheckedEvent, this);
	}

	std::string TexToolkitnewDdsPanel::getTextureName() const
	{
		return this->nameEdit->GetValue().ToStdString();
	}

	unsigned int TexToolkitnewDdsPanel::getTextureWidth() const
	{
		return this->widthEdit->GetValue();
	}

	unsigned int TexToolkitnewDdsPanel::getTextureHeight() const
	{
		return this->heightEdit->GetValue();
	}

	bool TexToolkitnewDdsPanel::getGenerateMipmaps() const
	{
		return this->mipmapsCheckbox->IsChecked();
	}

	unsigned int TexToolkitnewDdsPanel::getMipmapCount() const
	{
		return this->mipmapsCount->GetValue();
	}

	std::string TexToolkitnewDdsPanel::getTextureType() const
	{
		return this->typeChoice->GetStringSelection().ToStdString();
	}

	unsigned int TexToolkitnewDdsPanel::getLayersCount() const
	{
		return this->layersCount->GetValue();
	}

	void TexToolkitnewDdsPanel::updateWidgetStates()
	{
		auto type = this->typeChoice->GetStringSelection();
		if (type == "3D Texture" || type == "Array texture")
			this->layersCount->Enable();
		else
			this->layersCount->Disable();

		if (this->mipmapsCheckbox->IsChecked())
			this->mipmapsCount->Enable();
		else
			this->mipmapsCount->Disable();
	}

	void TexToolkitnewDdsPanel::typeChangedEvent(wxCommandEvent& event)
	{
		this->updateWidgetStates();
	}

	void TexToolkitnewDdsPanel::generateMipmapsCheckedEvent(wxCommandEvent& event)
	{
		this->updateWidgetStates();
	}
}
