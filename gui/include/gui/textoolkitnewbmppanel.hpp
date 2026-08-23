#ifndef _h_textoolkit_gui_newbmppanel
#define _h_textoolkit_gui_newbmppanel

#include "mainwindow.h"
#include "util.hpp"

namespace textoolkit
{
	class TexToolkitnewBmpPanel : public newBmpPanel, public NewImagePanel
	{
	public:
		TexToolkitnewBmpPanel(wxWindow* parent);

		virtual std::string getTextureName() const override;
		virtual unsigned int getTextureWidth() const override;
		virtual unsigned int getTextureHeight() const override;
	};
}

#endif
