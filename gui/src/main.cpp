#include "gui/textoolkitmainwindow.hpp"
#include "common/textoolkit.hpp"

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

			this->mainWindow = new TexToolkitMainWindow(nullptr);

#ifdef TEXTOOLKIT_WINDOWS
			wxFileName iconpath(respath.GetPath(), "logo.ico");
			this->mainWindow->SetIcons(wxIconBundle(iconpath.GetFullPath()));
#else
			this->mainWindow->SetIcon(logo_xpm);
#endif

			this->mainWindow->Show();

			return true;
		}

		void OnInitCmdLine(wxCmdLineParser& parser) override
		{
			parser.SetDesc(g_cmdLineDesc);
			parser.SetSwitchChars(wxT("-"));
		}

	private:
		TexToolkitMainWindow* mainWindow = nullptr;
	};
}

wxIMPLEMENT_APP(textoolkit::TexToolkit);