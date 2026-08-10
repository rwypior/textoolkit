#include "gui/textoolkitmainwindow.hpp"
#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitnewdialog.hpp"
#include "gui/textoolkitaboutwindow.hpp"
#include "gui/texture.hpp"

namespace textoolkit
{
	TexToolkitMainWindow::TexToolkitMainWindow(wxWindow* parent)
		:
		MainWindow(parent)
	{
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventNew, this, ID_NEW);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventOpen, this, ID_OPEN);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventAbout, this, ID_ABOUT);
	}

	void TexToolkitMainWindow::openNewTexture(std::unique_ptr<Texture>&& texture)
	{
		this->notebook->Freeze();
		this->notebook->AddPage(new TexToolkitTextureView(std::move(texture), this->notebook), "New texture", true);
		this->notebook->Thaw();
	}

	void TexToolkitMainWindow::eventNew(wxCommandEvent& event)
	{
		TexToolkitNewDialog newdialog(this);
		if (newdialog.ShowModal() == wxOK)
		{
			this->openNewTexture(newdialog.createTexture());
		}		
	}

	void TexToolkitMainWindow::eventOpen(wxCommandEvent& event)
	{

	}

	void TexToolkitMainWindow::eventAbout(wxCommandEvent& event)
	{
		TexToolkitAboutWindow about(this);
		about.Show();
	}
}