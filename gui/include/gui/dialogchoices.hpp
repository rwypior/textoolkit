#ifndef _h_textoolkit_gui_dialogchoices
#define _h_textoolkit_gui_dialogchoices

#include <wx/xml/xml.h>

#include <string>

namespace textoolkit
{
	class DialogChoices
	{
	public:
		DialogChoices();

		bool hasChoice(const std::string& dlgid) const;
		std::string getChoice(const std::string& dlgid, const std::string& defaultValue = "") const;
		int getChoiceInt(const std::string& dlgid, int defaultValue = 0) const;
		void saveChoice(const std::string& dlgid, const std::string& choice);
		void saveChoiceInt(const std::string& dlgid, int choice);

	private:
		wxXmlNode* findDialogNode(const std::string& name);
		const wxXmlNode* findDialogNode(const std::string& name) const;

		wxXmlDocument doc;
	};
}

#endif