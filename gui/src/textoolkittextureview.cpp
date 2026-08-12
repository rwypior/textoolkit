#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitsubimageentry.hpp"
#include "gui/texture.hpp"

namespace textoolkit
{
	TexToolkitTextureView::TexToolkitTextureView(std::unique_ptr<Texture>&& texture, wxWindow* parent)
		: TextureView(parent)
		, texture(std::move(texture))
	{
		this->updateFlatView();
		this->updateLevels();
	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;

	void TexToolkitTextureView::updateFlatView()
	{
		if (!this->texture)
			return;

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

	void TexToolkitTextureView::updateLevels()
	{
		this->levelScroller->Freeze();

		auto& children = this->levelScroller->GetChildren();
		for (auto& child : children)
		{
			child->Destroy();
		}

		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		unsigned int levels = image.getLevels();

		for (unsigned int level = 0; level < levels; level++)
		{
			auto subtexture = std::make_unique<SubTexture>(SubTexture::createLevel(*this->texture, level));
			auto entry = new TexToolkitSubimageEntry(std::move(subtexture), this->levelScroller);
			entry->setLineVisibility(level != levels - 1);
			this->levelScroller->GetSizer()->Add(entry);
			this->levelScroller->GetSizer()->FitInside(this->levelScroller);
		}

		this->levelScroller->Layout();
		this->levelScroller->Thaw();
	}
}