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

	notebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_DEFAULT_STYLE );

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

	m_notebook2 = new wxNotebook( renderPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM );
	page3d = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );

	canvas = new Canvas( page3d, wxID_ANY );
	bSizer61->Add( canvas, 1, wxALL|wxEXPAND, 0 );

	m_panel24 = new wxPanel( page3d, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );

	displaymode = new wxComboBox( m_panel24, wxID_ANY, _("Cube"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	displaymode->Append( _("Cube") );
	displaymode->Append( _("Rectangle") );
	displaymode->Append( _("Pyramid") );
	displaymode->Append( _("Sphere") );
	displaymode->SetMinSize( wxSize( 150,-1 ) );

	bSizer30->Add( displaymode, 0, wxBOTTOM|wxTOP, 5 );


	m_panel24->SetSizer( bSizer30 );
	m_panel24->Layout();
	bSizer30->Fit( m_panel24 );
	bSizer61->Add( m_panel24, 0, wxEXPAND | wxALL, 0 );


	page3d->SetSizer( bSizer61 );
	page3d->Layout();
	bSizer61->Fit( page3d );
	m_notebook2->AddPage( page3d, _("3D view"), false );
	pageflat = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );

	flatView = new wxStaticBitmap( pageflat, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( flatView, 1, wxALL|wxEXPAND, 5 );


	pageflat->SetSizer( bSizer29 );
	pageflat->Layout();
	bSizer29->Fit( pageflat );
	m_notebook2->AddPage( pageflat, _("Flat view"), false );
	pageEdit = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_panel21 = new wxPanel( pageEdit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 0, 2, 0, 0 );

	m_bpButton1 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_bpButton1->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_QUESTION), wxASCII_STR(wxART_TOOLBAR) ) );
	gSizer4->Add( m_bpButton1, 0, wxALL, 0 );

	m_bpButton2 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton2, 0, wxALL, 0 );

	m_bpButton7 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton7, 0, wxALL, 0 );

	m_bpButton8 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton8, 0, wxALL, 0 );

	m_bpButton9 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton9, 0, wxALL, 0 );

	m_bpButton10 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton10, 0, wxALL, 0 );

	m_bpButton11 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton11, 0, wxALL, 0 );

	m_bpButton12 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton12, 0, wxALL, 0 );

	m_bpButton13 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton13, 0, wxALL, 0 );

	m_bpButton14 = new wxBitmapButton( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	gSizer4->Add( m_bpButton14, 0, wxALL, 0 );


	gSizer4->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panel21->SetSizer( gSizer4 );
	m_panel21->Layout();
	gSizer4->Fit( m_panel21 );
	fgSizer3->Add( m_panel21, 0, wxALL, 0 );

	m_bitmap2 = new wxStaticBitmap( pageEdit, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_bitmap2, 1, wxALL|wxEXPAND, 5 );

	m_panel23 = new wxPanel( pageEdit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer3->Add( m_panel23, 0, wxALL, 5 );

	m_panel22 = new wxPanel( pageEdit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer3->Add( m_panel22, 0, wxALL, 5 );


	pageEdit->SetSizer( fgSizer3 );
	pageEdit->Layout();
	fgSizer3->Fit( pageEdit );
	m_notebook2->AddPage( pageEdit, _("Editor"), false );

	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );


	renderPanel->SetSizer( bSizer4 );
	renderPanel->Layout();
	bSizer4->Fit( renderPanel );
	bSizer3->Add( renderPanel, 1, wxEXPAND | wxALL, 0 );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter1OnIdle ), NULL, this );

	m_panel25 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel25, wxID_ANY, _("Layers / faces") ), wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	layerFaceScroller = new wxScrolledWindow( sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	layerFaceScroller->SetScrollRate( 5, 5 );
	bSizer5->Add( layerFaceScroller, 1, wxEXPAND | wxALL, 0 );

	m_panel251 = new wxPanel( sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );


	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );

	addLayerFaceButton = new wxBitmapButton( m_panel251, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	addLayerFaceButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_BUTTON) ) );
	bSizer27->Add( addLayerFaceButton, 0, wxTOP, 5 );

	removeLayerFaceButton = new wxBitmapButton( m_panel251, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	removeLayerFaceButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_BUTTON) ) );
	bSizer27->Add( removeLayerFaceButton, 0, wxLEFT|wxTOP, 5 );


	m_panel251->SetSizer( bSizer27 );
	m_panel251->Layout();
	bSizer27->Fit( m_panel251 );
	bSizer5->Add( m_panel251, 0, wxEXPAND | wxALL, 0 );


	sbSizer1->Add( bSizer5, 1, wxEXPAND, 5 );


	bSizer32->Add( sbSizer1, 1, wxEXPAND, 5 );

	m_splitter3 = new wxSplitterWindow( m_panel25, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter3OnIdle ), NULL, this );

	m_panel31 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel31, wxID_ANY, _("Levels") ), wxVERTICAL );

	levelScroller = new wxScrolledWindow( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	levelScroller->SetScrollRate( 5, 5 );
	sbSizer2->Add( levelScroller, 1, wxEXPAND | wxALL, 5 );

	m_panel261 = new wxPanel( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );


	bSizer28->Add( 0, 0, 1, wxEXPAND, 5 );

	addLevelButton = new wxBitmapButton( m_panel261, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	addLevelButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_BUTTON) ) );
	bSizer28->Add( addLevelButton, 0, wxTOP, 5 );

	removeLevelButton = new wxBitmapButton( m_panel261, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	removeLevelButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_BUTTON) ) );
	bSizer28->Add( removeLevelButton, 0, wxLEFT|wxTOP, 5 );


	m_panel261->SetSizer( bSizer28 );
	m_panel261->Layout();
	bSizer28->Fit( m_panel261 );
	sbSizer2->Add( m_panel261, 0, wxEXPAND | wxALL, 0 );


	m_panel31->SetSizer( sbSizer2 );
	m_panel31->Layout();
	sbSizer2->Fit( m_panel31 );
	m_splitter3->Initialize( m_panel31 );
	bSizer32->Add( m_splitter3, 1, wxEXPAND, 5 );


	m_panel25->SetSizer( bSizer32 );
	m_panel25->Layout();
	bSizer32->Fit( m_panel25 );
	m_panel26 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel26, wxID_ANY, _("Properties") ), wxVERTICAL );

	propertyGrid = new wxPropertyGrid(sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	propertyGrid->SetMinSize( wxSize( 250,-1 ) );

	sbSizer4->Add( propertyGrid, 1, wxALL|wxEXPAND, 0 );


	m_panel26->SetSizer( sbSizer4 );
	m_panel26->Layout();
	sbSizer4->Fit( m_panel26 );
	m_splitter1->SplitHorizontally( m_panel25, m_panel26, 0 );
	bSizer3->Add( m_splitter1, 0, wxEXPAND, 0 );


	this->SetSizer( bSizer3 );
	this->Layout();
}

TextureView::~TextureView()
{
}

NewDialog::NewDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	typeList = new wxListBox( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	typeList->SetMinSize( wxSize( 150,-1 ) );

	bSizer9->Add( typeList, 0, wxALL|wxEXPAND, 0 );

	propertiesPanel = new wxPanel( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );


	propertiesPanel->SetSizer( bSizer25 );
	propertiesPanel->Layout();
	bSizer25->Fit( propertiesPanel );
	bSizer9->Add( propertiesPanel, 1, wxEXPAND | wxALL, 0 );


	m_panel6->SetSizer( bSizer9 );
	m_panel6->Layout();
	bSizer9->Fit( m_panel6 );
	bSizer7->Add( m_panel6, 1, wxEXPAND | wxALL, 5 );

	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );


	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );

	cancelButton = new wxButton( m_panel7, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( cancelButton, 0, wxALL, 5 );

	okButton = new wxButton( m_panel7, wxID_ANY, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( okButton, 0, wxALL, 5 );


	m_panel7->SetSizer( bSizer8 );
	m_panel7->Layout();
	bSizer8->Fit( m_panel7 );
	bSizer7->Add( m_panel7, 0, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer7 );
	this->Layout();

	this->Centre( wxBOTH );
}

NewDialog::~NewDialog()
{
}

newBmpPanel::newBmpPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_panel9 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( m_panel9, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer13->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 5 );

	nameEdit = new wxTextCtrl( m_panel9, wxID_ANY, _("New texture"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( nameEdit, 1, wxALL, 5 );


	m_panel9->SetSizer( bSizer13 );
	m_panel9->Layout();
	bSizer13->Fit( m_panel9 );
	bSizer12->Add( m_panel9, 0, wxEXPAND | wxALL, 0 );

	m_panel10 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText6 = new wxStaticText( m_panel10, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer14->Add( m_staticText6, 0, wxALIGN_CENTER|wxALL, 5 );

	widthEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000000, 1024 );
	widthEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( widthEdit, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panel10, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer14->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 5 );

	heightEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000000, 1024 );
	heightEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( heightEdit, 0, wxALL, 5 );


	m_panel10->SetSizer( bSizer14 );
	m_panel10->Layout();
	bSizer14->Fit( m_panel10 );
	bSizer12->Add( m_panel10, 0, wxEXPAND | wxALL, 0 );


	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer12 );
	this->Layout();
}

newBmpPanel::~newBmpPanel()
{
}

newDdsPanel::newDdsPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_panel9 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( m_panel9, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer13->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 5 );

	nameEdit = new wxTextCtrl( m_panel9, wxID_ANY, _("New texture"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( nameEdit, 1, wxALL, 5 );


	m_panel9->SetSizer( bSizer13 );
	m_panel9->Layout();
	bSizer13->Fit( m_panel9 );
	bSizer12->Add( m_panel9, 0, wxEXPAND | wxALL, 0 );

	m_panel10 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText6 = new wxStaticText( m_panel10, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer14->Add( m_staticText6, 0, wxALIGN_CENTER|wxALL, 5 );

	widthEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000000, 1024 );
	widthEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( widthEdit, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panel10, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer14->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 5 );

	heightEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000000, 1024 );
	heightEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( heightEdit, 0, wxALL, 5 );


	m_panel10->SetSizer( bSizer14 );
	m_panel10->Layout();
	bSizer14->Fit( m_panel10 );
	bSizer12->Add( m_panel10, 0, wxEXPAND | wxALL, 0 );

	m_panel15 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	mipmapsCheckbox = new wxCheckBox( m_panel15, wxID_ANY, _("Generate mipmaps"), wxDefaultPosition, wxDefaultSize, 0 );
	mipmapsCheckbox->SetValue(true);
	bSizer22->Add( mipmapsCheckbox, 0, wxALIGN_CENTER|wxALL, 5 );

	mipmapsCount = new wxSpinCtrl( m_panel15, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	mipmapsCount->SetMinSize( wxSize( 50,-1 ) );

	bSizer22->Add( mipmapsCount, 0, wxALL, 5 );


	bSizer22->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( m_panel15, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer22->Add( m_staticText14, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString typeChoiceChoices[] = { _("2D Texture"), _("3D Texture"), _("Cube map"), _("Array texture") };
	int typeChoiceNChoices = sizeof( typeChoiceChoices ) / sizeof( wxString );
	typeChoice = new wxChoice( m_panel15, wxID_ANY, wxDefaultPosition, wxDefaultSize, typeChoiceNChoices, typeChoiceChoices, 0 );
	typeChoice->SetSelection( 0 );
	typeChoice->SetMinSize( wxSize( 150,-1 ) );

	bSizer22->Add( typeChoice, 0, wxALL, 5 );


	m_panel15->SetSizer( bSizer22 );
	m_panel15->Layout();
	bSizer22->Fit( m_panel15 );
	bSizer12->Add( m_panel15, 0, wxEXPAND | wxALL, 0 );

	m_panel16 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );

	additionalPanel = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText15 = new wxStaticText( additionalPanel, wxID_ANY, _("Layers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer24->Add( m_staticText15, 0, wxALIGN_CENTER|wxALL, 5 );

	layersCount = new wxSpinCtrl( additionalPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000, 0 );
	layersCount->SetMinSize( wxSize( 100,-1 ) );

	bSizer24->Add( layersCount, 0, wxALL, 5 );


	additionalPanel->SetSizer( bSizer24 );
	additionalPanel->Layout();
	bSizer24->Fit( additionalPanel );
	bSizer23->Add( additionalPanel, 0, wxEXPAND | wxALL, 0 );


	m_panel16->SetSizer( bSizer23 );
	m_panel16->Layout();
	bSizer23->Fit( m_panel16 );
	bSizer12->Add( m_panel16, 0, wxEXPAND | wxALL, 0 );


	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer12 );
	this->Layout();
}

newDdsPanel::~newDdsPanel()
{
}

AboutWindow::AboutWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	m_panel18 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );

	nameLabel = new wxStaticText( m_panel18, wxID_ANY, _("TexToolkit"), wxDefaultPosition, wxDefaultSize, 0 );
	nameLabel->Wrap( -1 );
	nameLabel->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer28->Add( nameLabel, 0, wxALL, 0 );


	bSizer28->Add( 0, 8, 0, 0, 5 );

	infoLabel = new wxStaticText( m_panel18, wxID_ANY, _("Version %ver.major%.%ver.minor%.%ver.patch%\nReleased %reldate%"), wxDefaultPosition, wxDefaultSize, 0 );
	infoLabel->Wrap( -1 );
	bSizer28->Add( infoLabel, 0, wxALL, 0 );

	githubLink = new wxHyperlinkCtrl( m_panel18, wxID_ANY, _("%githublink%"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer28->Add( githubLink, 0, wxALL, 5 );


	m_panel18->SetSizer( bSizer28 );
	m_panel18->Layout();
	bSizer28->Fit( m_panel18 );
	bSizer26->Add( m_panel18, 1, wxEXPAND | wxALL, 16 );

	m_panel19 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );


	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );

	okButton = new wxButton( m_panel19, wxID_ANY, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( okButton, 0, wxALL, 5 );


	m_panel19->SetSizer( bSizer27 );
	m_panel19->Layout();
	bSizer27->Fit( m_panel19 );
	bSizer26->Add( m_panel19, 0, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer26 );
	this->Layout();

	this->Centre( wxBOTH );
}

AboutWindow::~AboutWindow()
{
}

SubimageEntry::SubimageEntry( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxHORIZONTAL );

	previewImage = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer40->Add( previewImage, 0, wxALL|wxEXPAND|wxSHAPED, 5 );

	m_panel32 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );


	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );

	nameLabel = new wxStaticText( m_panel32, wxID_ANY, _("Layer / face ID"), wxDefaultPosition, wxDefaultSize, 0 );
	nameLabel->Wrap( -1 );
	bSizer41->Add( nameLabel, 0, wxALL, 5 );


	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panel32->SetSizer( bSizer41 );
	m_panel32->Layout();
	bSizer41->Fit( m_panel32 );
	bSizer40->Add( m_panel32, 1, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer40 );
	this->Layout();
}

SubimageEntry::~SubimageEntry()
{
}
