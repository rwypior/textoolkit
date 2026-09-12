#ifndef _h_textoolkit_gui_newddspanel
#define _h_textoolkit_gui_newddspanel

#include "gui/mainwindow.h"
#include "gui/textoolkitbatchimportdialog.hpp"
#include "gui/texture.hpp"
#include "gui/util.hpp"
#include "common/image.hpp"

#include <memory>

namespace textoolkit
{
	class DDS;

	class TexToolkitnewDdsPanel : public newDdsPanel, public NewImagePanel
	{
	public:
		TexToolkitnewDdsPanel(wxWindow* parent);

		std::unique_ptr<textoolkit::GuiTexture> createTexture();

		virtual std::string getTextureName() const override;
		virtual unsigned int getTextureWidth() const override;
		virtual unsigned int getTextureHeight() const override;
		virtual unsigned int getTextureDepth() const;
		bool getGenerateMipmaps() const;
		Image::TextureType getTextureType() const;
		Image::TextureInternalFormat getFormat() const;
		Image::CompressionType getCompression() const;
		unsigned int getLayersCount() const;
		TexToolkitBatchImportDialog::ImportItems getImportImages() const;

	private:
		void updateWidgetStates();
		void updateImportedLabel();
		const TexToolkitBatchImportDialog* getBatchImportDlg() const;
		TexToolkitBatchImportDialog* getBatchImportDlg();

		void importSubimages(std::shared_ptr<DDS>& dds);

		void typeChangedEvent(wxCommandEvent& event);
		void generateMipmapsCheckedEvent(wxCommandEvent& event);
		void importClickedEvent(wxCommandEvent& event);

		TexToolkitBatchImportDialog batchImportCubemap;
		TexToolkitBatchImportDialog batchImport2DArray;
	};
}

#endif
