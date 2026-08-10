#include "gui/textoolkitaboutwindow.hpp"

namespace textoolkit
{
	TexToolkitAboutWindow::TexToolkitAboutWindow(wxWindow* parent)
		:
		AboutWindow(parent)
	{
		this->Bind(wxEVT_BUTTON, &TexToolkitAboutWindow::eventOk, this);
	}

	void TexToolkitAboutWindow::eventOk(wxCommandEvent& event)
	{
		this->Close();
	}
}