#ifndef _h_textoolkit_gui_mainwindow
#define _h_textoolkit_gui_mainwindow

#include "mainwindow.h"
#include "windowgeometry.hpp"

#include <memory>
#include <vector>

namespace textoolkit
{
	class GuiTexture;
	class TexToolkitTextureView;
	class Texture;
	class TexToolkitTextureViewEvent;

	namespace renderer
	{
		class ModelDatabase;
	}

	class TexToolkitMainWindow : public MainWindow
	{
	public:
		enum class MenuCommands
		{
			Recent = 10000
		};

		static constexpr unsigned int maxRecentCount = 10;

	public:
		TexToolkitMainWindow(renderer::ModelDatabase& modelDatabase, wxWindow* parent);

		void openTexture(std::unique_ptr<GuiTexture>&& texture, const std::string& name);
		void openTexture(const std::string& path);
		TexToolkitTextureView* getCurrentTextureView();

		std::vector<TexToolkitTextureView*> getModifiedTextureViews();

	private:
		std::unique_ptr<Texture> loadTexture(const std::string& path);
		void updateMenus();
		wxString getPageTitle(const GuiTexture& texture);

		void loadGeometry();
		void saveGeometry();

		void loadRecent();
		void addRecent(const std::string& path);
		bool saveAs(GuiTexture& texture);
		bool save(GuiTexture& texture);
		bool save(GuiTexture& texture, const std::string& path);

		void eventNew(wxCommandEvent& event);
		void eventOpen(wxCommandEvent& event);
		void eventOpenRecent(wxCommandEvent& event);
		void eventSave(wxCommandEvent& event);
		void eventSaveAs(wxCommandEvent& event);
		void eventImportImage(wxCommandEvent& event);
		void eventBatchImport(wxCommandEvent& event);
		void eventExit(wxCommandEvent& event);
		void eventAbout(wxCommandEvent& event);

		void eventModified(TexToolkitTextureViewEvent& event);
		void eventTabClose(wxAuiNotebookEvent& event);
		void eventWindowClose(wxCloseEvent& event);

		renderer::ModelDatabase& modelDatabase;
		WindowGeometrySerializer geometrySerializer;
	};
}

#endif
