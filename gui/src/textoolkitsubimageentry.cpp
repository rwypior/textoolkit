#include "gui/textoolkitsubimageentry.hpp"

namespace textoolkit
{
	TexToolkitSubimageEntry::TexToolkitSubimageEntry(wxWindow* parent)
		:
		SubimageEntry(parent)
	{
		this->previewImage->SetBitmap(this->bitmap);

		this->Bind(wxEVT_SIZE, &TexToolkitSubimageEntry::sizeEvent, this);
	}

	void TexToolkitSubimageEntry::setTexture(Texture* texture)
	{
		this->updatePreview();
	}

	void TexToolkitSubimageEntry::updatePreview()
	{
		if (this->texture)
		{
			auto& image = this->texture->getImage();
			float scale = 1.0f;
			if (image.getWidth() > image.getHeight())
				scale = static_cast<float>(image.getWidth()) / static_cast<float>(image.getHeight());
			else
				scale = static_cast<float>(image.getHeight()) / static_cast<float>(image.getWidth());

			auto previewSize = this->previewImage->GetSize();
			wxSize size(previewSize.GetWidth() * scale, previewSize.GetHeight() * scale);

			this->bitmap = this->texture->getBitmap();
			wxBitmap::Rescale(this->bitmap, size);
		}
	}

	void TexToolkitSubimageEntry::sizeEvent(wxSizeEvent& ev)
	{
		this->updatePreview();
	}
}