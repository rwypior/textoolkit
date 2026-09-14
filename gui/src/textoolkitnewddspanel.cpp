#include "gui/textoolkitnewddspanel.hpp"
#include "dds/dds.hpp"
#include "common/threadpool.hpp"
#include "texture/textureloader.hpp"

namespace textoolkit
{
	TexToolkitnewDdsPanel::TexToolkitnewDdsPanel(wxWindow* parent)
		: newDdsPanel(parent)
		, batchImportCubemap(Image::TextureType::TextureCube, 0, 6, 0, this)
		, batchImport2DArray(Image::TextureType::Texture2DArray, 0, 0, 0, this)
	{
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

		this->updateWidgetStates();

		this->typeChoice->Bind(wxEVT_CHOICE, &TexToolkitnewDdsPanel::typeChangedEvent, this);
		this->mipmapsCheckbox->Bind(wxEVT_CHECKBOX, &TexToolkitnewDdsPanel::generateMipmapsCheckedEvent, this);
		this->selectImagesButton->Bind(wxEVT_BUTTON, &TexToolkitnewDdsPanel::importClickedEvent, this);
	}

	std::unique_ptr<textoolkit::GuiTexture> TexToolkitnewDdsPanel::createTexture()
	{
		auto dds = std::make_shared<textoolkit::DDS>(
			this->getTextureType(),
			this->getFormat(),
			this->getCompression(),
			glm::vec3(this->getTextureWidth(), this->getTextureHeight(), this->getTextureDepth()),
			this->getLayersCount(),
			this->getGenerateMipmaps()
		);

		if (this->getBatchImportDlg() && this->getBatchImportDlg()->getCountSelected() > 0)
			this->importSubimages(dds);

		return std::make_unique<textoolkit::GuiTexture>(std::move(dds), this->getTextureName());
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

	TexToolkitBatchImportDialog::ImportItems TexToolkitnewDdsPanel::getImportImages() const
	{
		if (auto dlg = this->getBatchImportDlg())
			return dlg->getItems();

		return {};
	}

	void TexToolkitnewDdsPanel::updateWidgetStates()
	{
		auto type = this->typeChoice->GetStringSelection();
		if (
			this->getTextureType() == Image::TextureType::Texture2DArray
			)
			this->layersCount->Enable();
		else
			this->layersCount->Disable();

		if (this->getTextureType() == Image::TextureType::Texture3D)
			this->depthEdit->Enable();
		else
			this->depthEdit->Disable();

		if (
			this->getTextureType() == Image::TextureType::TextureCube ||
			this->getTextureType() == Image::TextureType::Texture2DArray
			)
			this->selectImagesButton->Enable();
		else
			this->selectImagesButton->Disable();
	}

	void TexToolkitnewDdsPanel::updateImportedLabel()
	{
		this->importedImagesLabel->SetLabel(wxString::Format("Selected %d images", this->getBatchImportDlg()->getCountSelected()));
	}

	const TexToolkitBatchImportDialog* TexToolkitnewDdsPanel::getBatchImportDlg() const
	{
		switch (this->getTextureType())
		{
		case Image::TextureType::TextureCube:
			return &this->batchImportCubemap;
			break;
		case Image::TextureType::Texture2DArray:
			return &this->batchImport2DArray;
			break;
		}

		return nullptr;
	}

	TexToolkitBatchImportDialog* TexToolkitnewDdsPanel::getBatchImportDlg()
	{
		return const_cast<TexToolkitBatchImportDialog*>(const_cast<const TexToolkitnewDdsPanel*>(this)->getBatchImportDlg());
	}

	void TexToolkitnewDdsPanel::importSubimages(std::shared_ptr<DDS>& dds)
	{
		auto importDlg = this->getBatchImportDlg();
		if (!importDlg)
			return;

		TextureLoader loader;
		FiniteThreadpool tp;

		auto items = importDlg->getItems();
		InterpolationMinMag interpolation(
			importDlg->getMinInterpolation(),
			importDlg->getMagInterpolation()
		);

		for (auto& item : items)
		{
			tp.enqueue([&dds, &item, &loader, interpolation]() {
				auto tex = GuiTexture(std::move(*loader.loadTexture(item.path)));
				auto source = SubTexture::createLevel(tex, 0, 0, 0);

				switch (item.type)
				{
				case TexToolkitBatchImportDialog::ImportItem::Type::Face:
					for (unsigned int level = 0; level < dds->getLevels(); level++)
					{
						auto destination = SubTexture::createLevel(Texture(dds, ""), 0, static_cast<unsigned int>(item.face), level);
						destination.set(source, interpolation);
					}
					break;
				case TexToolkitBatchImportDialog::ImportItem::Type::Layer:
					for (unsigned int face = 0; face < dds->getFaces(); face++)
					{
						for (unsigned int level = 0; level < dds->getLevels(); level++)
						{
							auto destination = SubTexture::createLevel(Texture(dds, ""), item.layer, face, level);
							destination.set(source, interpolation);
						}
					}
					break;
				}
			});
		}

		tp.start();
		tp.waitForAll();
	}

	void TexToolkitnewDdsPanel::typeChangedEvent(wxCommandEvent& event)
	{
		this->updateWidgetStates();
	}

	void TexToolkitnewDdsPanel::generateMipmapsCheckedEvent(wxCommandEvent& event)
	{
		this->updateWidgetStates();
	}

	void TexToolkitnewDdsPanel::importClickedEvent(wxCommandEvent& event)
	{
		TexToolkitBatchImportDialog* dlg = nullptr;

		switch (this->getTextureType())
		{
		case Image::TextureType::TextureCube:
			this->batchImportCubemap.setFaces(6);
			dlg = &this->batchImportCubemap;
			break;
		case Image::TextureType::Texture2DArray:
			this->batchImport2DArray.setLayers(this->getLayersCount());
			dlg = &this->batchImport2DArray;
			break;
		}

		if (!dlg)
			return;

		dlg->ShowModal();
		this->updateImportedLabel();
	}
}
