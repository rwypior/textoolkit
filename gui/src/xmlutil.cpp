#include "gui/xmlutil.hpp"

namespace textoolkit::xml
{
	wxXmlNode* findXmlNode(wxXmlNode& parent, const std::string& name)
	{
		auto child = parent.GetChildren();
		while (child)
		{
			if (child->GetName().ToStdString() == name)
				return child;
			child = child->GetNext();
		}
		return nullptr;
	}

	void clearNode(wxXmlNode& node)
	{
		auto child = node.GetChildren();
		while (child)
		{
			auto next = child->GetNext();
			node.RemoveChild(child);
			child = next;
		}
	}
}