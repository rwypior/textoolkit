#include "gui/textoolkitmainwindow.hpp"
#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitnewdialog.hpp"
#include "gui/textoolkitaboutwindow.hpp"
#include "gui/texture.hpp"
#include "gui/textureloader.hpp"
#include "gui/util.hpp"

#include <wx/filedlg.h>
#include <wx/stdpaths.h>

#include <fstream>

namespace textoolkit
{
	TexToolkitMainWindow::TexToolkitMainWindow(renderer::ModelDatabase& modelDatabase, wxWindow* parent)
		: MainWindow(parent)
		, modelDatabase(modelDatabase)
	{
		this->loadRecent();

		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventNew, this, ID_NEW);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventOpen, this, ID_OPEN);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventSave, this, ID_SAVE);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventSaveAs, this, ID_SAVE_AS);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventAbout, this, ID_ABOUT);
	}

	void TexToolkitMainWindow::openTexture(std::unique_ptr<Texture>&& texture, const std::string& name)
	{
		this->notebook->Freeze();
		this->notebook->AddPage(new TexToolkitTextureView(std::move(texture), this->modelDatabase, this->notebook), name, true);
		this->notebook->Thaw();
		this->notebook->Layout();
	}

	TexToolkitTextureView* TexToolkitMainWindow::getCurrentTextureView()
	{
		auto page = this->notebook->GetCurrentPage();
		if (!page)
			return nullptr;
		return static_cast<TexToolkitTextureView*>(page);
	}

	void TexToolkitMainWindow::loadRecent()
	{
		std::ifstream str(getRecentPath());
		if (str.is_open())
		{
			std::vector<std::string> lines;
			for (std::string line; std::getline(str, line); )
			{
				if (line.empty())
					continue;

				lines.push_back(line);
			}

			if (lines.empty())
				return;

			// Delete old items
			std::queue<wxMenuItem*> itemsToDelete;
			int id = static_cast<int>(MenuCommands::Recent);
			while (auto item = this->file->FindItem(id))
			{
				itemsToDelete.push(item);
				id++;
			}
			while (!itemsToDelete.empty())
			{
				this->file->Remove(itemsToDelete.front());
				itemsToDelete.pop();
			}

			// Insert new items
			this->file->AppendSeparator()->SetId(static_cast<int>(MenuCommands::Recent));

			size_t cnt = 1;
			for (const auto& line : lines)
			{
				int id = static_cast<int>(MenuCommands::Recent) + cnt;
				this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventOpenRecent, this, id);
				this->file->Append(id, line);
				cnt++;
			}
		}
	}

	void TexToolkitMainWindow::addRecent(const std::string& path)
	{
		// Load existing items
		std::list<std::string> list;				
		int id = static_cast<int>(MenuCommands::Recent);
		while (auto item = this->file->FindItem(id))
		{
			list.push_back(item->GetItemLabelText().ToStdString());
			id++;
		}

		// Handle duplicates
		auto it = std::find(list.begin(), list.end(), path);
		if (it == list.end())
		{
			if (list.size() >= this->maxRecentCount)
				list.pop_back();
		}
		else
		{
			list.erase(it);
		}
		list.push_front(path);

		// Save list
		{
			std::ofstream str(getRecentPath());
			if (str.is_open())
			{
				for (const std::string& item : list)
				{
					str << item + "\n";
				}
			}
		}

		this->loadRecent();
	}

	void TexToolkitMainWindow::saveAs(Texture& texture)
	{
		TextureLoader loader;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Save image", picturesDir, texture.getName(), wildcard, wxFD_SAVE);
		dlg.SetFilterIndex(loader.getFilterIndexAll());

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		auto path = dlg.GetPath().ToStdString();
		this->save(texture, path);
	}

	void TexToolkitMainWindow::save(Texture& texture)
	{
		if (texture.getPath().empty())
			this->saveAs(texture);
		else
			this->save(texture, texture.getPath());
	}

	void TexToolkitMainWindow::save(Texture& texture, const std::string& path)
	{
		texture.save(path);

		// Rename page
		for (unsigned int i = 0; i < this->notebook->GetPageCount(); i++)
		{
			auto page = this->notebook->GetPage(i);
			if (!page)
				continue;

			auto view = static_cast<TexToolkitTextureView*>(page);
			if (&view->getTexture() != &texture)
				continue;

			auto name = wxFileName(path).GetFullName().ToStdString();
			this->notebook->SetPageText(i, name);
		}
	}

	void TexToolkitMainWindow::eventNew(wxCommandEvent& event)
	{
		TexToolkitNewDialog newdialog(this);
		if (newdialog.ShowModal() == wxID_OK)
		{
			this->openTexture(newdialog.createTexture(), newdialog.getTextureName());
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
		this->addRecent(path);

		auto name = wxFileName(path).GetFullName().ToStdString();
		this->openTexture(loader.loadTexture(path), name);
	}

	void TexToolkitMainWindow::eventSave(wxCommandEvent& event)
	{
		auto currentView = this->getCurrentTextureView();
		this->save(currentView->getTexture());
	}

	void TexToolkitMainWindow::eventSaveAs(wxCommandEvent& event)
	{
		auto currentView = this->getCurrentTextureView();
		this->saveAs(currentView->getTexture());		
	}

	void TexToolkitMainWindow::eventOpenRecent(wxCommandEvent& event)
	{
		wxMenu* menu = static_cast<wxMenu*>(event.GetEventObject());
		auto* item = menu->FindItem(event.GetId());
		auto path = item->GetItemLabelText().ToStdString();
		
		this->addRecent(path); // Bump up in the recent list
		auto name = wxFileName(path).GetFullName().ToStdString();
		TextureLoader loader;
		this->openTexture(loader.loadTexture(path), name);
	}

	void TexToolkitMainWindow::eventAbout(wxCommandEvent& event)
	{
		auto about = new TexToolkitAboutWindow(this);
		about->Show();
	}
}