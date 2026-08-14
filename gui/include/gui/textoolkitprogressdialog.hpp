#ifndef _h_textoolkit_gui_progressdialog
#define _h_textoolkit_gui_progressdialog

#include "mainwindow.h"

#include <functional>

namespace textoolkit
{
	class TexToolkitProgressDialog : public ProgressDialog
	{
	public:
		using CancelRequest = std::function<void()>;

	public:
		TexToolkitProgressDialog(wxWindow* parent, CancelRequest cancelRequest = {});

		std::function<void(unsigned int total, unsigned int completed)> createNotifier();

	private:
		void timerEvent(wxTimerEvent& event);

		unsigned int total = 0;
		unsigned int completed = 0;
		wxTimer timer;
		CancelRequest cancelRequest = {};
	};
}

#endif