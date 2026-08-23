#ifndef _h_textoolkit_gui_newddspanel
#define _h_textoolkit_gui_newddspanel

#include "mainwindow.h"
#include "util.hpp"
#include "common/image.hpp"

namespace textoolkit
{
	class TexToolkitnewDdsPanel : public newDdsPanel, public NewImagePanel
	{
	public:
		TexToolkitnewDdsPanel(wxWindow* parent);

		virtual std::string getTextureName() const override;
		virtual unsigned int getTextureWidth() const override;
		virtual unsigned int getTextureHeight() const override;
		virtual unsigned int getTextureDepth() const;
		bool getGenerateMipmaps() const;
		Image::TextureType getTextureType() const;
		Image::TextureInternalFormat getFormat() const;
		Image::CompressionType getCompression() const;
		unsigned int getLayersCount() const;

	private:
		void updateWidgetStates();

		void typeChangedEvent(wxCommandEvent& event);
		void generateMipmapsCheckedEvent(wxCommandEvent& event);
	};
}

#endif
