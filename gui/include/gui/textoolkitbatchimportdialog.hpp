#ifndef _h_textoolkit_gui_batchimportdialog
#define _h_textoolkit_gui_batchimportdialog

#include "gui/mainwindow.h"
#include "common/image.hpp"
#include "texture/texture.hpp"

#include <vector>
#include <string>
#include <functional>
#include <limits>

namespace textoolkit
{
	class TexToolkitBatchImportDialog : public BatchImportDialog
	{
	public:
		struct ImportItem
		{
			enum class Type
			{
				Layer,
				Face,
				Level
			};

			std::string path;
			Type type;
			union
			{
				unsigned int layer;
				Image::CubeFace face;
			};
		};
		using ImportItems = std::vector<ImportItem>;

	public:
		TexToolkitBatchImportDialog(Image::TextureType textureType, wxWindow* parent = nullptr);

		Interpolation getMinInterpolation() const;
		Interpolation getMagInterpolation() const;
		ImportItems getItems() const;
		unsigned int getCount() const;
		unsigned int getCountSelected() const;

	private:
		void loadList(const wxArrayString& paths);

		void importFolderEvent(wxCommandEvent& event);
		void importFilesEvent(wxCommandEvent& event);
		void importEvent(wxCommandEvent& event);
		void cancelEvent(wxCommandEvent& event);

		Image::TextureType textureType;
		std::vector<std::string> regexes;
		unsigned int maxItems = std::numeric_limits<unsigned int>::max();
	};
}

#endif
