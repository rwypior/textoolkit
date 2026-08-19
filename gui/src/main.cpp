#include "gui/textoolkitmainwindow.hpp"
#include "gui/util.hpp"
#include "common/textoolkit.hpp"
#include "common/logger.hpp"
#include "renderer/modeldatabase.hpp"
#include "renderer/model.hpp"

#include "logo.xpm"

#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/time.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>

namespace
{
	const wxCmdLineEntryDesc g_cmdLineDesc[] =
	{
		{ wxCMD_LINE_PARAM, "file", "file", "File to load",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "v", "verbose", "",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_HIDDEN },

		{ wxCMD_LINE_NONE }
	};

	static constexpr char logFileName[] = "textoolkit.log";

	std::string getLogPath()
	{
		wxFileName path(wxStandardPaths::Get().GetUserLocalDataDir(), logFileName);
		return path.GetFullPath().ToStdString();
	}
}

namespace textoolkit
{
	class TexToolkit : public wxApp
	{
	public:
		TexToolkit()
			: wxApp()
		{
			SetVendorName("TexToolkit");
			SetAppName("TexToolkit");

			Logger::init(getLogPath());

			this->SetAppearance(Appearance::System);

			wxStandardPaths& sp = wxStandardPaths::Get();
			sp.UseAppInfo(wxStandardPaths::AppInfo_AppName);
		}

		bool OnInit() override
		{
			if (!wxApp::OnInit())
				return false;

			wxInitAllImageHandlers();

			wxFileName apppath(wxStandardPaths::Get().GetExecutablePath());

			wxFileName respath(apppath);
			respath.AppendDir("res");

			this->mainWindow = new TexToolkitMainWindow(this->modelDatabase, nullptr);

#ifdef TEXTOOLKIT_WINDOWS
			wxFileName iconpath(respath.GetPath(), "logo.ico");
			this->mainWindow->SetIcons(wxIconBundle(iconpath.GetFullPath()));
#else
			this->mainWindow->SetIcon(logo_xpm);
#endif

			this->loadModels();

			this->mainWindow->Show();

			return true;
		}

		void OnInitCmdLine(wxCmdLineParser& parser) override
		{
			parser.SetDesc(g_cmdLineDesc);
			parser.SetSwitchChars(wxT("-"));
		}

	private:
		void loadModels()
		{
			auto modelPaths = getModels();
			auto result = this->modelDatabase.loadModels(modelPaths);
		}

	private:
		TexToolkitMainWindow* mainWindow = nullptr;
		renderer::ModelDatabase modelDatabase;
	};
}

wxIMPLEMENT_APP(textoolkit::TexToolkit);