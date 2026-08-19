#include "gui/textoolkitmainwindow.hpp"
#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitnewdialog.hpp"
#include "gui/textoolkitaboutwindow.hpp"
#include "gui/texture.hpp"
#include "gui/textureloader.hpp"

#include <wx/filedlg.h>
#include <wx/stdpaths.h>

namespace textoolkit
{
	TexToolkitMainWindow::TexToolkitMainWindow(renderer::ModelDatabase& modelDatabase, wxWindow* parent)
		: MainWindow(parent)
		, modelDatabase(modelDatabase)
	{
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventNew, this, ID_NEW);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventOpen, this, ID_OPEN);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventAbout, this, ID_ABOUT);
	}

	void TexToolkitMainWindow::openTexture(std::unique_ptr<Texture>&& texture)
	{
		this->notebook->Freeze();
		this->notebook->AddPage(new TexToolkitTextureView(std::move(texture), this->modelDatabase, this->notebook), "New texture", true);
		this->notebook->Thaw();
		this->notebook->Layout();
	}

	void TexToolkitMainWindow::eventNew(wxCommandEvent& event)
	{
		TexToolkitNewDialog newdialog(this);
		if (newdialog.ShowModal() == wxID_OK)
		{
			this->openTexture(newdialog.createTexture());
		}
	}

	void TexToolkitMainWindow::eventOpen(wxCommandEvent& event)
	{
		TextureLoader loader;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Open image", picturesDir, wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		dlg.SetFilterIndex(loader.getFilterIndexAll());

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		auto path = dlg.GetPath().ToStdString();
		this->openTexture(loader.loadTexture(path));
	}

	void TexToolkitMainWindow::eventAbout(wxCommandEvent& event)
	{
		auto about = new TexToolkitAboutWindow(this);
		about->Show();
	}
}