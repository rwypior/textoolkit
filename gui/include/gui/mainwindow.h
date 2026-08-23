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
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <wx/statbox.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/gauge.h>

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
			wxMenu* help;
			wxPanel* m_panel27;
			wxAuiNotebook* notebook;

		public:

			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TexToolkit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1187,710 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

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
			wxBitmapButton* refreshDisplayModeListButton;
			wxPanel* pageflat;
			wxStaticBitmap* flatView;
			wxPanel* m_panel45;
			wxStaticText* flatViewImageDetails;
			wxHyperlinkCtrl* selectBaseLink;
			wxPanel* pageEdit;
			wxPanel* m_panel43;
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
			wxPanel* m_panel44;
			wxStaticText* editorImageDetails;
			wxNotebook* m_notebook21;
			wxPanel* subImagesTab;
			wxSplitterWindow* m_splitter1;
			wxPanel* m_panel35;
			wxScrolledWindow* layerScroller;
			wxPanel* m_panel251;
			wxBitmapButton* addLayerButton;
			wxBitmapButton* removeLayerButton;
			wxPanel* m_panel36;
			wxSplitterWindow* m_splitter5;
			wxPanel* m_panel37;
			wxScrolledWindow* faceScroller;
			wxPanel* m_panel2611;
			wxBitmapButton* addFaceButton;
			wxBitmapButton* removeFaceButton;
			wxPanel* m_panel38;
			wxScrolledWindow* levelScroller;
			wxPanel* m_panel261;
			wxBitmapButton* addLevelButton;
			wxBitmapButton* removeLevelButton;
			wxPanel* propertiesTab;
			wxPropertyGrid* propertyGrid;

		public:

			TextureView( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1034,631 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~TextureView();

			void m_splitter1OnIdle( wxIdleEvent& )
			{
				m_splitter1->SetSashPosition( 200 );
				m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter1OnIdle ), NULL, this );
			}

			void m_splitter5OnIdle( wxIdleEvent& )
			{
				m_splitter5->SetSashPosition( 0 );
				m_splitter5->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter5OnIdle ), NULL, this );
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
			wxStaticText* m_staticText17;
			wxSpinCtrl* depthEdit;
			wxPanel* m_panel15;
			wxCheckBox* mipmapsCheckbox;
			wxStaticText* m_staticText14;
			wxChoice* typeChoice;
			wxPanel* m_panel40;
			wxStaticText* m_staticText151;
			wxChoice* formatChoice;
			wxPanel* m_panel41;
			wxStaticText* m_staticText16;
			wxChoice* compressionChoice;
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

			AboutWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TexToolkit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 328,192 ), long style = wxCAPTION|wxCLOSE_BOX|wxTAB_TRAVERSAL );

			~AboutWindow();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class SubimageEntry
	///////////////////////////////////////////////////////////////////////////////
	class SubimageEntry : public wxPanel
	{
		private:

		protected:
			wxPanel* m_panel29;
			wxStaticBitmap* previewImage;
			wxPanel* m_panel32;
			wxStaticText* nameLabel;
			wxStaticText* measurementsLabel;
			wxStaticLine* m_staticline1;

		public:

			SubimageEntry( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 337,71 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

			~SubimageEntry();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class ProgressDialog
	///////////////////////////////////////////////////////////////////////////////
	class ProgressDialog : public wxDialog
	{
		private:

		protected:
			wxGauge* progressBar;
			wxPanel* m_panel30;
			wxButton* cancelButton;

		public:

			ProgressDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Loading..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 377,111 ), long style = wxDEFAULT_DIALOG_STYLE );

			~ProgressDialog();

	};

} // namespace textoolkit

