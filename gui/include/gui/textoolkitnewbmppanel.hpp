#ifndef _h_textoolkit_gui_newbmppanel
#define _h_textoolkit_gui_newbmppanel

#include "mainwindow.h"

namespace textoolkit
{
	class TexToolkitnewBmpPanel : public newBmpPanel
	{
	public:
		TexToolkitnewBmpPanel(wxWindow* parent);

		std::string getTextureName() const;
		unsigned int getTextureWidth() const;
		unsigned int getTextureHeight() const;
	};
}

#endif
