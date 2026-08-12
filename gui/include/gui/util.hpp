#ifndef _h_textoolkit_gui_util
#define _h_textoolkit_gui_util

#include <wx/wx.h>

namespace textoolkit
{
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
}

#endif