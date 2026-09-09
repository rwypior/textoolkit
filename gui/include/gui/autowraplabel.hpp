#ifndef _h_textoolkit_gui_autowraplabel
#define _h_textoolkit_gui_autowraplabel

#include <wx/wx.h>

namespace textoolkit
{
    class AutowrapLabel : public wxStaticText
    {
    public:
        bool autoWrap = true;

        AutowrapLabel(wxWindow* parent,
            wxWindowID id,
            const wxString& label,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0,
            const wxString& name = wxASCII_STR(wxStaticTextNameStr));

        virtual void SetLabel(const wxString& label) override;

        void wrapText();

    private:
        void eventSize(wxSizeEvent& event);

    private:
        bool isWrapping = false;
        bool isUpdatingText = false;
        wxString originalText;
    };
}

#endif