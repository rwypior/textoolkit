#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitsubimageentry.hpp"
#include "gui/textoolkitprogressdialog.hpp"
#include "gui/texture.hpp"
#include "gui/util.hpp"
#include "gui/importdlg.hpp"
#include "gui/dialogchoices.hpp"
#include "common/image.hpp"
#include "common/logger.hpp"
#include "texture/textureloader.hpp"
#include "renderer/modeldatabase.hpp"
#include "renderer/model.hpp"
#include "renderer/object.hpp"

#include <wx/stdpaths.h>

#include <unordered_set>
#include <sstream>

namespace
{
	constexpr char ImportDlgId[] = "import";
	constexpr char ImportDlgFilterId[] = "importfilter";

	template<typename T>
	T getProperty(wxPGProperty* prop)
	{
		auto enumprop = static_cast<wxEnumProperty*>(prop);
		return static_cast<T>(enumprop->GetChoices().GetValue(enumprop->GetChoiceSelection()));
	}
	
	bool getBoolProperty(wxPGProperty* prop)
	{
		return static_cast<wxBoolProperty*>(prop)->GetValue().GetBool();
	}
}

namespace textoolkit
{
	// Event

	TexToolkitTextureViewEvent::TexToolkitTextureViewEvent(wxEventType eventType, TexToolkitTextureView* view)
		: wxCommandEvent(eventType, wxID_ANY)
		, view(view)
	{
	}

	TexToolkitTextureViewEvent* TexToolkitTextureViewEvent::Clone() const
	{
		return new TexToolkitTextureViewEvent(*this);
	}

	wxDEFINE_EVENT(texEVT_TEXTUREVIEW_MODIFIED, TexToolkitTextureViewEvent);

	// View

	TexToolkitTextureView::TexToolkitTextureView(std::unique_ptr<GuiTexture>&& texture, renderer::ModelDatabase& modelDatabase, wxWindow* parent)
		: TextureView(parent)
		, texture(std::move(texture))
		, progressNotifier(progressNotifier)
		, modelDatabase(modelDatabase)
	{
		this->object = this->canvas->addObject(std::make_unique<renderer::Object>(mainObjectName));
		this->canvas->setImage(this->texture->getImage());

		this->flatView->SetScaleMode(wxStaticBitmapBase::ScaleMode::Scale_AspectFit);
		this->m_notebook2->ChangeSelection(1);

		// TODO - remove this when editor is ready
		this->m_notebook2->DeletePage(2);
		this->m_panel251->Destroy(); // Add buttons
		this->m_panel2611->Destroy(); // Add buttons
		this->m_panel261->Destroy(); // Add buttons
		//

		this->depthSlider->SetMin(0);
		this->depthSlider->SetMax(this->texture->getImage().getDepth() - 1);
		this->depthSlider->SetValue(0);
		this->depthSpin->SetMin(0);
		this->depthSpin->SetMax(this->texture->getImage().getDepth() - 1);
		this->depthSpin->SetValue(0);
		
		this->setupProperties();
		this->updateModels();
		this->updateDisplayModes();

		this->updateFlatView();
		this->updateSubimages();
		this->update3DView();
		this->setupUserProperties();

		this->refreshDisplayModeListButton->Bind(wxEVT_BUTTON, &TexToolkitTextureView::displayModeUpdateButtonClicked, this);
		this->displaymode->Bind(wxEVT_COMBOBOX, &TexToolkitTextureView::displayModeSelected, this);
		this->propertyGrid->Bind(wxEVT_PG_CHANGED, &TexToolkitTextureView::propertyChanged, this);
		this->selectBaseLink->Bind(wxEVT_HYPERLINK, &TexToolkitTextureView::selectBaseClicked, this);
		this->depthSlider->Bind(wxEVT_SLIDER, &TexToolkitTextureView::depthSliderChanged, this);
		this->depthSpin->Bind(wxEVT_SPINCTRL, &TexToolkitTextureView::depthSpinChanged, this);
	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;

	GuiTexture& TexToolkitTextureView::getTexture()
	{
		return *this->texture;
	}

	std::string TexToolkitTextureView::getDescription() const
	{
		if (!this->texture)
			return "Blank texture";

		std::stringstream str;

		str << Image::translateTextureType(this->texture->getImage().getTextureType());

		if (!this->texture->getPath().empty())
			str << ", " << this->texture->getPath();
		else
			str << ", " << this->texture->getName();

		str << ", " << 
			"w " << this->texture->getImage().getWidth() << "px, " <<
			"h " << this->texture->getImage().getHeight() << "px, " <<
			"d " << this->texture->getImage().getDepth() << "px";

		return str.str();
	}

	bool TexToolkitTextureView::batchImportCompatible() const
	{
		if (!this->texture)
			return false;

		switch (this->texture->getImage().getTextureType())
		{
		case Image::TextureType::TextureCube:
		case Image::TextureType::Texture2DArray:
			return true;
		default:
			return false;
		}
	}

	TexToolkitTextureView::SubTextureContainer TexToolkitTextureView::createLayers(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int layers = image.getLayers();
		FiniteThreadpool threadpool(progressNotifier);
		TexToolkitTextureView::SubTextureContainer subtextures;
		for (unsigned int layer = 0; layer < layers; layer++)
		{
			threadpool.enqueue([this, layer, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createLayer(*this->texture, layer, this->currentDepth));
				auto lck = threadpool.lock();
				subtextures.push_back(std::move(subtexture));
				});
		}
		threadpool.start();
		threadpool.waitForAll();

		std::sort(subtextures.begin(), subtextures.end(), [](auto& a, auto& b) {
			return a->getLayer() < b->getLayer();
		});

		return subtextures;
	}

	TexToolkitTextureView::SubTextureContainer TexToolkitTextureView::createFaces(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int faces = image.getFaces();
		FiniteThreadpool threadpool(progressNotifier);
		TexToolkitTextureView::SubTextureContainer subtextures;
		for (unsigned int face = 0; face < faces; face++)
		{
			threadpool.enqueue([this, face, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createFace(*this->texture, this->currentLayer, face, this->currentDepth));
				auto lck = threadpool.lock();
				subtextures.push_back(std::move(subtexture));
			});
		}
		threadpool.start();
		threadpool.waitForAll();

		std::sort(subtextures.begin(), subtextures.end(), [](auto& a, auto& b) {
			return a->getFace() < b->getFace();
		});

		return subtextures;
	}

	TexToolkitTextureView::SubTextureContainer TexToolkitTextureView::createLevels(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int levels = this->getLevels(image);
		FiniteThreadpool threadpool(progressNotifier);
		TexToolkitTextureView::SubTextureContainer subtextures;
		for (unsigned int level = 0; level < levels; level++)
		{
			threadpool.enqueue([this, level, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createLevel(*this->texture, this->currentLayer, this->currentFace, level, this->currentDepth));
				auto lck = threadpool.lock();
				subtextures.push_back(std::move(subtexture));
			});
		}
		threadpool.start();
		threadpool.waitForAll();

		std::sort(subtextures.begin(), subtextures.end(), [](auto& a, auto& b) {
			return a->getLevel() < b->getLevel();
		});

		return subtextures;
	}

	unsigned int TexToolkitTextureView::getLevels(const Image& image) const
	{
		auto levels = image.getLevels();
		unsigned int count = 0;
		for (unsigned int i = 0; i < levels; i++)
		{
			count += this->currentDepth < image.getDepth(i);
		}
		return count;
	}

	TexToolkitSubimageEntry* TexToolkitTextureView::getLayer(unsigned int layer)
	{
		auto& children = this->layerScroller->GetChildren();
		for (auto& child : children)
		{
			auto& subentry = static_cast<TexToolkitSubimageEntry&>(*child);
			auto tex = subentry.getTexture();
			if (tex->getLayer() == layer)
				return &subentry;
		}
		return nullptr;
	}

	TexToolkitSubimageEntry* TexToolkitTextureView::getFace(unsigned int face)
	{
		auto& children = this->faceScroller->GetChildren();
		for (auto& child : children)
		{
			auto& subentry = static_cast<TexToolkitSubimageEntry&>(*child);
			auto tex = subentry.getTexture();
			if (tex->getFace() == face)
				return &subentry;
		}
		return nullptr;
	}

	TexToolkitSubimageEntry* TexToolkitTextureView::getLevel(unsigned int level)
	{
		auto& children = this->levelScroller->GetChildren();
		for (auto& child : children)
		{
			auto& subentry = static_cast<TexToolkitSubimageEntry&>(*child);
			auto tex = subentry.getTexture();
			if (tex->getLevel() == level)
				return &subentry;
		}
		return nullptr;
	}

	unsigned int TexToolkitTextureView::getFaceIndex(Image::CubeFace face) const
	{
		const auto alignmentProp = this->propertyGrid->GetProperty(propGrp3DCubemapAlignment);
		const unsigned int childCount = alignmentProp->GetChildCount();
		for (unsigned int i = 0; i < childCount; i++)
		{
			const auto prop = alignmentProp->Item(i);
			const auto propAlignment = getProperty<Image::CubeFace>(prop);
			if (propAlignment == face)
				return i;
		}
		return 0;
	}

	renderer::DisplayMode* TexToolkitTextureView::getDisplayMode()
	{
		unsigned int modeSelection = this->displaymode->GetSelection();
		if (modeSelection == wxNOT_FOUND)
			return nullptr;
		return reinterpret_cast<renderer::DisplayMode*>(this->displaymode->GetClientData(modeSelection));
	}

	bool TexToolkitTextureView::isUserProperty(const std::string& propname) const
	{
		auto prop = this->propertyGrid->GetProperty(propname);
		if (!prop)
			return false;

		if (auto parent = prop->GetParent())
		{
			auto userpropertiesgrp = this->propertyGrid->GetProperty(propGrp3DUserProperties);
			return parent->GetName() == userpropertiesgrp->GetName();
		}

		return false;
	}

	void TexToolkitTextureView::updateFlatView(unsigned int layer, unsigned int face, unsigned int level, unsigned int depth)
	{
		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		this->mainTexture = GuiSubTexture::createLevel(*this->texture, layer, face, level, depth);
		this->flatViewBitmap = this->mainTexture.getBitmap();

		wxString type = Image::translateTextureType(image.getTextureType());

		this->flatViewImageDetails->SetLabel(
			wxString::Format("%s; Layer %d; Face %d; Level %d; Depth %d; %dx%d px",
				type,
				this->mainTexture.getLayer(),
				this->mainTexture.getFace(),
				this->mainTexture.getLevel(),
				this->mainTexture.getDepth(),
				this->mainTexture.getSize().x,
				this->mainTexture.getSize().y
			));

		this->flatView->SetBitmap(this->flatViewBitmap);
		this->flatView->Refresh();
		this->flatView->Update();
		this->flatView->GetParent()->Layout();
	}

	void TexToolkitTextureView::updateSubimages(UpdateTargets targets)
	{
		SubTextureContainer layers;
		SubTextureContainer faces;
		SubTextureContainer levels;

		FiniteThreadpool threadpool(targets.count());
		std::vector<std::unique_ptr<SubTexture>> subtextures;
		if (targets.test(UpdateTarget::Layers))
			threadpool.enqueue([this, &layers]() {
				layers = this->createLayers();
			});
		if (targets.test(UpdateTarget::Faces))
			threadpool.enqueue([this, &faces]() {
				faces = this->createFaces();
			});
		if (targets.test(UpdateTarget::Levels))
			threadpool.enqueue([this, &levels]() {
				levels = this->createLevels();
			});
		threadpool.start();

		threadpool.waitForAll();

		if (targets.test(UpdateTarget::Layers))
			this->updateLayers(&layers);
		if (targets.test(UpdateTarget::Faces))
			this->updateFaces(&faces);
		if (targets.test(UpdateTarget::Levels))
			this->updateLevels(&levels);
	}

	void TexToolkitTextureView::updateLayers(SubTextureContainer* subtextures)
	{
		FreezeGuard fg(*this->layerScroller);

		auto& children = this->layerScroller->GetChildren();
		while (!children.empty())
			children.front()->Destroy();

		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		unsigned int layers = image.getLayers();

		SubTextureContainer tmp;
		if (!subtextures)
		{
			tmp = createLayers();
			subtextures = &tmp;
		}

		unsigned int idx = 0;
		for (auto& subtexture : *subtextures)
		{
			auto entry = new TexToolkitSubimageEntry(this->layerScroller, std::move(subtexture), idx == 0);
			entry->setLineVisibility(idx++ != layers - 1);
			this->layerScroller->GetSizer()->Add(entry);
			this->layerScroller->GetSizer()->FitInside(this->layerScroller);
			entry->Bind(texEVT_SUBIMAGE_SELECTED, &TexToolkitTextureView::layerSelected, this);
			entry->Bind(texEVT_SUBIMAGE_IMPORT_REQUESTED, &TexToolkitTextureView::importRequested, this);
		}

		this->layerScroller->Layout();
	}

	void TexToolkitTextureView::updateFaces(SubTextureContainer* subtextures)
	{
		FreezeGuard fg(*this->faceScroller);

		auto& children = this->faceScroller->GetChildren();
		while (!children.empty())
			children.front()->Destroy();

		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		unsigned int faces = image.getFaces();

		SubTextureContainer tmp;
		if (!subtextures)
		{
			tmp = createLayers();
			subtextures = &tmp;
		}

		unsigned int idx = 0;
		for (auto& subtexture : *subtextures)
		{
			auto entry = new TexToolkitSubimageEntry(this->faceScroller, std::move(subtexture), idx == 0);
			entry->setLineVisibility(idx++ != faces - 1);
			this->faceScroller->GetSizer()->Add(entry);
			this->faceScroller->GetSizer()->FitInside(this->faceScroller);
			entry->Bind(texEVT_SUBIMAGE_SELECTED, &TexToolkitTextureView::faceSelected, this);
			entry->Bind(texEVT_SUBIMAGE_IMPORT_REQUESTED, &TexToolkitTextureView::importRequested, this);
		}

		this->faceScroller->Layout();
	}

	void TexToolkitTextureView::updateLevels(SubTextureContainer* subtextures)
	{
		FreezeGuard fg(*this->levelScroller);

		auto& children = this->levelScroller->GetChildren();
		while (!children.empty())
			children.front()->Destroy();

		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		unsigned int levels = image.getLevels();

		SubTextureContainer tmp;
		if (!subtextures)
		{
			tmp = createLevels();
			subtextures = &tmp;
		}

		unsigned int idx = 0;
		for (auto& subtexture : *subtextures)
		{
			auto entry = new TexToolkitSubimageEntry(this->levelScroller, std::move(subtexture), idx == 0);
			entry->setLineVisibility(idx++ != levels - 1);
			this->levelScroller->GetSizer()->Add(entry);
			this->levelScroller->GetSizer()->FitInside(this->levelScroller);
			entry->Bind(texEVT_SUBIMAGE_SELECTED, &TexToolkitTextureView::levelSelected, this);
			entry->Bind(texEVT_SUBIMAGE_IMPORT_REQUESTED, &TexToolkitTextureView::importRequested, this);
		}

		this->levelScroller->Layout();
	}

	void TexToolkitTextureView::updateAllPreviews()
	{
		for (unsigned int layer = 0; layer < this->texture->getImage().getLayers(); layer++)
		{
			this->getLayer(layer)->updatePreview();
		}

		for (unsigned int face = 0; face < this->texture->getImage().getFaces(); face++)
		{
			this->getFace(face)->updatePreview();
		}

		for (unsigned int level = 0; level < this->texture->getImage().getLevels(); level++)
		{
			this->getLevel(level)->updatePreview();
		}

		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel, this->currentDepth);
		this->updateSubimages();
	}

	void TexToolkitTextureView::reuploadTexture()
	{
		this->canvas->reuploadTexture();
	}

	void TexToolkitTextureView::refreshView()
	{
		this->updateSubimages({ UpdateTarget::Layers, UpdateTarget::Faces, UpdateTarget::Levels });
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel, this->currentDepth);
	}

	void TexToolkitTextureView::setupProperties()
	{
		wxPGChoices wrappingChoices;
		wrappingChoices.Add("Clamp to edge", static_cast<int>(renderer::Wrapping::ClampToEdge));
		wrappingChoices.Add("Clamp to border", static_cast<int>(renderer::Wrapping::ClampToBorder));
		wrappingChoices.Add("Mirrored repeat", static_cast<int>(renderer::Wrapping::MirroredRepeat));
		wrappingChoices.Add("Repeat", static_cast<int>(renderer::Wrapping::Repeat));
		wrappingChoices.Add("Mirrored clamp to edge", static_cast<int>(renderer::Wrapping::MirroredClampToEdge));

		wxPGChoices filterMinChoices;
		filterMinChoices.Add("Nearest", static_cast<int>(renderer::FilteringMin::Nearest));
		filterMinChoices.Add("Linear", static_cast<int>(renderer::FilteringMin::Linear));
		filterMinChoices.Add("Nearest mipmap nearest", static_cast<int>(renderer::FilteringMin::NearestMipmapNearest));
		filterMinChoices.Add("Linear mipmap nearest", static_cast<int>(renderer::FilteringMin::LinearMipmapNearest));
		filterMinChoices.Add("Nearest mipmap linear", static_cast<int>(renderer::FilteringMin::NearestMipmapLinear));
		filterMinChoices.Add("Linear mipmap linear", static_cast<int>(renderer::FilteringMin::LinearMipmapLinear));

		wxPGChoices filterMagChoices;
		filterMagChoices.Add("Nearest", static_cast<int>(renderer::FilteringMag::Nearest));
		filterMagChoices.Add("Linear", static_cast<int>(renderer::FilteringMag::Linear));

		this->propertyGrid->Append(new wxPropertyCategory("3D display settings", propGrp3DDisplaySettings));
		this->propertyGrid->Append(new wxEnumProperty("Wrap S", propDisplayWrapS, wrappingChoices, 3));
		this->propertyGrid->Append(new wxEnumProperty("Wrap T", propDisplayWrapT, wrappingChoices, 3));
		this->propertyGrid->Append(new wxEnumProperty("Filtering min", propDisplayFilterMin, filterMinChoices));
		this->propertyGrid->Append(new wxEnumProperty("Filtering mag", propDisplayFilterMag, filterMagChoices));
		this->propertyGrid->Append(new wxBoolProperty("Show wireframe", propDisplayWireframe));

		wxPGChoices alignmentChoices;
		alignmentChoices.Add("Positive X", static_cast<int>(Image::CubeFace::PositiveX));
		alignmentChoices.Add("Negative X", static_cast<int>(Image::CubeFace::NegativeX));
		alignmentChoices.Add("Positive Y", static_cast<int>(Image::CubeFace::PositiveY));
		alignmentChoices.Add("Negative Y", static_cast<int>(Image::CubeFace::NegativeY));
		alignmentChoices.Add("Positive Z", static_cast<int>(Image::CubeFace::PositiveZ));
		alignmentChoices.Add("Negative Z", static_cast<int>(Image::CubeFace::NegativeZ));

		this->propertyGrid->Append(new wxPropertyCategory("Cubemap alignment", propGrp3DCubemapAlignment));
		this->propertyGrid->Append(new wxEnumProperty("Face 0", propCubeAlignment0, alignmentChoices, 0));
		this->propertyGrid->Append(new wxEnumProperty("Face 1", propCubeAlignment1, alignmentChoices, 1));
		this->propertyGrid->Append(new wxEnumProperty("Face 2", propCubeAlignment2, alignmentChoices, 2));
		this->propertyGrid->Append(new wxEnumProperty("Face 3", propCubeAlignment3, alignmentChoices, 3));
		this->propertyGrid->Append(new wxEnumProperty("Face 4", propCubeAlignment4, alignmentChoices, 4));
		this->propertyGrid->Append(new wxEnumProperty("Face 5", propCubeAlignment5, alignmentChoices, 5));

		this->propertyGrid->Append(new wxPropertyCategory("User properties", propGrp3DUserProperties));

		this->canvas->setWrappingS(getProperty<renderer::Wrapping>(this->propertyGrid->GetProperty(propDisplayWrapS)));
		this->canvas->setWrappingT(getProperty<renderer::Wrapping>(this->propertyGrid->GetProperty(propDisplayWrapT)));
		this->canvas->setFilterMin(getProperty<renderer::FilteringMin>(this->propertyGrid->GetProperty(propDisplayFilterMin)));
		this->canvas->setFilterMag(getProperty<renderer::FilteringMag>(this->propertyGrid->GetProperty(propDisplayFilterMag)));
		this->canvas->setShowWireframe(getBoolProperty(this->propertyGrid->GetProperty(propDisplayWireframe)));
		this->canvas->setCubeAlignment({ 
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)),
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)),
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)),
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)),
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)),
			getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5))
		});
	}

	void TexToolkitTextureView::setupUserProperties()
	{
		auto displayMode = this->getDisplayMode();
		if (!displayMode)
			return;

		auto userpropertiesgrp = this->propertyGrid->GetProperty(propGrp3DUserProperties);
		userpropertiesgrp->DeleteChildren();

		for (auto& propstr : displayMode->properties)
		{
			auto splitted = split(propstr, ":");
			std::string propname = splitted[0];
			std::string propval;
			if (splitted.size() == 2)
				propval = splitted[1];
			auto prop = this->canvas->getUserPropertyType(propname);
			if (!prop)
			{
				textoolkit::Logger::getLogger() << "Cannot find user property " << propname << " in current shader";
				continue;
			}

			auto type = *prop;
			switch (type)
			{
			case renderer::UniformType::Int:
				userpropertiesgrp->AppendChild(new wxIntProperty(propname, propname, propval.empty() ? 0 : std::stoi(propval)));
				break;
			case renderer::UniformType::Uint:
				userpropertiesgrp->AppendChild(new wxUIntProperty(propname, propname, propval.empty() ? 0 : std::stoi(propval)));
				break;
			case renderer::UniformType::Float:
				userpropertiesgrp->AppendChild(new wxFloatProperty(propname, propname, propval.empty() ? 0 : std::stof(propval)));
				break;
			case renderer::UniformType::Bool:
				userpropertiesgrp->AppendChild(new wxBoolProperty(propname, propname, propval.empty() ? 0 : std::stoi(propval)));
				break;
			}
		}

		this->propertyGrid->Update();
		this->propertyGrid->Refresh();

		this->updateUserProperties();
	}

	void TexToolkitTextureView::update3DView()
	{
		auto displayMode = this->getDisplayMode();
		auto model = this->modelDatabase.findModel(displayMode->model);
		if (!model)
			return;
		this->object->setModel(std::move(model));
		this->canvas->setDisplayMode(*displayMode);
		this->canvas->Refresh();
	}

	void TexToolkitTextureView::updateModels()
	{
		auto modelPaths = getModels();
		this->modelDatabase.loadModels(modelPaths);
	}

	void TexToolkitTextureView::updateDisplayModes()
	{
		auto displayModesPath = getDisplayModePath();
		this->displayModes = loadDisplayModes(displayModesPath);
		if (this->displayModes.empty())
		{
			wxMessageBox("Cannot open displaymodes.ini", "textoolkit - error", wxOK | wxICON_ERROR, this);
			exit(1);
		}
		this->updateDisplayModeList();

		// Select default display mode if applicable
		this->displaymode->SetSelection(0);
		for (unsigned int i = 0; i < this->displaymode->GetCount(); i++)
		{
			auto displayMode = reinterpret_cast<renderer::DisplayMode*>(this->displaymode->GetClientData(i));
			std::unordered_set<Image::TextureType> types;
			for (const auto& defaultType : displayMode->defaultFor)
				types.insert(Image::translateTextureType(defaultType));
			if (types.count(this->texture->getImage().getTextureType()) >= 1)
			{
				this->displaymode->SetSelection(i);
				break;
			}
		}
	}

	void TexToolkitTextureView::updateDisplayModeList()
	{
		this->displaymode->Clear();
		for (auto& entry : this->displayModes)
		{
			this->displaymode->Append(entry.second.name, reinterpret_cast<void*>(&entry.second));
		}
	}

	void TexToolkitTextureView::importImage(SubTexture::Type type, unsigned int layer, unsigned int face, unsigned int level)
	{
		TextureLoader loader;
		DialogChoices choices;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		ImportDlg dlg(this, "Open image", choices.getChoice(ImportDlgId, picturesDir.ToStdString()), wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		dlg.SetFilterIndex(choices.getChoiceInt(ImportDlgFilterId, loader.getFilterIndexAll()));

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		choices.saveChoice(ImportDlgId, dlg.GetDirectory().ToStdString());
		choices.saveChoiceInt(ImportDlgFilterId, dlg.GetFilterIndex());

		auto interpolation = dlg.getInterpolation();

		auto tex = GuiTexture(std::move(*loader.loadTexture(dlg.GetPath().ToStdString())));
		switch (type)
		{
		case GuiSubTexture::Type::Layer:
			this->importLayer(tex, layer, interpolation);
			break;
		case GuiSubTexture::Type::Face:
			this->importFace(tex, layer, face, interpolation);
			break;
		case GuiSubTexture::Type::Level:
			this->importLevel(tex, layer, face, level, interpolation);
			break;
		}

		this->updateFlatView(layer, face, level, this->currentDepth);
		this->updateSubimages();


	}

	void TexToolkitTextureView::importImage()
	{
		this->importImage(this->currentType, this->currentLayer, this->currentFace, this->currentLevel);
	}

	void TexToolkitTextureView::importLayer(GuiTexture& texture, unsigned int layer, InterpolationMinMag interpolation, bool performUpdate, bool reupload)
	{
		for (unsigned int face = 0; face < this->texture->getImage().getFaces(); face++)
		{
			for (unsigned int level = 0; level < this->getLevels(this->texture->getImage()); level++)
			{
				auto source = GuiSubTexture::createLevel(texture, 0, 0, 0);
				auto destination = GuiSubTexture::createLevel(*this->texture, layer, face, level, this->currentDepth);
				destination.set(source, interpolation);

				auto subentry = this->getLevel(level);
				if (performUpdate && subentry)
					subentry->updatePreview();
			}

			auto subentry = this->getFace(face);
			if (performUpdate && subentry)
				subentry->updatePreview();
		}

		auto subentry = this->getLayer(layer);
		if (performUpdate && subentry)
			subentry->updatePreview();

		if (reupload)
			this->canvas->reuploadTexture();

		this->reportModified();
	}

	void TexToolkitTextureView::importFace(GuiTexture& texture, unsigned int layer, unsigned int face, InterpolationMinMag interpolation, bool performUpdate, bool reupload)
	{
		for (unsigned int level = 0; level < this->getLevels(this->texture->getImage()); level++)
		{
			auto source = GuiSubTexture::createLevel(texture, 0, 0, 0);
			auto destination = GuiSubTexture::createLevel(*this->texture, layer, face, level, this->currentDepth);
			destination.set(source, interpolation);

			auto subentry = this->getLevel(level);
			if (performUpdate && subentry)
				subentry->updatePreview();
		}

		auto subentry = this->getFace(face);
		if (performUpdate && subentry)
			subentry->updatePreview();

		if (reupload)
			this->canvas->reuploadTexture();

		this->reportModified();
	}

	void TexToolkitTextureView::importLevel(GuiTexture& texture, unsigned int layer, unsigned int face, unsigned int level, InterpolationMinMag interpolation, bool performUpdate, bool reupload)
	{
		auto source = GuiSubTexture::createInternalLevel(texture, 0, 0, 0);
		auto destination = GuiSubTexture::createInternalLevel(*this->texture, layer, face, level, this->currentDepth);
		destination.set(source, interpolation);

		auto subentry = this->getLevel(level);
		if (performUpdate && subentry)
			subentry->updatePreview();

		if (reupload)
			this->canvas->reuploadTexture();

		this->reportModified();
	}

	void TexToolkitTextureView::reportModified()
	{
		this->texture->markAsModified();
		TexToolkitTextureViewEvent event(texEVT_TEXTUREVIEW_MODIFIED, this);
		this->ProcessEvent(event);
	}

	void TexToolkitTextureView::deselectOthers(wxScrolledWindow* scroller, TexToolkitSubimageEntry* entry)
	{
		for (auto& child : scroller->GetChildren())
		{
			auto* subEntryChild = static_cast<TexToolkitSubimageEntry*>(child);
			if (entry == subEntryChild)
				continue;

			subEntryChild->deselect();
		}
	}

	void TexToolkitTextureView::updateUserProperties()
	{
		auto userpropertiesgrp = this->propertyGrid->GetProperty(propGrp3DUserProperties);
		for (unsigned int i = 0; i < userpropertiesgrp->GetChildCount(); i++)
		{
			auto prop = userpropertiesgrp->Item(i);
			auto renderprop = createRenderProperty(prop->GetValue());
			auto propname = prop->GetBaseName();
			this->canvas->setUserProperty(propname.ToStdString(), renderprop);
		}
	}

	void TexToolkitTextureView::selectDepth(unsigned int depth)
	{
		this->depthSlider->SetValue(0);
		this->depthSpin->SetValue(0);
		this->currentDepth = this->depthSlider->GetValue();
	}

	void TexToolkitTextureView::layerSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->layerScroller, event.entry);
		this->currentType = SubTexture::Type::Layer;
		this->currentLayer = event.entry->getTexture()->getLayer();
		this->currentFace = 0;
		this->currentLevel = 0;
		this->updateSubimages({UpdateTarget::Faces, UpdateTarget::Levels });
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel, this->currentDepth);
	}

	void TexToolkitTextureView::faceSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->faceScroller, event.entry);
		this->currentType = SubTexture::Type::Face;
		this->currentFace = event.entry->getTexture()->getFace();
		this->currentLevel = 0;
		this->updateSubimages({ UpdateTarget::Levels });
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel, this->currentDepth);
	}

	void TexToolkitTextureView::levelSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->levelScroller, event.entry);
		this->currentType = SubTexture::Type::Level;
		this->currentLevel = event.entry->getTexture()->getLevel();
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel, this->currentDepth);
	}

	void TexToolkitTextureView::importRequested(TexToolkitSubimageEvent& event)
	{
		auto texture = event.entry->getTexture();		
		this->importImage(texture->getType(), texture->getLayer(), texture->getFace(), texture->getLevel());
	}

	void TexToolkitTextureView::displayModeUpdateButtonClicked(wxCommandEvent& event)
	{
		this->updateDisplayModes();
	}

	void TexToolkitTextureView::displayModeSelected(wxCommandEvent& event)
	{
		this->update3DView();
		this->setupUserProperties();
	}

	void TexToolkitTextureView::propertyChanged(wxPropertyGridEvent& event)
	{
		const std::string propname = event.m_propertyName.ToStdString();
		if (this->isUserProperty(propname))
		{
			auto prop = createRenderProperty(event.GetValue());
			this->canvas->setUserProperty(propname, prop);
		}
		else if (propname == propDisplayWrapS)
			this->canvas->setWrappingS(static_cast<renderer::Wrapping>(event.GetValue().GetInteger()));
		else if (propname == propDisplayWrapT)
			this->canvas->setWrappingT(static_cast<renderer::Wrapping>(event.GetValue().GetInteger()));
		else if (propname == propDisplayFilterMin)
			this->canvas->setFilterMin(static_cast<renderer::FilteringMin>(event.GetValue().GetInteger()));
		else if (propname == propDisplayFilterMag)
			this->canvas->setFilterMag(static_cast<renderer::FilteringMag>(event.GetValue().GetInteger()));
		else if (propname == propDisplayWireframe)
			this->canvas->setShowWireframe(event.GetValue().GetBool());
		else if (
			propname == propCubeAlignment0 ||
			propname == propCubeAlignment1 ||
			propname == propCubeAlignment2 ||
			propname == propCubeAlignment3 ||
			propname == propCubeAlignment4 ||
			propname == propCubeAlignment5
		)
		{
			this->fixAlignments(propname);
			this->canvas->setCubeAlignment({
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)),
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)),
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)),
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)),
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)),
				getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5))
				});
		}

		this->canvas->Refresh();
	}

	void TexToolkitTextureView::selectBaseClicked(wxHyperlinkEvent& event)
	{
		this->selectDepth(0);
		if (auto layer = this->getLayer(0))
			layer->select(false);
		if (auto face = this->getFace(0))
			face->select(false);
		if (auto level = this->getLevel(0))
			level->select(false);
		this->refreshView();
	}

	void TexToolkitTextureView::depthSliderChanged(wxCommandEvent& event)
	{
		this->depthSpin->SetValue(this->depthSlider->GetValue());
		this->currentDepth = this->depthSlider->GetValue();
		if (auto level = this->getLevel(0))
			level->select();
		this->refreshView();
	}

	void TexToolkitTextureView::depthSpinChanged(wxSpinEvent& event)
	{
		this->depthSlider->SetValue(this->depthSpin->GetValue());
		this->currentDepth = this->depthSpin->GetValue();
		if (auto level = this->getLevel(0))
			level->select();
		this->refreshView();
	}

	void TexToolkitTextureView::fixAlignments(const wxString& propname)
	{
		auto currentAlignment = getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propname));

		std::unordered_map<std::string, Image::CubeFace> properties{
			{ propCubeAlignment0, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)) },
			{ propCubeAlignment1, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)) },
			{ propCubeAlignment2, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)) },
			{ propCubeAlignment3, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)) },
			{ propCubeAlignment4, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)) },
			{ propCubeAlignment5, getProperty<Image::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5)) }
		};

		std::unordered_set<Image::CubeFace> alignments{
			Image::CubeFace::PositiveX,
			Image::CubeFace::NegativeX,
			Image::CubeFace::PositiveY,
			Image::CubeFace::NegativeY,
			Image::CubeFace::PositiveZ,
			Image::CubeFace::NegativeZ,
		};

		std::string propToChange;

		for (auto prop : properties)
		{
			if (currentAlignment == prop.second && prop.first != propname)
				propToChange = prop.first;

			alignments.erase(prop.second);
		}

		assert(alignments.size() == 1);

		auto enumprop = static_cast<wxEnumProperty*>(this->propertyGrid->GetProperty(propToChange));
		auto choiceidx = enumprop->GetChoices().Index(static_cast<int>(*alignments.begin()));
		enumprop->SetChoiceSelection(choiceidx);
		//this->propertyGrid->GetProperty(propToChange)->setval
		//this->propertyGrid->GetProperty(propToChange)->SetValue()
	}
}