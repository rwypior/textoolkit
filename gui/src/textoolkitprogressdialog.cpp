#include "gui/textoolkitprogressdialog.hpp"
#include "common/util.hpp"
#include "common/threadpool.hpp"

#include <wx/utils.h>

namespace textoolkit
{
	TexToolkitProgressDialog::TexToolkitProgressDialog(wxWindow* parent, CancelRequest cancelRequest)
		: ProgressDialog(parent)
		, cancelRequest(cancelRequest)
	{
		if (!this->cancelRequest)
		{
			this->cancelButton->Hide();
			this->Layout();
		}
	}

	void TexToolkitProgressDialog::wait(Threadpool& threadpool)
	{
		wxWindowDisabler disabler(this);
		this->Show();
		threadpool.start();
		this->aborted = false;
		while (threadpool.isWorking())
		{
			this->updateProgress();
			if (this->aborted)
			{
				this->aborted = false;
				threadpool.abort();
			}
		}
		this->Destroy();
	}

	std::function<void(unsigned int total, unsigned int completed)> TexToolkitProgressDialog::createNotifier()
	{
		return [this](unsigned int total, unsigned int completed) {
			this->indeterminate = false;
			this->total = total;
			this->completed = completed;
		};
	}

	void TexToolkitProgressDialog::makeIndeterminate()
	{
		this->total = 1;
		this->completed = 0;
		this->indeterminate = true;
	}

	void TexToolkitProgressDialog::updateProgress()
	{
		this->Update();
		if (this->indeterminate)
		{
			auto currentMillis = wxGetLocalTimeMillis();
			if (currentMillis - this->lastUpdate > 50)
			{
				this->progressBar->Pulse();
				this->lastUpdate = currentMillis;
			}
		}
		else
		{
			this->progressBar->SetRange(this->total);
			this->progressBar->SetValue(this->completed);
		}
	}

	void TexToolkitProgressDialog::abortRequested(wxCommandEvent& /*event*/)
	{
		this->aborted = true;
	}
}