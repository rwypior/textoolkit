#ifndef _h_textoolkit_gui_textureview
#define _h_textoolkit_gui_textureview

#include "mainwindow.h"
#include "textoolkitsubimageentry.hpp"
#include "common/threadpool.hpp"
#include "common/bitset.hpp"

#include <wx/bitmap.h>

#include <memory>
#include <vector>

namespace textoolkit
{
	class Texture;
	class SubTexture;

	namespace renderer
	{
		class ModelDatabase;
		class Object;
	}

	class TexToolkitTextureView : public TextureView
	{
	public:
		static constexpr char mainObjectName[] = "mainobject";

		using SubTextureContainer = std::vector<std::unique_ptr<SubTexture>>;

		enum class UpdateTarget
		{
			Layers,
			Faces,
			Levels
		};
		using UpdateTargets = EnumBitset<UpdateTarget, 3>;

	public:
		TexToolkitTextureView(std::unique_ptr<Texture>&& texture, renderer::ModelDatabase& modelDatabase, wxWindow* parent);
		~TexToolkitTextureView();

		SubTextureContainer createLayers(ProgressNotifier progressNotifier = {}) const;
		SubTextureContainer createFaces(ProgressNotifier progressNotifier = {}) const;
		SubTextureContainer createLevels(ProgressNotifier progressNotifier = {}) const;

		void updateFlatView(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);
		void updateSubimages(UpdateTargets targets = UpdateTargets::default(true));
		void updateLayers(SubTextureContainer* subtextures = nullptr);
		void updateFaces(SubTextureContainer* subtextures = nullptr);
		void updateLevels(SubTextureContainer* subtextures = nullptr);

		void update3DView();
		void updateModels();
		void updateModelList();

	private:
		void deselectOthers(wxScrolledWindow* scroller, TexToolkitSubimageEntry* entry);

		void layerSelected(TexToolkitSubimageEvent& event);
		void faceSelected(TexToolkitSubimageEvent& event);
		void levelSelected(TexToolkitSubimageEvent& event);
		void modelUpdateButtonClicked(wxCommandEvent& event);
		void modelSelected(wxCommandEvent& event);

		std::unique_ptr<Texture> texture;
		SubTexture mainTexture;
		wxBitmap flatViewBitmap;
		wxBitmap editorBitmap;
		ProgressNotifier progressNotifier;
		renderer::ModelDatabase& modelDatabase;

		std::unique_ptr<renderer::Object> object;

		unsigned int currentLayer = 0;
		unsigned int currentFace = 0;
		unsigned int currentLevel = 0;
	};
}

#endif
