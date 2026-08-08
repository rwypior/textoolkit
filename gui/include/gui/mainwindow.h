///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include "gui/canvas.hpp"
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

///////////////////////////////////////////////////////////////////////////

namespace textoolkit
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class MainWindow
	///////////////////////////////////////////////////////////////////////////////
	class MainWindow : public wxFrame
	{
		private:

		protected:
			enum
			{
				ID_NEW = 6000,
				ID_OPEN,
				ID_RECENT,
				ID_SAVE,
				ID_SAVE_AS,
				ID_EXIT,
				ID_ABOUT,
			};

			wxMenuBar* mainmenu;
			wxMenu* file;
			wxMenu* edit;
			wxMenu* help;
			wxNotebook* notebook;

		public:

			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 734,587 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~MainWindow();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class TextureView
	///////////////////////////////////////////////////////////////////////////////
	class TextureView : public wxPanel
	{
		private:

		protected:
			wxPanel* renderPanel;
			Canvas* canvas;
			wxPanel* m_panel4;
			wxComboBox* displaymode;
			wxComboBox* m_comboBox2;
			wxPanel* m_panel2;
			wxPropertyGrid* m_propertyGrid1;

		public:

			TextureView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 826,631 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~TextureView();

	};

} // namespace textoolkit

