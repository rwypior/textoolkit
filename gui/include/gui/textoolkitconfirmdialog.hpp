#ifndef textoolkit_gui_confirmdialog
#define textoolkit_gui_confirmdialog

#include "gui/mainwindow.h"

namespace textoolkit
{
	class TexToolkitConfirmDialog : public ConfirmDialog
	{
	public:
		TexToolkitConfirmDialog(
			wxWindow* parent, 
			const wxString& message,
			const wxString& title = wxEmptyString, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxSize(432, 130), 
			long style = wxDEFAULT_DIALOG_STYLE,
			wxWindowID id = wxID_ANY 
		);

		bool applyForAll() const;

	private:
		void yesClicked(wxCommandEvent& event);
		void noClicked(wxCommandEvent& event);
		void cancelClicked(wxCommandEvent& event);
	};
}

#endif
