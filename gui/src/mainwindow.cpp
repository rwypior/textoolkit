///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui/mainwindow.h"

///////////////////////////////////////////////////////////////////////////
using namespace textoolkit;

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mainmenu = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* newfile;
	newfile = new wxMenuItem( file, ID_NEW, wxString( _("New") ) + wxT('\t') + wxT("Alt + N"), wxEmptyString, wxITEM_NORMAL );
	file->Append( newfile );

	wxMenuItem* open;
	open = new wxMenuItem( file, ID_OPEN, wxString( _("Open") ) + wxT('\t') + wxT("Ctrl + O"), wxEmptyString, wxITEM_NORMAL );
	file->Append( open );

	wxMenuItem* recent;
	recent = new wxMenuItem( file, ID_RECENT, wxString( _("Recent...") ) + wxT('\t') + wxT("Alt + R"), wxEmptyString, wxITEM_NORMAL );
	file->Append( recent );

	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( _("Save") ) + wxT('\t') + wxT("Ctrl + S"), wxEmptyString, wxITEM_NORMAL );
	file->Append( save );

	wxMenuItem* saveAs;
	saveAs = new wxMenuItem( file, ID_SAVE_AS, wxString( _("Save as...") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( saveAs );

	file->AppendSeparator();

	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( exit );

	mainmenu->Append( file, _("File") );

	edit = new wxMenu();
	mainmenu->Append( edit, _("Edit") );

	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( _("About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	help->Append( about );

	mainmenu->Append( help, _("Help") );

	this->SetMenuBar( mainmenu );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	bSizer2->Add( notebook, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

MainWindow::~MainWindow()
{
}

TextureView::TextureView( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	renderPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	canvas = new Canvas(renderPanel, wxID_ANY );
	bSizer4->Add( canvas, 1, wxALL|wxEXPAND, 5 );

	m_panel4 = new wxPanel( renderPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	displaymode = new wxComboBox( m_panel4, wxID_ANY, _("Cube"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer6->Add( displaymode, 0, wxALL, 0 );

	m_comboBox2 = new wxComboBox( m_panel4, wxID_ANY, _("All"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer6->Add( m_comboBox2, 0, wxALL, 0 );


	m_panel4->SetSizer( bSizer6 );
	m_panel4->Layout();
	bSizer6->Fit( m_panel4 );
	bSizer4->Add( m_panel4, 0, wxEXPAND | wxALL, 0 );


	renderPanel->SetSizer( bSizer4 );
	renderPanel->Layout();
	bSizer4->Fit( renderPanel);
	bSizer3->Add( renderPanel, 1, wxEXPAND | wxALL, 0 );

	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetMinSize( wxSize( 250,-1 ) );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid1 = new wxPropertyGrid(m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	m_propertyGrid1->SetMinSize( wxSize( 250,-1 ) );

	bSizer5->Add( m_propertyGrid1, 1, wxALL|wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	bSizer3->Add( m_panel2, 0, wxALL|wxEXPAND, 0 );


	this->SetSizer( bSizer3 );
	this->Layout();
}

TextureView::~TextureView()
{
}
