#include "gui/textoolkittextureview.hpp"
#include "gui/texture.hpp"

namespace textoolkit
{
	TexToolkitTextureView::TexToolkitTextureView(std::unique_ptr<Texture>&& texture, wxWindow* parent)
		: TextureView(parent)
		, texture(std::move(texture))
	{
		this->updateFlatView();
	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;

	void TexToolkitTextureView::updateFlatView()
	{
		if (this->texture)
		{
			auto& image = this->texture->getImage();
			float scale = 1.0f;
			if (image.getWidth() > image.getHeight())
				scale = static_cast<float>(image.getWidth()) / static_cast<float>(image.getHeight());
			else
				scale = static_cast<float>(image.getHeight()) / static_cast<float>(image.getWidth());

			auto previewSize = this->flatView->GetSize();
			wxSize size(previewSize.GetWidth() * scale, previewSize.GetHeight() * scale);

			this->flatViewBitmap = this->texture->getBitmap();
			wxBitmap::Rescale(this->flatViewBitmap, size);

			this->flatView->SetBitmap(this->flatViewBitmap);
			this->flatView->Refresh();
			this->flatView->Update();
		}
	}
}