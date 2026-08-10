#ifndef _h_textoolkit_gui_textureview
#define _h_textoolkit_gui_textureview

#include "mainwindow.h"

#include <memory>

namespace textoolkit
{
	class Texture;

	class TexToolkitTextureView : public TextureView
	{
	public:
		TexToolkitTextureView(std::unique_ptr<Texture>&& texture, wxWindow* parent);
		~TexToolkitTextureView();

	private:
		std::unique_ptr<Texture> texture;
	};
}

#endif
