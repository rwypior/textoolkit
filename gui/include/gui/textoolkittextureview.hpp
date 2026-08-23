#ifndef _h_textoolkit_gui_textureview
#define _h_textoolkit_gui_textureview

#include "mainwindow.h"
#include "textoolkitsubimageentry.hpp"
#include "common/threadpool.hpp"
#include "common/bitset.hpp"
#include "renderer/displaymode.hpp"

#include <wx/bitmap.h>

#include <memory>
#include <vector>
#include <map>

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

		static constexpr char propGrp3DDisplaySettings[] = "grp3ddisplaysettings";
		static constexpr char propDisplayWrapS[] = "displaywraps";
		static constexpr char propDisplayWrapT[] = "displaywrapt";
		static constexpr char propDisplayFilterMin[] = "displayfiltermin";
		static constexpr char propDisplayFilterMag[] = "displayfiltermag";
		static constexpr char propDisplayWireframe[] = "displaywireframe";
		static constexpr char propCubeAlignment0[] = "cubeface0";
		static constexpr char propCubeAlignment1[] = "cubeface1";
		static constexpr char propCubeAlignment2[] = "cubeface2";
		static constexpr char propCubeAlignment3[] = "cubeface3";
		static constexpr char propCubeAlignment4[] = "cubeface4";
		static constexpr char propCubeAlignment5[] = "cubeface5";

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

		TexToolkitSubimageEntry* getLayer(unsigned int layer);
		TexToolkitSubimageEntry* getFace(unsigned int face);
		TexToolkitSubimageEntry* getLevel(unsigned int level);

		void updateFlatView(unsigned int layer = 0, unsigned int face = 0, unsigned int level = 0);
		void updateSubimages(UpdateTargets targets = UpdateTargets::default(true));
		void updateLayers(SubTextureContainer* subtextures = nullptr);
		void updateFaces(SubTextureContainer* subtextures = nullptr);
		void updateLevels(SubTextureContainer* subtextures = nullptr);

		void setupProperties();
		void update3DView();
		void updateModels();
		void updateDisplayModes();
		void updateDisplayModeList();

		void importLayer(Texture& texture, unsigned int layer);
		void importFace(Texture& texture, unsigned int layer, unsigned int face);
		void importLevel(Texture& texture, unsigned int layer, unsigned int face, unsigned int level);

	private:
		void deselectOthers(wxScrolledWindow* scroller, TexToolkitSubimageEntry* entry);

		void layerSelected(TexToolkitSubimageEvent& event);
		void faceSelected(TexToolkitSubimageEvent& event);
		void levelSelected(TexToolkitSubimageEvent& event);
		void importRequested(TexToolkitSubimageEvent& event);
		void displayModeUpdateButtonClicked(wxCommandEvent& event);
		void displayModeSelected(wxCommandEvent& event);
		void propertyChanged(wxPropertyGridEvent& event);

		void fixAlignments(const wxString& propname);

		std::unique_ptr<Texture> texture;
		SubTexture mainTexture;
		wxBitmap flatViewBitmap;
		wxBitmap editorBitmap;
		ProgressNotifier progressNotifier;
		renderer::ModelDatabase& modelDatabase;
		std::map<std::string, renderer::DisplayMode> displayModes;

		renderer::Object* object = nullptr;

		unsigned int currentLayer = 0;
		unsigned int currentFace = 0;
		unsigned int currentLevel = 0;
	};
}

#endif
