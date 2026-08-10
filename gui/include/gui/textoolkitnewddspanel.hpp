#ifndef _h_textoolkit_gui_newddspanel
#define _h_textoolkit_gui_newddspanel

#include "mainwindow.h"

namespace textoolkit
{
	class TexToolkitnewDdsPanel : public newDdsPanel
	{
	public:
		TexToolkitnewDdsPanel(wxWindow* parent);

		std::string getTextureName() const;
		unsigned int getTextureWidth() const;
		unsigned int getTextureHeight() const;
		bool getGenerateMipmaps() const;
		unsigned int getMipmapCount() const;
		std::string getTextureType() const;
		unsigned int getLayersCount() const;

	private:
		void updateWidgetStates();

		void typeChangedEvent(wxCommandEvent& event);
		void generateMipmapsCheckedEvent(wxCommandEvent& event);
	};
}

#endif
