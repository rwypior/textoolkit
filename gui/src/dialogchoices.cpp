#include "gui/dialogchoices.hpp"
#include "gui/util.hpp"
#include "gui/xmlutil.hpp"
#include "common/util.hpp"

#include <wx/filename.h>

namespace
{
	constexpr char XmlRoot[] = "dialogs";
	constexpr char XmlNodeDialog[] = "dialog";
	constexpr char XmlAttrDialogId[] = "id";
}

namespace textoolkit
{
	DialogChoices::DialogChoices()
	{
		bool success = false;

		wxFileName fn(getDialogChoicesPath());
		
		if (fn.Exists())
			success = this->doc.Load(getDialogChoicesPath(), wxXMLDOC_NONE);

		if (!success)
			this->doc.SetRoot(new wxXmlNode(wxXmlNodeType::wxXML_ELEMENT_NODE, XmlRoot));
	}

	bool DialogChoices::hasChoice(const std::string& dlgid) const
	{
		return !!this->findDialogNode(dlgid);
	}

	std::string DialogChoices::getChoice(const std::string& dlgid, const std::string& defaultValue) const
	{
		if (auto node = this->findDialogNode(dlgid))
			return node->GetChildren()->GetContent().ToStdString();
		return defaultValue;
	}

	int DialogChoices::getChoiceInt(const std::string& dlgid, int defaultValue) const
	{
		auto choice = this->getChoice(dlgid, std::to_string(defaultValue));
		return getNumber(choice, defaultValue);
	}

	void DialogChoices::saveChoice(const std::string& dlgid, const std::string& choice)
	{
		wxXmlNode* node = this->findDialogNode(dlgid);
		if (!node)
		{
			node = new wxXmlNode(wxXmlNodeType::wxXML_ELEMENT_NODE, XmlNodeDialog);
			node->AddAttribute(XmlAttrDialogId, dlgid);
			this->doc.GetRoot()->AddChild(node);
		}

		xml::clearNode(*node);
		node->AddChild(new wxXmlNode(wxXmlNodeType::wxXML_TEXT_NODE, "", choice));
		this->doc.Save(getDialogChoicesPath());
	}

	void DialogChoices::saveChoiceInt(const std::string& dlgid, int choice)
	{
		this->saveChoice(dlgid, std::to_string(choice));
	}

	wxXmlNode* DialogChoices::findDialogNode(const std::string& name)
	{
		auto child = this->doc.GetRoot()->GetChildren();
		while (child)
		{
			if (
				child->GetName().ToStdString() == XmlNodeDialog &&
				child->GetAttribute(XmlAttrDialogId).ToStdString() == name
				)
				return child;
			child = child->GetNext();
		}
		return nullptr;
	}

	const wxXmlNode* DialogChoices::findDialogNode(const std::string& name) const
	{
		return const_cast<const wxXmlNode*>(const_cast<DialogChoices*>(this)->findDialogNode(name));
	}
}