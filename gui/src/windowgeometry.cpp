#include "gui/windowgeometry.hpp"
#include "gui/util.hpp"
#include "common/util.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <unordered_map>

namespace
{
	constexpr char XmlRoot[] = "geometry";

	std::unordered_map<std::string, std::string> geometryDictionary{
		{ "x", "window_x" },
		{ "y", "window_y" },
		{ "w", "window_w" },
		{ "h", "window_h" }
	};
}

namespace textoolkit
{
	WindowGeometrySerializer::WindowGeometrySerializer()
	{
		bool success = false;

		wxFileName fn(getWindowGeometryPath());

		if (fn.Exists())
			success = this->doc.Load(getWindowGeometryPath(), wxXMLDOC_NONE);

		if (!success)
			this->doc.SetRoot(new wxXmlNode(wxXmlNodeType::wxXML_ELEMENT_NODE, XmlRoot));
	}

	WindowGeometrySerializer::~WindowGeometrySerializer()
	{
		if (this->changed)
			this->doc.Save(getWindowGeometryPath());
	}

	bool WindowGeometrySerializer::SaveField(const wxString& name, int value) const
	{
		if (geometryDictionary.count(name.ToStdString()))
		{
			std::string key = geometryDictionary.at(name.ToStdString());
			std::string val = std::to_string(value);
			if (this->get(key) != val)
				this->changed = true;
			this->set(key, std::to_string(value));
			return true;
		}

		return false;
	}

	bool WindowGeometrySerializer::RestoreField(const wxString& name, int* value)
	{
		if (geometryDictionary.count(name.ToStdString()))
		{
			std::string key = geometryDictionary.at(name.ToStdString());
			auto val = this->get(key);
			if (!isNumber(val))
				return false;
			*value = std::stoi(val);
			return true;
		}

		return false;
	}

	bool WindowGeometrySerializer::isChanged() const
	{
		return this->changed;
	}

	std::string WindowGeometrySerializer::get(const std::string& key) const
	{
		auto child = this->doc.GetRoot()->GetChildren();
		while (child)
		{
			if (child->GetName().ToStdString() == key)
				return child->GetChildren()->GetContent().ToStdString();
			child = child->GetNext();
		}
		return "";
	}

	void WindowGeometrySerializer::set(const std::string& key, const std::string& val) const
	{
		auto child = this->doc.GetRoot()->GetChildren();
		while (child)
		{
			if (child->GetName().ToStdString() == key)
			{
				child->GetChildren()->SetContent(val);
				return;
			}
			child = child->GetNext();
		}
		auto newnode = new wxXmlNode(wxXmlNodeType::wxXML_ELEMENT_NODE, key);
		newnode->AddChild(new wxXmlNode(wxXmlNodeType::wxXML_TEXT_NODE, "", val));
		this->doc.GetRoot()->AddChild(newnode);
	}
}