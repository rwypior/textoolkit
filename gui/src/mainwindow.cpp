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

	tools = new wxMenu();
	wxMenuItem* createCubemap;
	createCubemap = new wxMenuItem( tools, ID_CREATE_CUBEMAP, wxString( _("Create cubemap") ) , wxEmptyString, wxITEM_NORMAL );
	tools->Append( createCubemap );

	mainmenu->Append( tools, _("Tools") );

	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( _("About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	help->Append( about );

	mainmenu->Append( help, _("Help") );

	this->SetMenuBar( mainmenu );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_panel27 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );

	notebook = new wxAuiNotebook( m_panel27, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_DEFAULT_STYLE );

	bSizer29->Add( notebook, 1, wxEXPAND | wxALL, 5 );


	m_panel27->SetSizer( bSizer29 );
	m_panel27->Layout();
	bSizer29->Fit( m_panel27 );
	bSizer2->Add( m_panel27, 1, wxEXPAND | wxALL, 0 );


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

	displaymode = new wxComboBox( m_panel24, wxID_ANY, _("Cube"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	displaymode->SetToolTip( _("Display mode") );
	displaymode->SetMinSize( wxSize( 150,-1 ) );

	bSizer30->Add( displaymode, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxTOP, 5 );

	refreshDisplayModeListButton = new wxBitmapButton( m_panel24, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	refreshDisplayModeListButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_REDO), wxASCII_STR(wxART_BUTTON) ) );
	refreshDisplayModeListButton->SetToolTip( _("Refresh list") );

	bSizer30->Add( refreshDisplayModeListButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


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


	bSizer29->Add( 0, 0, 1, wxEXPAND, 5 );

	flatView = new wxStaticBitmap( pageflat, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( flatView, 0, wxALIGN_CENTER|wxALL, 5 );


	bSizer29->Add( 0, 0, 1, wxEXPAND, 5 );

	m_panel45 = new wxPanel( pageflat, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxHORIZONTAL );

	flatViewImageDetails = new wxStaticText( m_panel45, wxID_ANY, _("Layer %layer%; Face %face%; Level %level%; %width%x%height%"), wxDefaultPosition, wxDefaultSize, 0 );
	flatViewImageDetails->Wrap( -1 );
	flatViewImageDetails->SetFont( wxFont( 7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer47->Add( flatViewImageDetails, 0, wxALL, 3 );


	bSizer47->Add( 0, 0, 1, wxEXPAND, 5 );

	selectBaseLink = new wxHyperlinkCtrl( m_panel45, wxID_ANY, _("Select base"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	selectBaseLink->SetToolTip( _("Selected layer 0, face 0 and level 0 ") );

	bSizer47->Add( selectBaseLink, 0, wxALL, 3 );


	m_panel45->SetSizer( bSizer47 );
	m_panel45->Layout();
	bSizer47->Fit( m_panel45 );
	bSizer29->Add( m_panel45, 0, wxEXPAND | wxALL, 0 );


	pageflat->SetSizer( bSizer29 );
	pageflat->Layout();
	bSizer29->Fit( pageflat );
	m_notebook2->AddPage( pageflat, _("Flat view"), false );
	pageEdit = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxVERTICAL );

	m_panel43 = new wxPanel( pageEdit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_panel21 = new wxPanel( m_panel43, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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

	m_bitmap2 = new wxStaticBitmap( m_panel43, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_bitmap2, 1, wxALL|wxEXPAND, 5 );

	m_panel23 = new wxPanel( m_panel43, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer3->Add( m_panel23, 0, wxALL, 5 );

	m_panel22 = new wxPanel( m_panel43, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer3->Add( m_panel22, 0, wxALL, 5 );


	m_panel43->SetSizer( fgSizer3 );
	m_panel43->Layout();
	fgSizer3->Fit( m_panel43 );
	bSizer45->Add( m_panel43, 1, wxEXPAND | wxALL, 0 );

	m_panel44 = new wxPanel( pageEdit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxVERTICAL );

	editorImageDetails = new wxStaticText( m_panel44, wxID_ANY, _("Layer %layer%; Face %face%; Level %level%; %width%x%height%"), wxDefaultPosition, wxDefaultSize, 0 );
	editorImageDetails->Wrap( -1 );
	editorImageDetails->SetFont( wxFont( 7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer46->Add( editorImageDetails, 0, wxALL, 3 );


	m_panel44->SetSizer( bSizer46 );
	m_panel44->Layout();
	bSizer46->Fit( m_panel44 );
	bSizer45->Add( m_panel44, 0, wxEXPAND | wxALL, 0 );


	pageEdit->SetSizer( bSizer45 );
	pageEdit->Layout();
	bSizer45->Fit( pageEdit );
	m_notebook2->AddPage( pageEdit, _("Editor"), false );

	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );


	renderPanel->SetSizer( bSizer4 );
	renderPanel->Layout();
	bSizer4->Fit( renderPanel );
	bSizer3->Add( renderPanel, 1, wxEXPAND | wxALL, 0 );

	m_notebook21 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM );
	m_notebook21->SetMaxSize( wxSize( 250,-1 ) );

	subImagesTab = new wxPanel( m_notebook21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );

	m_splitter1 = new wxSplitterWindow( subImagesTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter1OnIdle ), NULL, this );

	m_panel35 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel35, wxID_ANY, _("Layers") ), wxVERTICAL );

	layerScroller = new wxScrolledWindow( sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	layerScroller->SetScrollRate( 0, 5 );
	wxBoxSizer* bSizer321;
	bSizer321 = new wxBoxSizer( wxVERTICAL );


	layerScroller->SetSizer( bSizer321 );
	layerScroller->Layout();
	bSizer321->Fit( layerScroller );
	sbSizer4->Add( layerScroller, 1, wxEXPAND | wxALL, 0 );

	m_panel251 = new wxPanel( sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );


	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );

	addLayerButton = new wxBitmapButton( m_panel251, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	addLayerButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_BUTTON) ) );
	addLayerButton->SetToolTip( _("Add new layer") );

	bSizer27->Add( addLayerButton, 0, wxTOP, 5 );

	removeLayerButton = new wxBitmapButton( m_panel251, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	removeLayerButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_BUTTON) ) );
	removeLayerButton->SetToolTip( _("Remove selected layer") );

	bSizer27->Add( removeLayerButton, 0, wxLEFT|wxTOP, 5 );


	m_panel251->SetSizer( bSizer27 );
	m_panel251->Layout();
	bSizer27->Fit( m_panel251 );
	sbSizer4->Add( m_panel251, 0, wxEXPAND | wxALL, 0 );


	m_panel35->SetSizer( sbSizer4 );
	m_panel35->Layout();
	sbSizer4->Fit( m_panel35 );
	m_panel36 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );

	m_splitter5 = new wxSplitterWindow( m_panel36, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter5->Connect( wxEVT_IDLE, wxIdleEventHandler( TextureView::m_splitter5OnIdle ), NULL, this );

	m_panel37 = new wxPanel( m_splitter5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel37, wxID_ANY, _("Faces") ), wxVERTICAL );

	faceScroller = new wxScrolledWindow( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	faceScroller->SetScrollRate( 0, 5 );
	wxBoxSizer* bSizer331;
	bSizer331 = new wxBoxSizer( wxVERTICAL );


	faceScroller->SetSizer( bSizer331 );
	faceScroller->Layout();
	bSizer331->Fit( faceScroller );
	sbSizer5->Add( faceScroller, 1, wxALL|wxEXPAND, 0 );

	m_panel2611 = new wxPanel( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer281;
	bSizer281 = new wxBoxSizer( wxHORIZONTAL );


	bSizer281->Add( 0, 0, 1, wxEXPAND, 5 );

	addFaceButton = new wxBitmapButton( m_panel2611, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	addFaceButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_BUTTON) ) );
	addFaceButton->SetToolTip( _("Add new face") );

	bSizer281->Add( addFaceButton, 0, wxTOP, 5 );

	removeFaceButton = new wxBitmapButton( m_panel2611, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	removeFaceButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_BUTTON) ) );
	removeFaceButton->SetToolTip( _("Remove selected face") );

	bSizer281->Add( removeFaceButton, 0, wxLEFT|wxTOP, 5 );


	m_panel2611->SetSizer( bSizer281 );
	m_panel2611->Layout();
	bSizer281->Fit( m_panel2611 );
	sbSizer5->Add( m_panel2611, 0, wxALL|wxEXPAND, 0 );


	m_panel37->SetSizer( sbSizer5 );
	m_panel37->Layout();
	sbSizer5->Fit( m_panel37 );
	m_panel38 = new wxPanel( m_splitter5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel38, wxID_ANY, _("Levels") ), wxVERTICAL );

	levelScroller = new wxScrolledWindow( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	levelScroller->SetScrollRate( 0, 5 );
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );


	levelScroller->SetSizer( bSizer33 );
	levelScroller->Layout();
	bSizer33->Fit( levelScroller );
	sbSizer6->Add( levelScroller, 1, wxALL|wxEXPAND, 5 );

	m_panel261 = new wxPanel( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );


	bSizer28->Add( 0, 0, 1, wxEXPAND, 5 );

	addLevelButton = new wxBitmapButton( m_panel261, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	addLevelButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_BUTTON) ) );
	addLevelButton->SetToolTip( _("Add new level") );

	bSizer28->Add( addLevelButton, 0, wxTOP, 5 );

	removeLevelButton = new wxBitmapButton( m_panel261, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	removeLevelButton->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_BUTTON) ) );
	removeLevelButton->SetToolTip( _("Remove selected level") );

	bSizer28->Add( removeLevelButton, 0, wxLEFT|wxTOP, 5 );


	m_panel261->SetSizer( bSizer28 );
	m_panel261->Layout();
	bSizer28->Fit( m_panel261 );
	sbSizer6->Add( m_panel261, 0, wxEXPAND | wxALL, 0 );


	m_panel38->SetSizer( sbSizer6 );
	m_panel38->Layout();
	sbSizer6->Fit( m_panel38 );
	m_splitter5->SplitHorizontally( m_panel37, m_panel38, 0 );
	bSizer39->Add( m_splitter5, 1, wxEXPAND, 5 );


	m_panel36->SetSizer( bSizer39 );
	m_panel36->Layout();
	bSizer39->Fit( m_panel36 );
	m_splitter1->SplitHorizontally( m_panel35, m_panel36, 200 );
	bSizer36->Add( m_splitter1, 1, wxEXPAND, 0 );


	subImagesTab->SetSizer( bSizer36 );
	subImagesTab->Layout();
	bSizer36->Fit( subImagesTab );
	m_notebook21->AddPage( subImagesTab, _("Subimages"), false );
	propertiesTab = new wxPanel( m_notebook21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );

	propertyGrid = new wxPropertyGrid(propertiesTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	propertyGrid->SetMinSize( wxSize( 250,-1 ) );

	bSizer37->Add( propertyGrid, 1, wxALL|wxEXPAND, 0 );


	propertiesTab->SetSizer( bSizer37 );
	propertiesTab->Layout();
	bSizer37->Fit( propertiesTab );
	m_notebook21->AddPage( propertiesTab, _("Properties"), false );

	bSizer3->Add( m_notebook21, 0, wxEXPAND | wxALL, 5 );


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

	typeList = new wxListBox( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE );
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

	widthEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000000, 1024 );
	widthEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( widthEdit, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panel10, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer14->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 5 );

	heightEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000000, 1024 );
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

	widthEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000000, 1024 );
	widthEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( widthEdit, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panel10, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer14->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 5 );

	heightEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000000, 1024 );
	heightEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( heightEdit, 0, wxALL, 5 );

	m_staticText17 = new wxStaticText( m_panel10, wxID_ANY, _("Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer14->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	depthEdit = new wxSpinCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000000, 1 );
	depthEdit->SetMinSize( wxSize( 100,-1 ) );

	bSizer14->Add( depthEdit, 0, wxALL, 5 );


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


	bSizer22->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( m_panel15, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer22->Add( m_staticText14, 0, wxALIGN_CENTER|wxALL, 5 );

	wxArrayString typeChoiceChoices;
	typeChoice = new wxChoice( m_panel15, wxID_ANY, wxDefaultPosition, wxDefaultSize, typeChoiceChoices, 0 );
	typeChoice->SetSelection( 0 );
	typeChoice->SetMinSize( wxSize( 150,-1 ) );

	bSizer22->Add( typeChoice, 0, wxALL, 5 );


	m_panel15->SetSizer( bSizer22 );
	m_panel15->Layout();
	bSizer22->Fit( m_panel15 );
	bSizer12->Add( m_panel15, 0, wxEXPAND | wxALL, 0 );

	m_panel40 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxHORIZONTAL );


	bSizer42->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText151 = new wxStaticText( m_panel40, wxID_ANY, _("Format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer42->Add( m_staticText151, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString formatChoiceChoices;
	formatChoice = new wxChoice( m_panel40, wxID_ANY, wxDefaultPosition, wxDefaultSize, formatChoiceChoices, 0 );
	formatChoice->SetSelection( 0 );
	bSizer42->Add( formatChoice, 0, wxALL, 5 );


	m_panel40->SetSizer( bSizer42 );
	m_panel40->Layout();
	bSizer42->Fit( m_panel40 );
	bSizer12->Add( m_panel40, 0, wxEXPAND | wxALL, 0 );

	m_panel41 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );


	bSizer43->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText16 = new wxStaticText( m_panel41, wxID_ANY, _("Compression"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer43->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString compressionChoiceChoices;
	compressionChoice = new wxChoice( m_panel41, wxID_ANY, wxDefaultPosition, wxDefaultSize, compressionChoiceChoices, 0 );
	compressionChoice->SetSelection( 0 );
	bSizer43->Add( compressionChoice, 0, wxALL, 5 );


	m_panel41->SetSizer( bSizer43 );
	m_panel41->Layout();
	bSizer43->Fit( m_panel41 );
	bSizer12->Add( m_panel41, 0, wxEXPAND | wxALL, 0 );

	m_panel16 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );

	additionalPanel = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText15 = new wxStaticText( additionalPanel, wxID_ANY, _("Layers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer24->Add( m_staticText15, 0, wxALIGN_CENTER|wxALL, 5 );

	layersCount = new wxSpinCtrl( additionalPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000, 0 );
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
	this->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	m_panel28 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );

	m_panel18 = new wxPanel( m_panel28, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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
	bSizer30->Add( m_panel18, 1, wxEXPAND | wxALL, 16 );

	m_panel19 = new wxPanel( m_panel28, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );


	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );

	okButton = new wxButton( m_panel19, wxID_ANY, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( okButton, 0, wxALL, 5 );


	m_panel19->SetSizer( bSizer27 );
	m_panel19->Layout();
	bSizer27->Fit( m_panel19 );
	bSizer30->Add( m_panel19, 0, wxEXPAND | wxALL, 0 );


	m_panel28->SetSizer( bSizer30 );
	m_panel28->Layout();
	bSizer30->Fit( m_panel28 );
	bSizer26->Add( m_panel28, 1, wxEXPAND | wxALL, 0 );


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
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	m_panel29 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel29->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );

	previewImage = new wxStaticBitmap( m_panel29, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( previewImage, 0, wxALL|wxEXPAND|wxSHAPED, 5 );

	m_panel32 = new wxPanel( m_panel29, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );


	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );

	nameLabel = new wxStaticText( m_panel32, wxID_ANY, _("Layer / face ID"), wxDefaultPosition, wxDefaultSize, 0 );
	nameLabel->Wrap( -1 );
	bSizer41->Add( nameLabel, 0, wxLEFT|wxRIGHT, 5 );

	measurementsLabel = new wxStaticText( m_panel32, wxID_ANY, _("00px x 00px"), wxDefaultPosition, wxDefaultSize, 0 );
	measurementsLabel->Wrap( -1 );
	bSizer41->Add( measurementsLabel, 0, wxLEFT|wxRIGHT|wxTOP, 5 );


	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panel32->SetSizer( bSizer41 );
	m_panel32->Layout();
	bSizer41->Fit( m_panel32 );
	bSizer31->Add( m_panel32, 1, wxEXPAND | wxALL, 0 );


	m_panel29->SetSizer( bSizer31 );
	m_panel29->Layout();
	bSizer31->Fit( m_panel29 );
	bSizer40->Add( m_panel29, 1, wxEXPAND | wxALL, 0 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer40->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer40 );
	this->Layout();
}

SubimageEntry::~SubimageEntry()
{
}

ProgressDialog::ProgressDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );

	progressBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	progressBar->SetValue( 0 );
	bSizer34->Add( progressBar, 0, wxALL|wxEXPAND, 16 );

	m_panel30 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );


	bSizer35->Add( 0, 0, 1, wxEXPAND, 5 );

	cancelButton = new wxButton( m_panel30, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( cancelButton, 0, wxALL, 5 );


	bSizer35->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panel30->SetSizer( bSizer35 );
	m_panel30->Layout();
	bSizer35->Fit( m_panel30 );
	bSizer34->Add( m_panel30, 0, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer34 );
	this->Layout();

	this->Centre( wxBOTH );
}

ProgressDialog::~ProgressDialog()
{
}
