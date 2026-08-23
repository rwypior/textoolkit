#include "gui/textoolkitaboutwindow.hpp"
#include "common/textoolkit.hpp"
#include "common/util.hpp"

namespace textoolkit
{
	TexToolkitAboutWindow::TexToolkitAboutWindow(wxWindow* parent)
		: AboutWindow(parent)
	{
		auto label = this->infoLabel->GetLabel();
		label.Replace("%ver.major%", STRINGIFY(TEXTOOLKIT_VERSION_MAJOR));
		label.Replace("%ver.minor%", STRINGIFY(TEXTOOLKIT_VERSION_MINOR));
		label.Replace("%ver.patch%", STRINGIFY(TEXTOOLKIT_VERSION_PATCH));
		label.Replace("%reldate%", TEXTOOLKIT_COMPILATION_DATE " " TEXTOOLKIT_COMPILATION_TIME);
		this->infoLabel->SetLabel(label);

		static constexpr char url[] = "https://github.com/rwypior/textoolkit";
		static constexpr char urltxt[] = "github.com/rwypior/textoolkit";
		this->githubLink->SetURL(url);
		this->githubLink->SetLabel(urltxt);

		this->Bind(wxEVT_BUTTON, &TexToolkitAboutWindow::eventOk, this);
	}

	void TexToolkitAboutWindow::eventOk(wxCommandEvent& event)
	{
		this->Close();
	}
}