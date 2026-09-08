#ifndef _h_textoolkit_gui_progressdialog
#define _h_textoolkit_gui_progressdialog

#include "mainwindow.h"

#include <functional>

namespace textoolkit
{
	class Threadpool;

	class TexToolkitProgressDialog : public ProgressDialog
	{
	public:
		using CancelRequest = std::function<void()>;

	public:
		unsigned int indeterminatePulsePeriod = 1000;

		TexToolkitProgressDialog(wxWindow* parent, CancelRequest cancelRequest = {});

		void wait(Threadpool& threadpool);

		std::function<void(unsigned int total, unsigned int completed)> createNotifier();
		void makeIndeterminate();

	private:
		void updateProgress();

		void abortRequested(wxCommandEvent& event);

		unsigned int total = 0;
		unsigned int completed = 0;
		bool indeterminate = true;
		wxMilliClock_t lastUpdate = 0;
		bool aborted = false;
		CancelRequest cancelRequest = {};
	};
}

#endif