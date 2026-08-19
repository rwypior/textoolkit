#ifndef _h_textoolkit_gui_util
#define _h_textoolkit_gui_util

#include <string>

#include <wx/wx.h>
#include <wx/dir.h>

namespace textoolkit
{
	std::string getModelsPath();
	std::string getShadersPath();
	wxArrayString getModels();
	wxArrayString getShaders();

	/// Bind an event to given class and all of it's children recursively
	template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
	void bindRecursively(
		wxWindow& wnd,
		const EventTag& eventType,
		void (Class::* method)(EventArg&),
		EventHandler* handler,
		int winid = wxID_ANY,
		int lastId = wxID_ANY,
		wxObject* userData = nullptr
	)
	{
		wnd.Bind(eventType, method, handler);
		for (auto& child : wnd.GetChildren())
		{
			bindRecursively(*child, eventType, method, handler, winid, lastId, userData);
		}
	}

	class WildcardFileTraverser : public wxDirTraverser
	{
	public:
		WildcardFileTraverser(const std::string& wildcard);
		virtual wxDirTraverseResult OnFile(const wxString& filename) override;
		virtual wxDirTraverseResult OnDir(const wxString& dirname) override;
		const wxArrayString& getFoundFiles() const;

	private:
		std::string wildcard;
		wxArrayString files;
	};
}

#endif