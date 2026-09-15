#ifndef _h_textoolkit_gui_xmlutil
#define _h_textoolkit_gui_xmlutil

#include <wx/xml/xml.h>

#include <string>

namespace textoolkit::xml
{
	wxXmlNode* findXmlNode(wxXmlNode& parent, const std::string& name);
	void clearNode(wxXmlNode& node);
}

#endif