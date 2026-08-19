#include "gui/util.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>

namespace textoolkit
{
	std::string getModelsPath()
	{
		wxFileName apppath(wxStandardPaths::Get().GetExecutablePath());

		wxFileName respath(apppath);
		respath.AppendDir("models");

		return respath.GetPath().ToStdString();
	}
	
	std::string getShadersPath()
	{
		wxFileName apppath(wxStandardPaths::Get().GetExecutablePath());

		wxFileName respath(apppath);
		respath.AppendDir("shaders");

		return respath.GetPath().ToStdString();
	}

	wxArrayString getModels()
	{
		WildcardFileTraverser trav("*.obj");
		std::string modelsPath = getModelsPath();
		wxDir dir(modelsPath);
		dir.Traverse(trav);
		return trav.getFoundFiles();
	}

	wxArrayString getShaders()
	{
		WildcardFileTraverser trav("*.sdr");
		std::string shadersPath = getShadersPath();
		wxDir dir(shadersPath);
		dir.Traverse(trav);
		return trav.getFoundFiles();
	}

	// Model traverser

	WildcardFileTraverser::WildcardFileTraverser(const std::string& wildcard)
		: wildcard(wildcard)
	{
	}

	wxDirTraverseResult WildcardFileTraverser::OnFile(const wxString& filename)
	{
		if (wxMatchWild("*.obj", filename))
			this->files.Add(filename);
		return wxDIR_CONTINUE;
	}

	wxDirTraverseResult WildcardFileTraverser::OnDir(const wxString& /*dirname*/)
	{
		return wxDIR_CONTINUE;
	}

	const wxArrayString& WildcardFileTraverser::getFoundFiles() const
	{
		return this->files;
	}
}