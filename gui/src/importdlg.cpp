#include "gui/importdlg.hpp"

namespace textoolkit
{
    InterpolationMinMag ImportDlg::interpolationChoice;

    ImportDlg::ImportDlg()
        : wxFileDialog()
    {
        this->enableCustomization();
    }

    ImportDlg::ImportDlg(wxWindow* parent,
        const wxString& message,
        const wxString& defaultDir,
        const wxString& defaultFile,
        const wxString& wildCard,
        long style,
        const wxPoint& pos,
        const wxSize& sz,
        const wxString& name)
        : wxFileDialog(parent, message, defaultDir, defaultFile, wildCard, style, pos, sz, name)
    {
        this->enableCustomization();
    }

    InterpolationMinMag ImportDlg::getInterpolation()
    {
        if (this->usingHook)
        {
            const auto& importHook = static_cast<const ImportDlgCustomizeHook&>(this->hook);
            return InterpolationMinMag(importHook.getMinInterpolation(), importHook.getMagInterpolation());
        }

        return interpolationChoice;
    }

    void ImportDlg::enableCustomization()
    {
        this->usingHook = true;
        if (!this->SetCustomizeHook(this->hook))
        {
            this->usingHook = false;
            this->SetExtraControlCreator(&importDlgCustomizationFnc);
        }
    }

    wxWindow* ImportDlg::importDlgCustomizationFnc(wxWindow* wnd)
    {
        auto interpolationMap = getInterpolationMap();

        wxPanel* panel = new wxPanel(wnd, wxID_ANY);

        wxPanel* minPanel = new wxPanel(panel, wxID_ANY);
        wxStaticText* minInterpolationLabel = new wxStaticText(minPanel, wxID_ANY, "Minifying interpolation");
        wxChoice* minInterpolationChoice = new wxChoice(minPanel, wxID_ANY);
        minInterpolationChoice->Bind(wxEVT_CHOICE, [&interpolationMap](wxCommandEvent& event) {
            interpolationChoice.minInterpolation = static_cast<Interpolation>(reinterpret_cast<size_t>(event.GetClientData()));
        });
        minInterpolationChoice->SetParent(minPanel);
        wxBoxSizer* minSizer = new wxBoxSizer(wxHORIZONTAL);
        minSizer->Add(minInterpolationLabel, 0, wxALL, 5);
        minSizer->Add(minInterpolationChoice, 0, wxALL, 5);
        minPanel->SetSizerAndFit(minSizer);

        wxPanel* magPanel = new wxPanel(panel, wxID_ANY);
        wxStaticText* magInterpolationLabel = new wxStaticText(magPanel, wxID_ANY, "Magnifying interpolation");
        wxChoice* magInterpolationChoice = new wxChoice(magPanel, wxID_ANY);
        magInterpolationChoice->Bind(wxEVT_CHOICE, [&interpolationMap](wxCommandEvent& event) {
            interpolationChoice.magInterpolation = static_cast<Interpolation>(reinterpret_cast<size_t>(event.GetClientData()));
        });
        magInterpolationChoice->SetParent(magPanel);
        wxBoxSizer* magSizer = new wxBoxSizer(wxHORIZONTAL);
        magSizer->Add(magInterpolationLabel, 0, wxALL, 5);
        magSizer->Add(magInterpolationChoice, 0, wxALL, 5);
        magPanel->SetSizerAndFit(magSizer);

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(minPanel, 0, wxALL, 0);
        sizer->Add(magPanel, 0, wxALL, 0);
        panel->SetSizerAndFit(sizer);

        for (auto [e, s] : interpolationMap)
        {
            int minI = minInterpolationChoice->Append(s, reinterpret_cast<void*>(e));
            int magI = magInterpolationChoice->Append(s, reinterpret_cast<void*>(e));

            if (e == Interpolation::Bicubic)
                minInterpolationChoice->SetSelection(minI);
            else if (e == Interpolation::NearestNeighbors)
                magInterpolationChoice->SetSelection(magI);
        }

        return panel;
    }

    void ImportDlg::ImportDlgCustomizeHook::AddCustomControls(wxFileDialogCustomize& customizer)
    {
        auto interpolationMap = getInterpolationMap();

        this->interpolations.clear();
        unsigned int i = 0;
        unsigned int defaultMin = 0;
        unsigned int defaultMag = 0;
        for (auto el : interpolationMap)
        {
            this->interpolations.push_back(el.second);
            if (el.first == Interpolation::Bicubic)
                defaultMin = i;
            else if (el.first == Interpolation::NearestNeighbors)
                defaultMag = i;
            i++;
        }

        customizer.AddStaticText("Minifying interpolation");
        this->minInterpolationChoice = customizer.AddChoice(interpolations.size(), interpolations.data());
        this->minInterpolationChoice->SetSelection(defaultMin);
        customizer.AddStaticText("Magnifying interpolation");
        this->magInterpolationChoice = customizer.AddChoice(interpolations.size(), interpolations.data());
        this->magInterpolationChoice->SetSelection(defaultMag);
    }

    void ImportDlg::ImportDlgCustomizeHook::TransferDataFromCustomControls()
    {
        auto interpolationMap = getInterpolationMapStr();
        auto minInterpolationStr = this->interpolations[this->minInterpolationChoice->GetSelection()];
        auto magInterpolationStr = this->interpolations[this->magInterpolationChoice->GetSelection()];
        this->minInterpolation = interpolationMap.at(minInterpolationStr.ToStdString());
        this->magInterpolation = interpolationMap.at(magInterpolationStr.ToStdString());
    }

    Interpolation ImportDlg::ImportDlgCustomizeHook::getMinInterpolation() const
    {
        return this->minInterpolation;
    }

    Interpolation ImportDlg::ImportDlgCustomizeHook::getMagInterpolation() const
    {
        return this->magInterpolation;
    }
}