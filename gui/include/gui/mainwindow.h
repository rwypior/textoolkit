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
#include <wx/aui/auibook.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include "gui/canvas.hpp"
#include <wx/combobox.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <wx/statbox.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/hyperlink.h>

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
			wxPanel* m_panel27;
			wxAuiNotebook* notebook;

		public:

			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TexToolkit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 734,587 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

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
			wxNotebook* m_notebook2;
			wxPanel* page3d;
			Canvas* canvas;
			wxPanel* m_panel24;
			wxComboBox* displaymode;
			wxPanel* pageflat;
			wxStaticBitmap* flatView;
			wxPanel* pageEdit;
			wxPanel* m_panel21;
			wxBitmapButton* m_bpButton1;
			wxBitmapButton* m_bpButton2;
			wxBitmapButton* m_bpButton7;
			wxBitmapButton* m_bpButton8;
			wxBitmapButton* m_bpButton9;
			wxBitmapButton* m_bpButton10;
			wxBitmapButton* m_bpButton11;
			wxBitmapButton* m_bpButton12;
			wxBitmapButton* m_bpButton13;
			wxBitmapButton* m_bpButton14;
			wxStaticBitmap* m_bitmap2;
			wxPanel* m_panel23;
			wxPanel* m_panel22;
			wxSplitterWindow* m_splitter1;
			wxPanel* m_panel25;
			wxScrolledWindow* layerFaceScroller;
			wxPanel* m_panel251;
			wxBitmapButton* addLayerFaceButton;
			wxBitmapButton* removeLayerFaceButton;
			wxSplitterWindow* m_splitter3;
			wxPanel* m_panel31;
			wxScrolledWindow* levelScroller;
			wxPanel* m_panel261;
			wxBitmapButton* addLevelButton;
			wxBitmapButton* removeLevelButton;
			wxPanel* m_panel26;
			wxPropertyGrid* propertyGrid;

		public:

			TextureView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 826,631 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~TextureView();

			void m_splitter1OnIdle( wxIdleEvent& )
			{
				m_splitter1->SetSashPosition( 0 );
				m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter1OnIdle ), NULL, this );
			}

			void m_splitter3OnIdle( wxIdleEvent& )
			{
				m_splitter3->SetSashPosition( 0 );
				m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter3OnIdle ), NULL, this );
			}

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class NewDialog
	///////////////////////////////////////////////////////////////////////////////
	class NewDialog : public wxDialog
	{
		private:

		protected:
			wxPanel* m_panel6;
			wxListBox* typeList;
			wxPanel* propertiesPanel;
			wxPanel* m_panel7;
			wxButton* cancelButton;
			wxButton* okButton;

		public:

			NewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 667,391 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

			~NewDialog();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class newBmpPanel
	///////////////////////////////////////////////////////////////////////////////
	class newBmpPanel : public wxPanel
	{
		private:

		protected:
			wxPanel* m_panel9;
			wxStaticText* m_staticText5;
			wxTextCtrl* nameEdit;
			wxPanel* m_panel10;
			wxStaticText* m_staticText6;
			wxSpinCtrl* widthEdit;
			wxStaticText* m_staticText7;
			wxSpinCtrl* heightEdit;

		public:

			newBmpPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~newBmpPanel();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class newDdsPanel
	///////////////////////////////////////////////////////////////////////////////
	class newDdsPanel : public wxPanel
	{
		private:

		protected:
			wxPanel* m_panel9;
			wxStaticText* m_staticText5;
			wxTextCtrl* nameEdit;
			wxPanel* m_panel10;
			wxStaticText* m_staticText6;
			wxSpinCtrl* widthEdit;
			wxStaticText* m_staticText7;
			wxSpinCtrl* heightEdit;
			wxPanel* m_panel15;
			wxCheckBox* mipmapsCheckbox;
			wxSpinCtrl* mipmapsCount;
			wxStaticText* m_staticText14;
			wxChoice* typeChoice;
			wxPanel* m_panel16;
			wxPanel* additionalPanel;
			wxStaticText* m_staticText15;
			wxSpinCtrl* layersCount;

		public:

			newDdsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~newDdsPanel();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class AboutWindow
	///////////////////////////////////////////////////////////////////////////////
	class AboutWindow : public wxFrame
	{
		private:

		protected:
			wxPanel* m_panel28;
			wxPanel* m_panel18;
			wxStaticText* nameLabel;
			wxStaticText* infoLabel;
			wxHyperlinkCtrl* githubLink;
			wxPanel* m_panel19;
			wxButton* okButton;

		public:

			AboutWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 328,223 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~AboutWindow();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class SubimageEntry
	///////////////////////////////////////////////////////////////////////////////
	class SubimageEntry : public wxPanel
	{
		private:

		protected:
			wxStaticBitmap* previewImage;
			wxPanel* m_panel32;
			wxStaticText* nameLabel;

		public:

			SubimageEntry( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 337,79 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~SubimageEntry();

	};

} // namespace textoolkit

