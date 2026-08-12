#ifndef _h_textoolkit_gui_subimage_entry
#define _h_textoolkit_gui_subimage_entry

#include "mainwindow.h"
#include "texture.hpp"

#include <wx/bitmap.h>

#include <optional>

namespace textoolkit
{
	class TexToolkitSubimageEntry : public SubimageEntry
	{
	public:
		TexToolkitSubimageEntry(std::unique_ptr<SubTexture>&& texture, wxWindow* parent);

		void setTexture(std::unique_ptr<SubTexture>&& texture);
		void setLineVisibility(bool visible);

		std::string getSubtextureName() const;
		std::string getSubtextureSize() const;

	private:
		void updatePreview();

		void sizeEvent(wxSizeEvent& ev);
		void mouseEnterEvent(wxMouseEvent& ev);
		void mouseLeaveEvent(wxMouseEvent& ev);

		std::unique_ptr<SubTexture> texture = nullptr;
		wxBitmap bitmap;

		std::optional<wxColour> backgroundColor;
	};
}

#endif
