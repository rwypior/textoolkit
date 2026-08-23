#ifndef _h_textoolkit_gui_mainwindow
#define _h_textoolkit_gui_mainwindow

#include "mainwindow.h"

#include <memory>

namespace textoolkit
{
	class Texture;
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

		void openTexture(std::unique_ptr<Texture>&& texture, const std::string& name);
		TexToolkitTextureView* getCurrentTextureView();

	private:
		void loadRecent();
		void addRecent(const std::string& path);
		void saveAs(Texture& texture);
		void save(Texture& texture);
		void save(Texture& texture, const std::string& path);

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
