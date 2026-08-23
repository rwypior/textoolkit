#include "gui/textoolkitnewddspanel.hpp"

namespace textoolkit
{
	TexToolkitnewDdsPanel::TexToolkitnewDdsPanel(wxWindow* parent)
		:
		newDdsPanel(parent)
	{
		this->updateWidgetStates();

		this->typeChoice->Append("2D Texture", reinterpret_cast<void*>(Image::TextureType::Texture2D));
		this->typeChoice->Append("3D Texture", reinterpret_cast<void*>(Image::TextureType::Texture3D));
		this->typeChoice->Append("Cube map", reinterpret_cast<void*>(Image::TextureType::TextureCube));
		this->typeChoice->Append("2D array texture", reinterpret_cast<void*>(Image::TextureType::Texture2DArray));
		this->typeChoice->Append("Cube map array texture", reinterpret_cast<void*>(Image::TextureType::TextureCubeArray));
		this->typeChoice->Select(0);

		this->formatChoice->Append("RGB8", reinterpret_cast<void*>(Image::TextureInternalFormat::Rgb8));
		this->formatChoice->Select(0);
		
		this->compressionChoice->Append("None", reinterpret_cast<void*>(Image::CompressionType::None));
		this->compressionChoice->Append("DXT1", reinterpret_cast<void*>(Image::CompressionType::DXT1));
		this->compressionChoice->Append("DXT3", reinterpret_cast<void*>(Image::CompressionType::DXT3));
		this->compressionChoice->Append("DXT5", reinterpret_cast<void*>(Image::CompressionType::DXT5));
		this->compressionChoice->Select(0);

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

	unsigned int TexToolkitnewDdsPanel::getTextureDepth() const
	{
		return this->depthEdit->GetValue();
	}

	bool TexToolkitnewDdsPanel::getGenerateMipmaps() const
	{
		return this->mipmapsCheckbox->IsChecked();
	}

	Image::TextureType TexToolkitnewDdsPanel::getTextureType() const
	{
		return static_cast<Image::TextureType>(reinterpret_cast<size_t>(this->typeChoice->GetClientData(this->typeChoice->GetSelection())));
	}

	Image::TextureInternalFormat TexToolkitnewDdsPanel::getFormat() const
	{
		return static_cast<Image::TextureInternalFormat>(reinterpret_cast<size_t>(this->formatChoice->GetClientData(this->formatChoice->GetSelection())));
	}

	Image::CompressionType TexToolkitnewDdsPanel::getCompression() const
	{
		return static_cast<Image::CompressionType>(reinterpret_cast<size_t>(this->compressionChoice->GetClientData(this->compressionChoice->GetSelection())));
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

		if (type == "3D Texture")
			this->depthEdit->Enable();
		else
			this->depthEdit->Disable();
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
