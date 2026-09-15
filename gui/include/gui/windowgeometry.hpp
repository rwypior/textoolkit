#ifndef _h_textoolkit_gui_windowgeometry
#define _h_textoolkit_gui_windowgeometry

#include <wx/xml/xml.h>
#include <wx/wx.h>

#include <string>
#include <optional>

namespace textoolkit
{
	class WindowGeometrySerializer : public wxTopLevelWindow::GeometrySerializer
	{
	public:
		WindowGeometrySerializer();
		~WindowGeometrySerializer();
		bool SaveField(const wxString& name, int value) const override;
		bool RestoreField(const wxString& name, int* value) override;
		bool isChanged() const;

	private:
		std::string get(const std::string& key) const;
		void set(const std::string& key, const std::string& val) const;

		mutable wxXmlDocument doc;
		mutable bool changed = false;
	};
}

#endif