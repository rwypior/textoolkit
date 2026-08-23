#include "gui/textoolkitnewdialog.hpp"
#include "gui/textoolkitnewbmppanel.hpp"
#include "gui/textoolkitnewddspanel.hpp"
#include "gui/texture.hpp"
#include "common/image.hpp"
#include "bmp/bmp.hpp"
#include "dds/dds.hpp"

#include <wx/listctrl.h>

#include <map>
#include <functional>
#include <memory>
#include <tuple>

namespace
{
	using createPanelFnc = wxPanel*(*)(wxWindow* parent);
	using createTextureFnc = std::unique_ptr<textoolkit::Texture>(*)(wxWindow* panel);
	using panelTuple = std::tuple<std::string, createPanelFnc, createTextureFnc>;

	wxPanel* createBmpPanel(wxWindow* parent)
	{
		return new textoolkit::TexToolkitnewBmpPanel(parent);
	}

	std::unique_ptr<textoolkit::Texture> createBmp(wxWindow* panel)
	{
		auto bmpPanel = static_cast<textoolkit::TexToolkitnewBmpPanel*>(panel);
		auto bmp = std::make_shared<textoolkit::Bmp>(bmpPanel->getTextureWidth(), bmpPanel->getTextureHeight());
		return std::make_unique<textoolkit::Texture>(std::move(bmp), bmpPanel->getTextureName());
	}

	wxPanel* createDdsPanel(wxWindow* parent)
	{
		return new textoolkit::TexToolkitnewDdsPanel(parent);
	}

	std::unique_ptr<textoolkit::Texture> createDds(wxWindow* panel)
	{
		auto ddsPanel = static_cast<textoolkit::TexToolkitnewDdsPanel*>(panel);
		auto dds = std::make_shared<textoolkit::DDS>(
			ddsPanel->getTextureType(),
			ddsPanel->getFormat(),
			ddsPanel->getCompression(),
			glm::vec3(ddsPanel->getTextureWidth(), ddsPanel->getTextureHeight(), ddsPanel->getTextureDepth()),
			ddsPanel->getLayersCount(),
			ddsPanel->getGenerateMipmaps()
		);
		return std::make_unique<textoolkit::Texture>(std::move(dds), ddsPanel->getTextureName());
	}

	std::map<textoolkit::Image::Type, panelTuple> imageTypes{
		{ textoolkit::Image::Type::BMP, {"Bitmap (.bmp)", createBmpPanel, createBmp} },
		{ textoolkit::Image::Type::DDS, {"Direct Draw Surface (.dds)", createDdsPanel, createDds} }
	};
}

namespace textoolkit
{
	TexToolkitNewDialog::TexToolkitNewDialog(wxWindow* parent)
		:
		NewDialog(parent)
	{
		for (auto& [type, tuple] : imageTypes)
		{
			auto name = std::get<0>(tuple);
			this->typeList->Append(name, reinterpret_cast<void*>(type));
		}

		this->typeList->Bind(wxEVT_LISTBOX, &TexToolkitNewDialog::typeChangedEvent, this);
		this->okButton->Bind(wxEVT_BUTTON, &TexToolkitNewDialog::okEvent, this);
		this->cancelButton->Bind(wxEVT_BUTTON, &TexToolkitNewDialog::cancelEvent, this);

		this->typeList->Select(0);
		this->updatePropertiesWidget();
	}

	TexToolkitNewDialog::~TexToolkitNewDialog() = default;

	std::unique_ptr<Texture> TexToolkitNewDialog::createTexture()
	{
		auto type = static_cast<Image::Type>(reinterpret_cast<size_t>(this->typeList->GetClientData(this->typeList->GetSelection())));
		auto it = imageTypes.find(type);
		if (it == imageTypes.end())
		{
			assert(!"Invalid image type");
			return nullptr;
		}

		auto children = this->propertiesPanel->GetChildren();
		assert(children.size() == 1 && "Must contain exactly one child");

		auto createFnc = std::get<2>(it->second);
		return createFnc(children.front());
	}

	std::string TexToolkitNewDialog::getTextureName() const
	{
		auto children = this->propertiesPanel->GetChildren();
		assert(children.size() == 1 && "Must contain exactly one child");

		auto panel = dynamic_cast<NewImagePanel*>(children.front());
		if (!panel)
		{
			assert(!"Invalid new texture panel");
			return "";
		}

		return panel->getTextureName();
	}

	void TexToolkitNewDialog::updatePropertiesWidget()
	{
		auto type = static_cast<Image::Type>(reinterpret_cast<size_t>(this->typeList->GetClientData(this->typeList->GetSelection())));
		auto typedata = imageTypes.find(type);
		if (typedata == imageTypes.end())
		{
			assert(!"Invalid image type");
			return;
		}

		this->propertiesPanel->Freeze();

		auto& children = this->propertiesPanel->GetChildren();
		while (!children.empty())
		{
			auto& child = children.front();
			child->Destroy();
		}

		auto& tuple = typedata->second;
		auto createPanel = std::get<1>(tuple);
		auto* widget = createPanel(this->propertiesPanel);
		this->propertiesPanel->GetSizer()->Add(widget);

		this->propertiesPanel->Layout();
		this->propertiesPanel->Thaw();
	}

	void TexToolkitNewDialog::typeChangedEvent(wxCommandEvent& /*event*/)
	{
		this->updatePropertiesWidget();
	}

	void TexToolkitNewDialog::okEvent(wxCommandEvent& event)
	{
		this->EndModal(wxID_OK);
	}

	void TexToolkitNewDialog::cancelEvent(wxCommandEvent& event)
	{
		this->EndModal(wxID_CANCEL);
	}
}
