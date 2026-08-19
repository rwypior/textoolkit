#ifndef _h_textoolkit_gui_mainwindow
#define _h_textoolkit_gui_mainwindow

#include "mainwindow.h"

#include <memory>

namespace textoolkit
{
	class Texture;

	namespace renderer
	{
		class ModelDatabase;
	}

	class TexToolkitMainWindow : public MainWindow
	{
	public:
		TexToolkitMainWindow(renderer::ModelDatabase& modelDatabase, wxWindow* parent);

		void openTexture(std::unique_ptr<Texture>&& texture);

	private:
		void eventNew(wxCommandEvent& event);
		void eventOpen(wxCommandEvent& event);
		void eventAbout(wxCommandEvent& event);

		renderer::ModelDatabase& modelDatabase;
	};
}

#endif
