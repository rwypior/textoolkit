#include "gui/textoolkitprogressdialog.hpp"

namespace textoolkit
{
	TexToolkitProgressDialog::TexToolkitProgressDialog(wxWindow* parent, CancelRequest cancelRequest)
		: ProgressDialog(parent)
		, cancelRequest(cancelRequest)
		, timer(this, wxID_ANY)
	{
		this->timer.Start(50);

		if (!this->cancelRequest)
		{
			this->cancelButton->Hide();
			this->Layout();
		}

		this->timer.Bind(wxEVT_TIMER, &TexToolkitProgressDialog::timerEvent, this, this->timer.GetId());
	}

	std::function<void(unsigned int total, unsigned int completed)> TexToolkitProgressDialog::createNotifier()
	{
		return [this](unsigned int total, unsigned int completed) {
			this->total = total;
			this->completed = completed;
		};
	}

	void TexToolkitProgressDialog::timerEvent(wxTimerEvent& event)
	{
		this->progressBar->SetRange(this->total);
		this->progressBar->SetValue(this->completed);
	}
}