#include "gui/util.hpp"
#include "common/logger.hpp"
#include "common/util.hpp"
#include "renderer/displaymode.hpp"

#include <iniparser/iniparser.hpp>

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

	std::string getDisplayModePath()
	{
		wxFileName apppath(wxStandardPaths::Get().GetExecutablePath());

		wxFileName respath(apppath);
		respath.SetName("displaymodes");
		respath.SetExt("ini");

		return respath.GetFullPath().ToStdString();
	}

	std::string getRecentPath()
	{
		wxFileName path(wxStandardPaths::Get().GetUserLocalDataDir(), "recent.txt");
		return path.GetFullPath().ToStdString();
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

	std::map<std::string, renderer::DisplayMode> loadDisplayModes(const std::string& path)
	{
		Ini::Parser parser;
		Ini::Model model;
		auto result = parser.readFile(path, model);
		if (result.code != Ini::Result::Code::OK)
			return {};

		std::map<std::string, renderer::DisplayMode> displayModes;
		for (auto& section : model.sections)
		{
			const auto& key = section.first;
			auto itName = section.second.find("name");
			auto itModel = section.second.find("model");
			auto itShader = section.second.find("shader");
			auto itDefault = section.second.find("default");
			auto itProperties = section.second.find("properties");

			if (itName == section.second.end())
			{
				Logger::getLogger() << path << ": doesn't have 'name' entry";
				return {};
			}
			if (itModel == section.second.end())
			{
				Logger::getLogger() << path << ": doesn't have 'model' entry";
				return {};
			}
			if (itShader == section.second.end())
			{
				Logger::getLogger() << path << ": doesn't have 'shader' entry";
				return {};
			}

			std::unordered_set<std::string> defaults;
			if (itDefault != section.second.end())
			{
				auto splitted = split(itDefault->second->toString(), ",");
				std::transform(splitted.begin(), splitted.end(), splitted.begin(), [](std::string& el) { return trimmed(el); });
				defaults.insert(splitted.begin(), splitted.end());
			}

			std::unordered_set<std::string> properties;
			if (itProperties != section.second.end())
			{
				auto splitted = split(itProperties->second->toString(), ",");
				std::transform(splitted.begin(), splitted.end(), splitted.begin(), [](std::string& el) { return trimmed(el); });
				properties.insert(splitted.begin(), splitted.end());
			}

			displayModes[key] = renderer::DisplayMode(
				trimmed(itName->second->toString()),
				trimmed(itModel->second->toString()),
				trimmed(itShader->second->toString()),
				defaults,
				properties
			);
		}

		return displayModes;
	}

	renderer::RenderProperty createRenderProperty(const wxVariant& data)
	{
		if (
			data.IsType("long") ||
			data.IsType("longlong") ||
			data.IsType("char")
			)
			return data.GetInteger();

		if (
			data.IsType("double")
			)
			return static_cast<float>(data.GetDouble());

		if (
			data.IsType("bool")
			)
			return data.GetBool();

		return data.GetInteger();
	}

	bool matchWildcard(const std::string& string, const std::string& wildcard)
	{
		const auto splitted = split(wildcard, "|");
		const auto wildcards = splitted.size() == 1 ? splitted : split(splitted.at(1), ";");
		for (const auto& wildcard : wildcards)
		{
			if (wxMatchWild(wildcard, string))
				return true;
		}
		return false;
	}

	// Model traverser

	WildcardFileTraverser::WildcardFileTraverser(const std::string& wildcard)
		: wildcard(wildcard)
	{
	}

	wxDirTraverseResult WildcardFileTraverser::OnFile(const wxString& filename)
	{
		//if (wxMatchWild(this->wildcard, filename))
		if (matchWildcard(filename.ToStdString(), this->wildcard))
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

	// Freeze guard

	FreezeGuard::FreezeGuard(wxWindowBase& window)
		: window(window)
	{
		window.Freeze();
	}

	FreezeGuard::~FreezeGuard()
	{
		window.Thaw();
	}
}