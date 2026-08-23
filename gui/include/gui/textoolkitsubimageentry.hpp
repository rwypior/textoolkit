#ifndef _h_textoolkit_gui_subimage_entry
#define _h_textoolkit_gui_subimage_entry

#include "mainwindow.h"
#include "texture.hpp"

#include <wx/bitmap.h>

#include <optional>

namespace textoolkit
{
	class TexToolkitSubimageEntry;

	class TexToolkitSubimageEvent : public wxCommandEvent
	{
	public:
		TexToolkitSubimageEntry* entry;

		TexToolkitSubimageEvent(wxEventType eventType, TexToolkitSubimageEntry* entry);
		TexToolkitSubimageEvent* Clone() const;
	};
	wxDECLARE_EVENT(texEVT_SUBIMAGE_SELECTED, TexToolkitSubimageEvent);
	wxDECLARE_EVENT(texEVT_SUBIMAGE_DESELECTED, TexToolkitSubimageEvent);
	wxDECLARE_EVENT(texEVT_SUBIMAGE_IMPORT_REQUESTED, TexToolkitSubimageEvent);

	class TexToolkitSubimageEntry : public SubimageEntry
	{
	public:
		enum class ContextMenuItem
		{
			Import
		};

	public:
		TexToolkitSubimageEntry(wxWindow* parent, std::unique_ptr<SubTexture>&& texture, bool initiallySelected = false);

		void deselect(bool sendEvent = true);
		void select(bool sendEvent = true);

		void setTexture(std::unique_ptr<SubTexture>&& texture);
		void setLineVisibility(bool visible);

		void updatePreview();

		std::string getSubtextureName() const;
		std::string getSubtextureSize() const;

		SubTexture* getTexture();

	private:
		void setColor(const wxColour& color);

		void importImage();

		void sizeEvent(wxSizeEvent& ev);
		void mouseEnterEvent(wxMouseEvent& ev);
		void mouseLeaveEvent(wxMouseEvent& ev);
		void mouseClickEvent(wxMouseEvent& ev);
		void mouseRightClickEvent(wxMouseEvent& ev);
		void menuClicked(wxCommandEvent& ev);

		std::unique_ptr<SubTexture> texture = nullptr;
		wxBitmap bitmap;

		wxColour originalColor;
		wxColour highlightColor;
		wxColour activeColor;
		bool isSelected = false;
	};
}

#endif
