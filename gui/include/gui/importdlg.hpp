#ifndef _h_textoolkit_gui_importdlg
#define _h_textoolkit_gui_importdlg

#include "texture/texture.hpp"

#include <wx/wx.h>
#include <wx/filedlgcustomize.h>

namespace textoolkit
{
    class ImportDlg : public wxFileDialog
    {
    public:
        class ImportDlgCustomizeHook : public wxFileDialogCustomizeHook
        {
        public:
            void AddCustomControls(wxFileDialogCustomize& customizer) override;
            void TransferDataFromCustomControls() override;

            Interpolation getMinInterpolation() const;
            Interpolation getMagInterpolation() const;

        private:
            wxFileDialogChoice* minInterpolationChoice;
            wxFileDialogChoice* magInterpolationChoice;

            std::vector<wxString> interpolations;
            Interpolation minInterpolation;
            Interpolation magInterpolation;
        };

        ImportDlg();
        ImportDlg(wxWindow* parent,
            const wxString& message = wxASCII_STR(wxFileSelectorPromptStr),
            const wxString& defaultDir = wxEmptyString,
            const wxString& defaultFile = wxEmptyString,
            const wxString& wildCard = wxASCII_STR(wxFileSelectorDefaultWildcardStr),
            long style = wxFD_DEFAULT_STYLE,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& sz = wxDefaultSize,
            const wxString& name = wxASCII_STR(wxFileDialogNameStr));

        InterpolationMinMag getInterpolation();

    private:
        void enableCustomization();

        static wxWindow* importDlgCustomizationFnc(wxWindow* wnd);

        ImportDlgCustomizeHook hook;
        bool usingHook = false;

        /// Used only for SetExtraControlCreator approach
        static InterpolationMinMag interpolationChoice;
    };
}

#endif