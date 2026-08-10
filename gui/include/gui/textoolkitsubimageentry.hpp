#ifndef _h_textoolkit_gui_subimage_entry
#define _h_textoolkit_gui_subimage_entry

#include "mainwindow.h"
#include "texture.hpp"

#include <wx/bitmap.h>

namespace textoolkit
{
	class TexToolkitSubimageEntry : public SubimageEntry
	{
	public:
		TexToolkitSubimageEntry(wxWindow* parent);

		void setTexture(Texture* texture);

	private:
		void updatePreview();

		void sizeEvent(wxSizeEvent& ev);

		Texture* texture = nullptr;
		wxBitmap bitmap;
	};
}

#endif
