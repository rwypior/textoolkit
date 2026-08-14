#include "gui/textoolkitsubimageentry.hpp"
#include "gui/util.hpp"

namespace textoolkit
{
	// Event

	TexToolkitSubimageEvent::TexToolkitSubimageEvent(wxEventType eventType, TexToolkitSubimageEntry* entry)
		: wxCommandEvent(eventType, wxID_ANY)
		, entry(entry)
	{
	}

	TexToolkitSubimageEvent* TexToolkitSubimageEvent::Clone() const
	{
		return new TexToolkitSubimageEvent(*this);
	}

	wxDEFINE_EVENT(texEVT_SUBIMAGE_SELECTED, TexToolkitSubimageEvent);
	wxDEFINE_EVENT(texEVT_SUBIMAGE_DESELECTED, TexToolkitSubimageEvent);

	// Entry

	TexToolkitSubimageEntry::TexToolkitSubimageEntry(wxWindow* parent, std::unique_ptr<SubTexture>&& texture, bool initiallySelected)
		: SubimageEntry(parent)
		, texture(std::move(texture))
	{
		this->originalColor = this->GetBackgroundColour();
		this->highlightColor = wxColour(
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Red()) * 1.2f)),
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Green()) * 1.2f)),
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Blue()) * 1.2f))
		);
		this->activeColor = wxColour(
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Red()) * 1.3f)),
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Green()) * 1.3f)),
			std::min(255, static_cast<int>(static_cast<float>(this->originalColor.Blue()) * 1.6f))
		);
		this->SetBackgroundColour(this->originalColor);

		if (initiallySelected)
			this->select(false);

		this->updatePreview();

		this->SetCursor(wxCursor(wxStockCursor::wxCURSOR_HAND));

		this->Bind(wxEVT_SIZE, &TexToolkitSubimageEntry::sizeEvent, this);
		bindRecursively(*this, wxEVT_ENTER_WINDOW, &TexToolkitSubimageEntry::mouseEnterEvent, this);
		bindRecursively(*this, wxEVT_LEAVE_WINDOW, &TexToolkitSubimageEntry::mouseLeaveEvent, this);
		bindRecursively(*this, wxEVT_LEFT_UP, &TexToolkitSubimageEntry::mouseClickEvent, this);

		this->Refresh();
		this->Update();
		this->Layout();
	}

	void TexToolkitSubimageEntry::deselect(bool sendEvent)
	{
		if (!this->isSelected)
			return;

		this->isSelected = false;
		this->setColor(this->originalColor);

		if (sendEvent)
		{
			TexToolkitSubimageEvent event(texEVT_SUBIMAGE_DESELECTED, this);
			this->ProcessEvent(event);
		}
	}

	void TexToolkitSubimageEntry::select(bool sendEvent)
	{
		this->isSelected = true;
		this->setColor(this->activeColor);

		if (sendEvent)
		{
			TexToolkitSubimageEvent event(texEVT_SUBIMAGE_SELECTED, this);
			this->ProcessEvent(event);
		}
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
		case SubTexture::Type::Level: return std::string("Level #") + std::to_string(this->texture->getLevel());
		case SubTexture::Type::Face: return std::string("Face #") + std::to_string(this->texture->getFace());
		case SubTexture::Type::Layer: return std::string("Layer #") + std::to_string(this->texture->getLayer());
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

	SubTexture* TexToolkitSubimageEntry::getTexture()
	{
		return this->texture.get();
	}

	void TexToolkitSubimageEntry::setColor(const wxColour& color)
	{
		this->SetBackgroundColour(color);
		for (auto& child : this->GetChildren())
		{
			child->SetBackgroundColour(color);
		}
		this->Refresh();
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
		this->setColor(this->highlightColor);
	}

	void TexToolkitSubimageEntry::mouseLeaveEvent(wxMouseEvent& ev)
	{
		const auto wnd = static_cast<wxWindow*>(ev.GetEventObject());
		const auto mousePos = wnd->ClientToScreen(ev.GetPosition());
		if (!this->GetScreenRect().Contains(mousePos))
		{
			if (this->isSelected)
				this->setColor(this->activeColor);
			else
				this->setColor(this->originalColor);
		}
	}

	void TexToolkitSubimageEntry::mouseClickEvent(wxMouseEvent& ev)
	{
		this->select();
	}
}