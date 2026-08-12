#include "gui/textoolkitsubimageentry.hpp"
#include "gui/util.hpp"

namespace textoolkit
{
	TexToolkitSubimageEntry::TexToolkitSubimageEntry(std::unique_ptr<SubTexture>&& texture, wxWindow* parent)
		: SubimageEntry(parent)
		, texture(std::move(texture))
	{
		this->updatePreview();

		this->SetCursor(wxCursor(wxStockCursor::wxCURSOR_HAND));

		this->Bind(wxEVT_SIZE, &TexToolkitSubimageEntry::sizeEvent, this);
		bindRecursively(*this, wxEVT_ENTER_WINDOW, &TexToolkitSubimageEntry::mouseEnterEvent, this);
		bindRecursively(*this, wxEVT_LEAVE_WINDOW, &TexToolkitSubimageEntry::mouseLeaveEvent, this);
	}

	void TexToolkitSubimageEntry::setTexture(std::unique_ptr<SubTexture>&& texture)
	{
		this->texture = std::move(texture);
		this->updatePreview();
	}

	void TexToolkitSubimageEntry::setLineVisibility(bool visible)
	{
		this->m_staticline1->Show(visible);
	}

	std::string TexToolkitSubimageEntry::getSubtextureName() const
	{
		switch (this->texture->getType())
		{
		case SubTexture::Type::Base: return "Base texture";
		case SubTexture::Type::Level: return std::string("Level #") + std::to_string(this->texture->getIndex());
		case SubTexture::Type::Face: return std::string("Face #") + std::to_string(this->texture->getIndex());
		case SubTexture::Type::Layer: return std::string("Layer #") + std::to_string(this->texture->getIndex());
		}

		assert(!"Invalid texture type");
		return "";
	}

	std::string TexToolkitSubimageEntry::getSubtextureSize() const
	{
		unsigned int width = 0;
		unsigned int height = 0;
		if (this->texture)
		{
			auto size = this->texture->getSize();
			width = size.x;
			height = size.y;
		}
		return std::to_string(width) + "px x " + std::to_string(height) + "px";
	}

	void TexToolkitSubimageEntry::updatePreview()
	{
		if (this->texture)
		{
			this->nameLabel->SetLabel(this->getSubtextureName());
			this->measurementsLabel->SetLabel(this->getSubtextureSize());

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

			this->previewImage->SetBitmap(this->bitmap);
		}
	}

	void TexToolkitSubimageEntry::sizeEvent(wxSizeEvent& ev)
	{
		this->updatePreview();
	}

	void TexToolkitSubimageEntry::mouseEnterEvent(wxMouseEvent& ev)
	{
		if (!this->backgroundColor)
			this->backgroundColor = this->GetBackgroundColour();

		wxColour highlightColor(
			std::min(255, static_cast<int>(static_cast<float>(this->backgroundColor->Red()) * 1.2f)),
			std::min(255, static_cast<int>(static_cast<float>(this->backgroundColor->Green()) * 1.2f)),
			std::min(255, static_cast<int>(static_cast<float>(this->backgroundColor->Blue()) * 1.2f))
		);
		this->SetBackgroundColour(highlightColor);
		for (auto& child : this->GetChildren())
		{
			child->SetBackgroundColour(highlightColor);
		}
		this->Refresh();
	}

	void TexToolkitSubimageEntry::mouseLeaveEvent(wxMouseEvent& ev)
	{
		const auto wnd = static_cast<wxWindow*>(ev.GetEventObject());
		const auto mousePos = wnd->ClientToScreen(ev.GetPosition());
		if (!this->GetScreenRect().Contains(mousePos))
		{
			this->SetBackgroundColour(*this->backgroundColor);
			for (auto& child : this->GetChildren())
			{
				child->SetBackgroundColour(*this->backgroundColor);
			}
			this->Refresh();
		}
	}
}