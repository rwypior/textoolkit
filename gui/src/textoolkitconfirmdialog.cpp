#include "gui/textoolkitconfirmdialog.hpp"
#include "gui/autowraplabel.hpp"

namespace textoolkit
{
	TexToolkitConfirmDialog::TexToolkitConfirmDialog(
		wxWindow* parent,
		const wxString& message,
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size,
		long style,
		wxWindowID id
	)
		: ConfirmDialog(parent, id, title, pos, size, style)
	{
		this->label->SetLabel(message);

		this->yesbutton->Bind(wxEVT_BUTTON, &TexToolkitConfirmDialog::yesClicked, this);
		this->nobutton->Bind(wxEVT_BUTTON, &TexToolkitConfirmDialog::noClicked, this);
		this->cancelbutton->Bind(wxEVT_BUTTON, &TexToolkitConfirmDialog::cancelClicked, this);
	}

	bool TexToolkitConfirmDialog::applyForAll() const
	{
		return this->applyforallcheckbox->IsChecked();
	}

	void TexToolkitConfirmDialog::yesClicked(wxCommandEvent& event)
	{
		this->EndModal(wxYES);
	}

	void TexToolkitConfirmDialog::noClicked(wxCommandEvent& event)
	{
		this->EndModal(wxNO);
	}
	
	void TexToolkitConfirmDialog::cancelClicked(wxCommandEvent& event)
	{
		this->EndModal(wxCANCEL);
	}
}
