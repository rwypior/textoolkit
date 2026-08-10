#ifndef _h_textoolkit_gui_aboutwindow
#define _h_textoolkit_gui_aboutwindow

#include "mainwindow.h"

namespace textoolkit
{
	class TexToolkitAboutWindow : public AboutWindow
	{
	public:
		TexToolkitAboutWindow(wxWindow* parent);

	private:
		void eventOk(wxCommandEvent& event);
	};
}

#endif
