#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitsubimageentry.hpp"
#include "gui/textoolkitprogressdialog.hpp"
#include "gui/texture.hpp"
#include "gui/util.hpp"
#include "gui/textureloader.hpp"
#include "renderer/modeldatabase.hpp"
#include "renderer/model.hpp"
#include "renderer/object.hpp"

#include <wx/stdpaths.h>

#include <unordered_set>

namespace
{
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

		this->m_notebook2->DeletePage(2); // TODO - remove this when editor is ready
		
		this->setupProperties();
		this->updateModels();
		this->updateDisplayModes();

		this->updateFlatView();
		this->updateSubimages();
		this->update3DView();

		this->refreshDisplayModeListButton->Bind(wxEVT_BUTTON, &TexToolkitTextureView::displayModeUpdateButtonClicked, this);
		this->displaymode->Bind(wxEVT_COMBOBOX, &TexToolkitTextureView::displayModeSelected, this);
		this->propertyGrid->Bind(wxEVT_PG_CHANGED, &TexToolkitTextureView::propertyChanged, this);
	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;

	GuiTexture& TexToolkitTextureView::getTexture()
	{
		return *this->texture;
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
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createLayer(*this->texture, layer));
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
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createFace(*this->texture, this->currentLayer, face));
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
		unsigned int levels = image.getLevels();
		FiniteThreadpool threadpool(progressNotifier);
		TexToolkitTextureView::SubTextureContainer subtextures;
		for (unsigned int level = 0; level < levels; level++)
		{
			threadpool.enqueue([this, level, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<GuiSubTexture>(GuiSubTexture::createLevel(*this->texture, this->currentLayer, this->currentFace, level));
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

	void TexToolkitTextureView::updateFlatView(unsigned int layer, unsigned int face, unsigned int level)
	{
		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		this->mainTexture = GuiSubTexture::createLevel(*this->texture, this->currentLayer, this->currentFace, level);
		this->flatViewBitmap = this->mainTexture.getBitmap();

		this->flatViewImageDetails->SetLabel(
			wxString::Format("Layer %d; Face %d; Level %d; %dx%d",
				this->mainTexture.getLayer(),
				this->mainTexture.getFace(),
				this->mainTexture.getLevel(),
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

		this->propertyGrid->Append(new wxPropertyCategory("3D display settings", "grp3ddisplaysettings"));
		this->propertyGrid->Append(new wxPropertyCategory("3D display settings", propGrp3DDisplaySettings));
		this->propertyGrid->Append(new wxEnumProperty("Wrap S", propDisplayWrapS, wrappingChoices));
		this->propertyGrid->Append(new wxEnumProperty("Wrap T", propDisplayWrapT, wrappingChoices));
		this->propertyGrid->Append(new wxEnumProperty("Filtering min", propDisplayFilterMin, filterMinChoices));
		this->propertyGrid->Append(new wxEnumProperty("Filtering mag", propDisplayFilterMag, filterMagChoices));
		this->propertyGrid->Append(new wxBoolProperty("Show wireframe", propDisplayWireframe));

		wxPGChoices alignmentChoices;
		alignmentChoices.Add("Positive X", static_cast<int>(renderer::CubeFace::PositiveX));
		alignmentChoices.Add("Negative X", static_cast<int>(renderer::CubeFace::NegativeX));
		alignmentChoices.Add("Positive Y", static_cast<int>(renderer::CubeFace::PositiveY));
		alignmentChoices.Add("Negative Y", static_cast<int>(renderer::CubeFace::NegativeY));
		alignmentChoices.Add("Positive Z", static_cast<int>(renderer::CubeFace::PositiveZ));
		alignmentChoices.Add("Negative Z", static_cast<int>(renderer::CubeFace::NegativeZ));

		this->propertyGrid->Append(new wxPropertyCategory("Cubemap alignment", "grp3dcubemapalignment"));
		this->propertyGrid->Append(new wxEnumProperty("Face 0", propCubeAlignment0, alignmentChoices, 0));
		this->propertyGrid->Append(new wxEnumProperty("Face 1", propCubeAlignment1, alignmentChoices, 1));
		this->propertyGrid->Append(new wxEnumProperty("Face 2", propCubeAlignment2, alignmentChoices, 2));
		this->propertyGrid->Append(new wxEnumProperty("Face 3", propCubeAlignment3, alignmentChoices, 3));
		this->propertyGrid->Append(new wxEnumProperty("Face 4", propCubeAlignment4, alignmentChoices, 4));
		this->propertyGrid->Append(new wxEnumProperty("Face 5", propCubeAlignment5, alignmentChoices, 5));

		this->propertyGrid->Append(new wxPropertyCategory("Texture properties", "grp3ddisplaysettings"));

		this->canvas->setWrappingS(getProperty<renderer::Wrapping>(this->propertyGrid->GetProperty(propDisplayWrapS)));
		this->canvas->setWrappingT(getProperty<renderer::Wrapping>(this->propertyGrid->GetProperty(propDisplayWrapT)));
		this->canvas->setFilterMin(getProperty<renderer::FilteringMin>(this->propertyGrid->GetProperty(propDisplayFilterMin)));
		this->canvas->setFilterMag(getProperty<renderer::FilteringMag>(this->propertyGrid->GetProperty(propDisplayFilterMag)));
		this->canvas->setShowWireframe(getBoolProperty(this->propertyGrid->GetProperty(propDisplayWireframe)));
		this->canvas->setCubeAlignment({ 
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)),
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)),
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)),
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)),
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)),
			getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5))
		});
	}

	void TexToolkitTextureView::update3DView()
	{
		unsigned int modelSelection = this->displaymode->GetSelection();
		if (modelSelection == wxNOT_FOUND)
			return;
		auto displayMode = reinterpret_cast<renderer::DisplayMode*>(this->displaymode->GetClientData(modelSelection));
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

	void TexToolkitTextureView::importLayer(GuiTexture& texture, unsigned int layer)
	{
		auto subentry = this->getLayer(layer);
		if (!subentry)
			return;

		for (unsigned int face = 0; face < this->texture->getImage().getFaces(); face++)
		{
			for (unsigned int level = 0; level < this->texture->getImage().getLevels(); level++)
			{
				auto source = GuiSubTexture::createLevel(texture, 0, 0, 0);
				auto destination = GuiSubTexture::createLevel(*this->texture, layer, face, level);
				destination.set(source);

				if (auto subentry = this->getLevel(level))
					subentry->updatePreview();
			}

			if (auto subentry = this->getFace(face))
				subentry->updatePreview();
		}

		if (auto subentry = this->getLayer(layer))
			subentry->updatePreview();

		this->canvas->reuploadTexture();
	}

	void TexToolkitTextureView::importFace(GuiTexture& texture, unsigned int layer, unsigned int face)
	{
		for (unsigned int level = 0; level < this->texture->getImage().getLevels(); level++)
		{
			auto source = GuiSubTexture::createLevel(texture, 0, 0, 0);
			auto destination = GuiSubTexture::createLevel(*this->texture, layer, face, level);
			destination.set(source);

			if (auto subentry = this->getLevel(level))
				subentry->updatePreview();
		}

		if (auto subentry = this->getFace(face))
			subentry->updatePreview();

		this->canvas->reuploadTexture();
	}

	void TexToolkitTextureView::importLevel(GuiTexture& texture, unsigned int layer, unsigned int face, unsigned int level)
	{
		auto source = GuiSubTexture::createLevel(texture, 0, 0, 0);
		auto destination = GuiSubTexture::createLevel(*this->texture, layer, face, level);
		destination.set(source);

		if (auto subentry = this->getLevel(level))
			subentry->setTexture(std::make_unique<GuiSubTexture>(std::move(destination)));

		this->canvas->reuploadTexture();
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

	void TexToolkitTextureView::layerSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->layerScroller, event.entry);
		this->currentLayer = event.entry->getTexture()->getLayer();
		this->currentFace = 0;
		this->currentLevel = 0;
		this->updateSubimages({UpdateTarget::Faces, UpdateTarget::Levels });
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel);
	}

	void TexToolkitTextureView::faceSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->faceScroller, event.entry);
		this->currentFace = event.entry->getTexture()->getFace();
		this->currentLevel = 0;
		this->updateSubimages({ UpdateTarget::Levels });
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel);
	}

	void TexToolkitTextureView::levelSelected(TexToolkitSubimageEvent& event)
	{
		this->deselectOthers(this->levelScroller, event.entry);
		this->currentLevel = event.entry->getTexture()->getLevel();
		this->updateFlatView(this->currentLayer, this->currentFace, this->currentLevel);
	}

	void TexToolkitTextureView::importRequested(TexToolkitSubimageEvent& event)
	{
		TextureLoader loader;

		const auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);
		std::string wildcard = loader.getWildcardString();

		wxFileDialog dlg(this, "Open image", picturesDir, wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		dlg.SetFilterIndex(loader.getFilterIndexAll());

		auto res = dlg.ShowModal();
		if (res == wxID_CANCEL)
			return;

		auto tex = GuiTexture(std::move(*loader.loadTexture(dlg.GetPath().ToStdString())));

		auto textureType = event.entry->getTexture()->getType();
		switch (textureType)
		{
		case GuiSubTexture::Type::Layer:
			this->importLayer(tex, event.entry->getTexture()->getLayer());
			break;
		case GuiSubTexture::Type::Face:
			this->importFace(tex, event.entry->getTexture()->getLayer(), event.entry->getTexture()->getFace());
			break;
		case GuiSubTexture::Type::Level:
			this->importLevel(tex, event.entry->getTexture()->getLayer(), event.entry->getTexture()->getFace(), event.entry->getTexture()->getLevel());
			break;
		}
	}

	void TexToolkitTextureView::displayModeUpdateButtonClicked(wxCommandEvent& event)
	{
		this->updateDisplayModes();
	}

	void TexToolkitTextureView::displayModeSelected(wxCommandEvent& event)
	{
		this->update3DView();
	}

	void TexToolkitTextureView::propertyChanged(wxPropertyGridEvent& event)
	{
		if (event.m_propertyName == propDisplayWrapS)
			this->canvas->setWrappingS(static_cast<renderer::Wrapping>(event.GetValue().GetInteger()));
		else if (event.m_propertyName == propDisplayWrapT)
			this->canvas->setWrappingT(static_cast<renderer::Wrapping>(event.GetValue().GetInteger()));
		else if (event.m_propertyName == propDisplayFilterMin)
			this->canvas->setFilterMin(static_cast<renderer::FilteringMin>(event.GetValue().GetInteger()));
		else if (event.m_propertyName == propDisplayFilterMag)
			this->canvas->setFilterMag(static_cast<renderer::FilteringMag>(event.GetValue().GetInteger()));
		else if (event.m_propertyName == propDisplayWireframe)
			this->canvas->setShowWireframe(event.GetValue().GetBool());
		else if (
			event.m_propertyName == propCubeAlignment0 ||
			event.m_propertyName == propCubeAlignment1 ||
			event.m_propertyName == propCubeAlignment2 ||
			event.m_propertyName == propCubeAlignment3 ||
			event.m_propertyName == propCubeAlignment4 ||
			event.m_propertyName == propCubeAlignment5
		)
		{
			this->fixAlignments(event.m_propertyName);
			this->canvas->setCubeAlignment({
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)),
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)),
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)),
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)),
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)),
				getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5))
				});
		}
	}

	void TexToolkitTextureView::fixAlignments(const wxString& propname)
	{
		auto currentAlignment = getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propname));

		std::unordered_map<std::string, renderer::CubeFace> properties{
			{ propCubeAlignment0, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment0)) },
			{ propCubeAlignment1, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment1)) },
			{ propCubeAlignment2, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment2)) },
			{ propCubeAlignment3, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment3)) },
			{ propCubeAlignment4, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment4)) },
			{ propCubeAlignment5, getProperty<renderer::CubeFace>(this->propertyGrid->GetProperty(propCubeAlignment5)) }
		};

		std::unordered_set<renderer::CubeFace> alignments{
			renderer::CubeFace::PositiveX,
			renderer::CubeFace::NegativeX,
			renderer::CubeFace::PositiveY,
			renderer::CubeFace::NegativeY,
			renderer::CubeFace::PositiveZ,
			renderer::CubeFace::NegativeZ,
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