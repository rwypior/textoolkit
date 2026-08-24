#ifndef _h_textoolkit_gui_mainwindow
#define _h_textoolkit_gui_mainwindow

#include "mainwindow.h"

#include <memory>

namespace textoolkit
{
	class GuiTexture;
	class TexToolkitTextureView;

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

	private:
		void loadRecent();
		void addRecent(const std::string& path);
		void saveAs(GuiTexture& texture);
		void save(GuiTexture& texture);
		void save(GuiTexture& texture, const std::string& path);

		void eventNew(wxCommandEvent& event);
		void eventOpen(wxCommandEvent& event);
		void eventOpenRecent(wxCommandEvent& event);
		void eventSave(wxCommandEvent& event);
		void eventSaveAs(wxCommandEvent& event);
		void eventAbout(wxCommandEvent& event);

		renderer::ModelDatabase& modelDatabase;
	};
}

#endif
