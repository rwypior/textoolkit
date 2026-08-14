#include "gui/textoolkittextureview.hpp"
#include "gui/textoolkitsubimageentry.hpp"
#include "gui/textoolkitprogressdialog.hpp"
#include "gui/texture.hpp"
#include "gui/util.hpp"

namespace textoolkit
{
	TexToolkitTextureView::TexToolkitTextureView(wxWindow* parent, std::unique_ptr<Texture>&& texture)
		: TextureView(parent)
		, texture(std::move(texture))
		, progressNotifier(progressNotifier)
	{
		this->flatView->SetScaleMode(wxStaticBitmapBase::ScaleMode::Scale_AspectFit);
		this->m_notebook2->ChangeSelection(1);

		this->updateFlatView();
		this->updateSubimages();
	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;

	std::vector<std::unique_ptr<SubTexture>> TexToolkitTextureView::createLayers(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int layers = image.getLayers();
		FiniteThreadpool threadpool(progressNotifier);
		std::vector<std::unique_ptr<SubTexture>> subtextures;
		for (unsigned int layer = 0; layer < layers; layer++)
		{
			threadpool.enqueue([this, layer, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<SubTexture>(SubTexture::createLayer(*this->texture, layer));
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

	std::vector<std::unique_ptr<SubTexture>> TexToolkitTextureView::createFaces(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int faces = image.getFaces();
		FiniteThreadpool threadpool(progressNotifier);
		std::vector<std::unique_ptr<SubTexture>> subtextures;
		for (unsigned int face = 0; face < faces; face++)
		{
			threadpool.enqueue([this, face, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<SubTexture>(SubTexture::createFace(*this->texture, this->currentLayer, face));
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

	std::vector<std::unique_ptr<SubTexture>> TexToolkitTextureView::createLevels(ProgressNotifier progressNotifier) const
	{
		auto& image = this->texture->getImage();
		unsigned int levels = image.getLevels();
		FiniteThreadpool threadpool(progressNotifier);
		std::vector<std::unique_ptr<SubTexture>> subtextures;
		for (unsigned int level = 0; level < levels; level++)
		{
			threadpool.enqueue([this, level, &subtextures, &threadpool]() {
				auto subtexture = std::make_unique<SubTexture>(SubTexture::createLevel(*this->texture, this->currentLayer, this->currentFace, level));
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

	void TexToolkitTextureView::updateFlatView(unsigned int layer, unsigned int face, unsigned int level)
	{
		if (!this->texture)
			return;

		auto& image = this->texture->getImage();
		this->mainTexture = SubTexture::createLevel(*this->texture, this->currentLayer, this->currentFace, level);
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
		this->layerScroller->Freeze();

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
		}

		this->layerScroller->Layout();
		this->layerScroller->Thaw();
	}

	void TexToolkitTextureView::updateFaces(SubTextureContainer* subtextures)
	{
		this->faceScroller->Freeze();

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
		}

		this->faceScroller->Layout();
		this->faceScroller->Thaw();
	}

	void TexToolkitTextureView::updateLevels(SubTextureContainer* subtextures)
	{
		this->levelScroller->Freeze();

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
		}

		this->levelScroller->Layout();
		this->levelScroller->Thaw();
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
}