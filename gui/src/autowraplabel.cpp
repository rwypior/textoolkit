#include "gui/autowraplabel.hpp"

#include <wx/textwrapper.h>

namespace textoolkit
{
    // Wrapper

    class Wrapper : public wxTextWrapper
    {
    public:
        Wrapper(wxWindow* win, const wxString& text, int widthMax)
        {
            Wrap(win, text, widthMax);
        }

        const std::vector<wxString>& getLines() const
        {
            return this->lines;
        }

        wxString getText() const
        {
            wxString text;
            for (wxString line : lines)
                text += line + "\n";
            return text;
        }

    protected:
        void OnOutputLine(const wxString& line) override
        {
            this->lines.push_back(line);
        }

    private:
        std::vector<wxString> lines;
    };

    // Label

    AutowrapLabel::AutowrapLabel(wxWindow* parent,
        wxWindowID id,
        const wxString& label,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name)
        : wxStaticText(parent, id, label, pos, size, style, name)
    {
        this->Bind(wxEVT_SIZE, &AutowrapLabel::eventSize, this);
    }

    void AutowrapLabel::SetLabel(const wxString& label)
    {
        this->isUpdatingText = true;

        wxWindow* parent = this->GetParent();
        if (parent && !this->isWrapping)
        {
            int width = parent->GetClientSize().GetWidth();

            Wrapper wrapper(this, label, width);
            wxStaticText::SetLabel(wrapper.getText());
        }
        else
            wxStaticText::SetLabel(label);

        if (!this->isWrapping)
        {
            this->originalText = label;
        }

        this->isUpdatingText = false;
    }

    void AutowrapLabel::wrapText()
    {
        wxWindow* parent = this->GetParent();
        if (!parent)
            return;

        int width = parent->GetClientSize().GetWidth();

        this->isWrapping = true;
        Wrapper wrapper(this, this->originalText, width);
        this->SetLabel(wrapper.getText());
        this->isWrapping = false;
    }

    void AutowrapLabel::eventSize(wxSizeEvent& event)
    {
        if (!this->autoWrap || this->isUpdatingText || this->isWrapping)
            return;

        this->wrapText();
    }
}