#ifndef _h_textoolkit_gui_newbmppanel
#define _h_textoolkit_gui_newbmppanel

#include "gui/mainwindow.h"
#include "gui/texture.hpp"
#include "gui/util.hpp"

namespace textoolkit
{
	class TexToolkitnewBmpPanel : public newBmpPanel, public NewImagePanel
	{
	public:
		TexToolkitnewBmpPanel(wxWindow* parent);

		std::unique_ptr<textoolkit::GuiTexture> createTexture();

		virtual std::string getTextureName() const override;
		virtual unsigned int getTextureWidth() const override;
		virtual unsigned int getTextureHeight() const override;
	};
}

#endif
