#include "gui/textoolkitbatchimportdialog.hpp"
#include "gui/autowraplabel.hpp"
#include "gui/util.hpp"
#include "common/util.hpp"
#include "texture/textureloader.hpp"

#include <wx/stdpaths.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>

#include <map>
#include <regex>

namespace
{
	class FileNameRenderer : public wxDataViewCustomRenderer
	{
	public:
		FileNameRenderer(
			const wxString& varianttype = GetDefaultType(), 
			wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT, 
			int align = wxAlignment::wxALIGN_LEFT
		)
			: wxDataViewCustomRenderer(varianttype, mode, align)
		{
		}

		bool SetValue(const wxVariant& val) override
		{
			this->path = val.GetString();
			return true;
		}

		bool GetValue(wxVariant& val) const override
		{
			val = this->path;
			return true;
		}

		wxSize GetSize() const override
		{
			wxClientDC dc(this->GetView());
			dc.SetFont(this->GetView()->GetFont());
			return dc.GetTextExtent(this->getFilename());
		}

		bool Render(wxRect cell, wxDC* dc, int state) override
		{
			dc->SetFont(this->GetView()->GetFont());
			dc->DrawText(this->getFilename(), cell.GetTopLeft() + wxPoint(4, 4));
			return true;
		}

	private:
		wxString path;

		wxString getFilename() const
		{
			return wxFileName(this->path).GetFullName();
		}
	};

	static constexpr unsigned int ColumnPath = 0;
	static constexpr unsigned int ColumnImport = 1;
	static constexpr unsigned int ColumnTarget = 2;
}

namespace textoolkit
{
	TexToolkitBatchImportDialog::TexToolkitBatchImportDialog(
		Image::TextureType textureType, 
		unsigned int layers,
		unsigned int faces,
		unsigned int levels,
		wxWindow* parent
	)
		: BatchImportDialog(parent)
		, textureType(textureType)
		, layers(layers)
		, faces(faces)
		, levels(levels)
	{
		this->imageList->AppendColumn(new wxDataViewColumn("Path", new FileNameRenderer(), ColumnPath, 200));
		this->imageList->AppendToggleColumn("Import", wxDATAVIEW_CELL_ACTIVATABLE, 50, wxAlignment::wxALIGN_CENTER);

		wxString typeInfo;
		switch (textureType)
		{
		case Image::TextureType::TextureCube:
		{
			typeInfo = "\n" R"INFO(Cubemap textures follow the pattern name_[positive/negative]_[x/y/z] or name_[w/n/e/s/u/d].)INFO"
				"\n" R"INFO(For example name_positive_x, name_negative_x, name_positive_y, name_negative_y, etc.)INFO";
			this->regexes.push_back(R"REGEX((.+)_((?:positive|negative)_(?:[xyz]))\.(?:.*))REGEX");
			this->regexes.push_back(R"REGEX((.+)_([wnesud])\.(?:.*))REGEX");
			this->maxItems = faces;
			wxArrayString choices;
			for (auto& [s, e] : Image::getCubeFaceMap())
			{
				choices.Add(s);
			}
			this->imageList->AppendColumn(new wxDataViewColumn("Target", new wxDataViewChoiceRenderer(choices), ColumnTarget, 100));
			break;
		}
		case Image::TextureType::Texture2DArray:
			typeInfo = "\n" R"INFO(Array textures follow the pattern name_[n], eg. name_0, name_1, name_2, etc.)INFO";
			this->regexes.push_back(R"REGEX((.+)_(\d+)\.(?:.*))REGEX");
			this->imageList->AppendColumn(new wxDataViewColumn("Target", new wxDataViewSpinRenderer(0, std::numeric_limits<int>::max()), ColumnTarget, 100));
			this->maxItems = layers;
			break;
		}

		this->updateMax();

		wxString info = this->infoLabel->GetLabel();
		info.Replace("%pattern_info", typeInfo);
		this->infoLabel->SetLabel(info);

		auto interpolationMap = getInterpolationMap();
		for (auto [e, s] : interpolationMap)
		{
			int minI = this->filterMin->Append(s, reinterpret_cast<void*>(e));
			int magI = this->filterMag->Append(s, reinterpret_cast<void*>(e));

			if (e == Interpolation::Bicubic)
				this->filterMin->SetSelection(minI);
			else if (e == Interpolation::NearestNeighbors)
				this->filterMag->SetSelection(magI);
		}

		this->importFolderButton->Bind(wxEVT_BUTTON, &TexToolkitBatchImportDialog::importFolderEvent, this);
		this->importFilesButton->Bind(wxEVT_BUTTON, &TexToolkitBatchImportDialog::importFilesEvent, this);
		this->importButton->Bind(wxEVT_BUTTON, &TexToolkitBatchImportDialog::importEvent, this);
		this->cancelButton->Bind(wxEVT_BUTTON, &TexToolkitBatchImportDialog::cancelEvent, this);
	}

	Interpolation TexToolkitBatchImportDialog::getMinInterpolation() const
	{
		auto data = this->filterMin->GetClientData(this->filterMin->GetCurrentSelection());
		return static_cast<Interpolation>(reinterpret_cast<size_t>(data));
	}

	Interpolation TexToolkitBatchImportDialog::getMagInterpolation() const
	{
		auto data = this->filterMag->GetClientData(this->filterMag->GetCurrentSelection());
		return static_cast<Interpolation>(reinterpret_cast<size_t>(data));
	}

	TexToolkitBatchImportDialog::ImportItems TexToolkitBatchImportDialog::getItems() const
	{
		ImportItems result;

		for (unsigned int i = 0; i < this->imageList->GetItemCount(); i++)
		{
			wxVariant valPath;
			wxVariant valImport;
			wxVariant valTarget;
			this->imageList->GetValue(valPath, i, ColumnPath);
			this->imageList->GetValue(valImport, i, ColumnImport);
			this->imageList->GetValue(valTarget, i, ColumnTarget);

			if (!valImport.GetBool())
				continue;

			ImportItem item;
			item.path = valPath.GetString();

			if (valTarget.IsType("string"))
			{
				item.type = ImportItem::Type::Face;
				item.face = Image::translateCubeFace(valTarget.GetString().ToStdString());
			}
			else
			{
				item.type = ImportItem::Type::Layer;
				item.layer = valTarget.GetInteger();
			}

			result.push_back(item);
		}

		return result;
	}

	unsigned int TexToolkitBatchImportDialog::getCount() const
	{
		return this->imageList->GetItemCount();
	}

	unsigned int TexToolkitBatchImportDialog::getCountSelected() const
	{
		unsigned int count = 0;

		for (unsigned int i = 0; i < this->imageList->GetItemCount(); i++)
		{
			wxVariant valImport;
			this->imageList->GetValue(valImport, i, ColumnImport);
			count += valImport.GetBool() == true;
		}

		return count;
	}

	void TexToolkitBatchImportDialog::setLayers(unsigned int n)
	{
		this->layers = n;
		this->updateMax();
	}

	void TexToolkitBatchImportDialog::setFaces(unsigned int n)
	{
		this->faces = n;
		this->updateMax();
	}

	void TexToolkitBatchImportDialog::setLevels(unsigned int n)
	{
		this->levels = n;
		this->updateMax();
	}

	void TexToolkitBatchImportDialog::loadList(const wxArrayString& paths)
	{
		using Path_t = std::string;
		using Target_t = std::string;
		using Pattern_t = std::string;
		using Face_t = std::string;

		using ImageEntry = std::tuple<Path_t, Target_t>;
		using PatternMap = std::multimap<Pattern_t, ImageEntry>;
		PatternMap patternMap;
		std::map<Path_t, PatternMap::iterator> pathMap;

		unsigned int largestSetCount = 0;
		Pattern_t largestSet = "";

		// Group into patterns
		for (const auto& path : paths)
		{
			const std::string stdpath = path;
			for (const auto& regexstr : this->regexes)
			{
				const std::regex regex(regexstr);
				std::smatch match;
				if (std::regex_search(stdpath, match, regex))
				{
					std::string name = match[1];
					std::string target = match[2];

					auto newIt = patternMap.insert({ name, ImageEntry{path, target} });
					pathMap[stdpath] = newIt;
					unsigned int count = patternMap.count(name);
					if (count > largestSetCount)
					{
						largestSetCount = count;
						largestSet = name;
					}
				}
			}
		}

		if (patternMap.empty())
			return;

		const std::unordered_map<Target_t, Face_t> faceMap{
			{ "positive_x", Image::translateCubeFace(Image::CubeFace::PositiveX) },
			{ "negative_x", Image::translateCubeFace(Image::CubeFace::NegativeX) },
			{ "positive_y", Image::translateCubeFace(Image::CubeFace::PositiveY) },
			{ "negative_y", Image::translateCubeFace(Image::CubeFace::NegativeY) },
			{ "positive_z", Image::translateCubeFace(Image::CubeFace::PositiveZ) },
			{ "negative_z", Image::translateCubeFace(Image::CubeFace::NegativeZ) },
			{ "w", Image::translateCubeFace(Image::CubeFace::NegativeX) },
			{ "e", Image::translateCubeFace(Image::CubeFace::PositiveX) },
			{ "n", Image::translateCubeFace(Image::CubeFace::PositiveZ) },
			{ "s", Image::translateCubeFace(Image::CubeFace::NegativeZ) },
			{ "u", Image::translateCubeFace(Image::CubeFace::PositiveY) },
			{ "d", Image::translateCubeFace(Image::CubeFace::NegativeY) },
		};

		// Fill the list
		this->imageList->DeleteAllItems();
		for (const auto& path : paths)
		{
			const std::string stdpath = path;
			auto pattern = pathMap.at(stdpath);

			bool importEnabled = pattern->first == largestSet;
			const auto& imageEntry = pattern->second;

			wxVariant target;

			const auto& targetStr = std::get<1>(imageEntry);
			auto faceIt = faceMap.find(targetStr);
			if (faceIt != faceMap.end())
				target = wxString(faceIt->second);
			else if (isNumber(targetStr))
				target = wxVariant(std::stoi(targetStr));

			wxVector<wxVariant> entry{
				path,
				importEnabled,
				target
			};
			this->imageList->AppendItem(entry);
		}

		// Fix selections
		using Index_t = size_t;
		std::vector<std::tuple<Index_t, Path_t>> selection;
		for (unsigned int i = 0; i < this->imageList->GetItemCount(); i++)
		{
			wxVariant valPath;
			wxVariant valImport;
			this->imageList->GetValue(valPath, i, ColumnPath);
			this->imageList->GetValue(valImport, i, ColumnImport);

			if (!valImport.GetBool())
				continue;

			selection.push_back({ i, valPath.GetString().ToStdString() });
		}
		
		std::sort(selection.begin(), selection.end(), [&pathMap](const auto& a, const auto& b) {
			auto targetStrA = std::get<1>(pathMap.at(std::get<1>(a))->second);
			auto targetStrB = std::get<1>(pathMap.at(std::get<1>(b))->second);
			if (isNumber(targetStrA) && isNumber(targetStrB))
			{
				return std::stoi(targetStrA) < std::stoi(targetStrB);
			}
			return targetStrA < targetStrB;
		});

		int selectedCount = 0;
		for (auto& tuple : selection)
		{
			auto idx = std::get<0>(tuple);

			wxVariant valTarget;
			this->imageList->GetValue(valTarget, idx, ColumnTarget);
			
			this->imageList->SetValue(selectedCount < this->maxItems, idx, ColumnImport);
			if (valTarget.IsType("long"))
				this->imageList->SetValue(selectedCount, idx, ColumnTarget);

			selectedCount++;
		}
	}

	void TexToolkitBatchImportDialog::updateMax()
	{
		switch (this->textureType)
		{
		case Image::TextureType::TextureCube:
		{
			this->maxItems = this->faces;
			break;
		}
		case Image::TextureType::Texture2DArray:
			this->maxItems = this->layers;
			break;
		}
	}

	void TexToolkitBatchImportDialog::importFolderEvent(wxCommandEvent& event)
	{
		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);

		wxDirDialog dlg(this, "Open image", picturesDir, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		TextureLoader loader;

		WildcardFileTraverser trav(loader.getWildcardString());
		wxDir dir(dlg.GetPath());
		dir.Traverse(trav);
		this->loadList(trav.getFoundFiles());
	}

	void TexToolkitBatchImportDialog::importFilesEvent(wxCommandEvent& event)
	{
		TextureLoader loader;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Open image", picturesDir, wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
		dlg.SetFilterIndex(loader.getFilterIndexAll());

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		wxArrayString paths;
		dlg.GetPaths(paths);
		this->loadList(paths);
	}

	void TexToolkitBatchImportDialog::importEvent(wxCommandEvent& event)
	{
		if (this->getCountSelected() > this->maxItems)
		{
			wxMessageBox(wxString::Format("Maximum of %d items may be selected", this->maxItems), "Error", wxOK | wxICON_ERROR);
			return;
		}

		this->EndModal(wxID_OK);
	}

	void TexToolkitBatchImportDialog::cancelEvent(wxCommandEvent& event)
	{
		this->EndModal(wxID_CANCEL);
	}
}
