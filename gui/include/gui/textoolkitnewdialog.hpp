#ifndef _h_textoolkit_gui_newdialog
#define _h_textoolkit_gui_newdialog

#include "mainwindow.h"
#include "texture.hpp"

#include <memory>

namespace textoolkit
{
	class TexToolkitNewDialog : public NewDialog
	{
	public:
		TexToolkitNewDialog(wxWindow* parent);
		~TexToolkitNewDialog();

		std::unique_ptr<GuiTexture> createTexture();
		std::string getTextureName() const;

	private:
		void updatePropertiesWidget();

		void typeChangedEvent(wxCommandEvent& event);
		void okEvent(wxCommandEvent& event);
		void cancelEvent(wxCommandEvent& event);
	};
}

#endif
