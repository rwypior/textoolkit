#include "gui/textoolkitmainwindow.hpp"
#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitnewdialog.hpp"
#include "gui/textoolkitaboutwindow.hpp"
#include "gui/textoolkitbatchimportdialog.hpp"
#include "gui/textoolkitprogressdialog.hpp"
#include "gui/textoolkitconfirmdialog.hpp"
#include "gui/dialogchoices.hpp"
#include "gui/texture.hpp"
#include "gui/util.hpp"
#include "texture/textureloader.hpp"
#include "common/threadpool.hpp"

#include <wx/filedlg.h>
#include <wx/stdpaths.h>

#include <fstream>

namespace
{
	constexpr char SaveDlgId[] = "save";
	constexpr char SaveDlgFilter[] = "savefilter";
	constexpr char LoadDlgId[] = "load";
	constexpr char LoadDlgFilter[] = "loadfilter";
}

namespace textoolkit
{
	TexToolkitMainWindow::TexToolkitMainWindow(renderer::ModelDatabase& modelDatabase, wxWindow* parent)
		: MainWindow(parent)
		, modelDatabase(modelDatabase)
	{
		this->loadRecent();
		this->updateMenus();
		this->loadGeometry();

		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventNew, this, ID_NEW);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventOpen, this, ID_OPEN);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventSave, this, ID_SAVE);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventSaveAs, this, ID_SAVE_AS);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventExit, this, ID_EXIT);

		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventImportImage, this, ID_IMPORT_IMAGE);
		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventBatchImport, this, ID_BATCH_IMPORT);

		this->Bind(wxEVT_MENU, &TexToolkitMainWindow::eventAbout, this, ID_ABOUT);

		this->Bind(wxEVT_CLOSE_WINDOW, &TexToolkitMainWindow::eventWindowClose, this);

		this->notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &TexToolkitMainWindow::eventTabClose, this);
	}

	void TexToolkitMainWindow::openTexture(std::unique_ptr<GuiTexture>&& texture, const std::string& name)
	{
		this->notebook->Freeze();
		auto newPage = new TexToolkitTextureView(std::move(texture), this->modelDatabase, this->notebook);
		newPage->Bind(texEVT_TEXTUREVIEW_MODIFIED, &TexToolkitMainWindow::eventModified, this);
		this->notebook->AddPage(newPage, name, true);
		this->notebook->SetPageToolTip(this->notebook->GetPageCount() - 1, newPage->getDescription());
		this->notebook->Thaw();
		this->notebook->Layout();

		this->updateMenus();
	}

	void TexToolkitMainWindow::openTexture(const std::string& path)
	{
		auto texture = this->loadTexture(path);
		auto name = texture->getName();

		if (texture)
		{
			this->openTexture(std::make_unique<GuiTexture>(std::move(*texture)), name);
		}
		else
			wxMessageBox("Unable to open the file", "textoolkit - error", wxOK | wxICON_ERROR, this);
	}

	TexToolkitTextureView* TexToolkitMainWindow::getCurrentTextureView()
	{
		auto page = this->notebook->GetCurrentPage();
		if (!page)
			return nullptr;
		return static_cast<TexToolkitTextureView*>(page);
	}

	std::vector<TexToolkitTextureView*> TexToolkitMainWindow::getModifiedTextureViews()
	{
		std::vector<TexToolkitTextureView*> result;
		for (unsigned int i = 0; i < this->notebook->GetPageCount(); i++)
		{
			auto view = dynamic_cast<TexToolkitTextureView*>(this->notebook->GetPage(i));
			if (!view)
				continue;
			auto& texture = view->getTexture();
			if (texture.isModified())
				result.push_back(view);
		}
		return result;
	}

	std::unique_ptr<Texture> TexToolkitMainWindow::loadTexture(const std::string& path)
	{
		TextureLoader loader;
		std::unique_ptr<Texture> texture;

		FiniteThreadpool tp(1);
		tp.enqueue([&texture, &loader, &path]() {
			texture = loader.loadTexture(path);
		});

		TexToolkitProgressDialog progress(this);
		progress.wait(tp);

		return texture;
	}

	void TexToolkitMainWindow::updateMenus()
	{
		auto view = this->getCurrentTextureView();
		bool viewOpened = !!view;

		this->GetMenuBar()->FindItem(ID_SAVE)->Enable(viewOpened);
		this->GetMenuBar()->FindItem(ID_SAVE_AS)->Enable(viewOpened);

		this->GetMenuBar()->FindItem(ID_IMPORT_IMAGE)->Enable(viewOpened);
		this->GetMenuBar()->FindItem(ID_BATCH_IMPORT)->Enable(viewOpened && view->batchImportCompatible());
	}

	wxString TexToolkitMainWindow::getPageTitle(const GuiTexture& texture)
	{
		wxString name = texture.getName();
		if (texture.isModified())
			name = "* " + name;
		return name;
	}

	void TexToolkitMainWindow::loadGeometry()
	{
		this->RestoreToGeometry(this->geometrySerializer);
	}

	void TexToolkitMainWindow::saveGeometry()
	{
		this->SaveGeometry(this->geometrySerializer);
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

	bool TexToolkitMainWindow::saveAs(GuiTexture& texture)
	{
		TextureLoader loader;
		DialogChoices choices;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Save image", choices.getChoice(SaveDlgId, picturesDir.ToStdString()), texture.getName(), wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		dlg.SetFilterIndex(choices.getChoiceInt(SaveDlgFilter, loader.getFilterIndexAll()));

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return false;

		choices.saveChoice(SaveDlgId, dlg.GetDirectory().ToStdString());
		choices.saveChoiceInt(SaveDlgFilter, dlg.GetFilterIndex());

		auto path = dlg.GetPath().ToStdString();
		this->save(texture, path);

		return true;
	}

	bool TexToolkitMainWindow::save(GuiTexture& texture)
	{
		if (texture.getPath().empty())
			return this->saveAs(texture);
		else
			return this->save(texture, texture.getPath());
	}

	bool TexToolkitMainWindow::save(GuiTexture& texture, const std::string& path)
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
			view->getTexture().setName(name);
			this->notebook->SetPageText(i, this->getPageTitle(texture));
		}

		return true;
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
		DialogChoices choices;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Open image", choices.getChoice(LoadDlgId, picturesDir.ToStdString()), wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		dlg.SetFilterIndex(choices.getChoiceInt(LoadDlgFilter, loader.getFilterIndexAll()));

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		choices.saveChoice(LoadDlgId, dlg.GetDirectory().ToStdString());
		choices.saveChoiceInt(LoadDlgFilter, dlg.GetFilterIndex());

		auto path = dlg.GetPath().ToStdString();
		this->addRecent(path);

		this->openTexture(path);
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
		
		auto texture = this->loadTexture(path);

		this->openTexture(std::make_unique<GuiTexture>(std::move(*texture)), name);
	}

	void TexToolkitMainWindow::eventExit(wxCommandEvent& event)
	{
		this->Close();
	}

	void TexToolkitMainWindow::eventImportImage(wxCommandEvent& event)
	{
		if (auto texview = this->getCurrentTextureView())
			texview->importImage();
	}

	void TexToolkitMainWindow::eventBatchImport(wxCommandEvent& event)
	{
		auto textureView = this->getCurrentTextureView();
		if (!textureView)
			return;

		auto layersCount = textureView->getTexture().getImage().getLayers();
		auto facesCount = textureView->getTexture().getImage().getFaces();
		auto levelsCount = textureView->getTexture().getImage().getLevels();

		auto textureType = textureView->getTexture().getImage().getTextureType();
		auto batchDialog = new TexToolkitBatchImportDialog(textureType, layersCount, facesCount, levelsCount, this);
		if (batchDialog->ShowModal() == wxID_CANCEL)
			return;

		auto items = batchDialog->getItems();
		InterpolationMinMag interpolation(
			batchDialog->getMinInterpolation(),
			batchDialog->getMagInterpolation()
		);

		TextureLoader loader;
		FiniteThreadpool tp;

		for (auto& item : items)
		{
			tp.enqueue([&textureView, &item, &loader, interpolation]() {
				auto tex = GuiTexture(std::move(*loader.loadTexture(item.path)));
				switch (item.type)
				{
				case TexToolkitBatchImportDialog::ImportItem::Type::Face:
					textureView->importFace(tex, 0, textureView->getFaceIndex(item.face), interpolation, false, false);
					break;
				case TexToolkitBatchImportDialog::ImportItem::Type::Layer:
					textureView->importLayer(tex, item.layer, interpolation, false, false);
					break;
				}
			});
		}

		TexToolkitProgressDialog progress(this);
		progress.wait(tp);

		textureView->updateAllPreviews();
		textureView->reuploadTexture();
	}

	void TexToolkitMainWindow::eventAbout(wxCommandEvent& event)
	{
		auto about = new TexToolkitAboutWindow(this);
		about->Show();
	}

	void TexToolkitMainWindow::eventModified(TexToolkitTextureViewEvent& event)
	{
		auto pageid = this->notebook->FindPage(event.view);
		if (pageid == wxNOT_FOUND)
			return;
		auto& texture = event.view->getTexture();
		this->notebook->SetPageText(pageid, this->getPageTitle(texture));
	}

	void TexToolkitMainWindow::eventTabClose(wxAuiNotebookEvent& event)
	{
		auto view = dynamic_cast<TexToolkitTextureView*>(this->notebook->GetPage(event.GetSelection()));
		if (view)
		{
			auto& texture = view->getTexture();
			if (texture.isModified())
			{
				auto choice = wxMessageBox(wxString::Format("Texture \"%s\" has unsaved changes. Save before closing?", texture.getName()),
					"Confirm", wxICON_QUESTION | wxYES_NO | wxCANCEL);
				switch (choice)
				{
				case wxYES:
					if (this->save(texture))
						event.Skip();
					else
						event.Veto();
					break;
				case wxNO:
					event.Skip();
					break;
				case wxCANCEL:
					event.Veto();
					break;
				}
			}
		}
	}

	void TexToolkitMainWindow::eventWindowClose(wxCloseEvent& event)
	{
		auto modifiedViews = this->getModifiedTextureViews();
		if (modifiedViews.empty())
		{
			this->saveGeometry();
			event.Skip();
			return;
		}

		bool applyForAll = false;
		int choice = std::numeric_limits<int>::max();

		for (auto& view : modifiedViews)
		{
			auto& texture = view->getTexture();
			auto dlg = new TexToolkitConfirmDialog(this, wxString::Format("Texture \"%s\" has unsaved changes. Save before closing?", texture.getName()),
				"Confirm");
			
			if (!applyForAll)
			{
				choice = dlg->ShowModal();
				applyForAll = dlg->applyForAll();
			}

			switch (choice)
			{
			case wxYES:
				if (!this->save(texture))
				{
					event.Veto();
					return;
				}
				break;
			case wxNO:
				break;
			case wxCANCEL:
				event.Veto();
				return;
			}
		}
		
		this->saveGeometry();
		event.Skip();
	}
}