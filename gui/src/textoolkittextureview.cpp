#include "gui/textoolkittextureview.hpp"
#include "gui/texture.hpp"

namespace textoolkit
{
	TexToolkitTextureView::TexToolkitTextureView(std::unique_ptr<Texture>&& texture, wxWindow* parent)
		: TextureView(parent)
		, texture(std::move(texture))
	{

	}

	TexToolkitTextureView::~TexToolkitTextureView() = default;
}