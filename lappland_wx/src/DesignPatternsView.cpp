#include "DesignPatternsView.hpp"

DesignPatternsView::DesignPatternsView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxStaticText* lblDesignPatterns = new wxStaticText(this, wxID_ANY, "Design Patterns");
    wxStaticText* lblProDesignPatterns = new wxStaticText(this, wxID_ANY, "Pro Design Patterns");

    lstDesignPatterns = new wxListBox(this, ID_lstDesignPatterns, wxDefaultPosition, wxDefaultSize);
    lstProDesignPatterns =
        new wxListBox(this, ID_lstProDesignPatterns, wxDefaultPosition, wxDefaultSize);

    wxBitmap bmpPattern = wxBitmap();
    bmpPattern.Create(wxSize(320, 320));
    bmpPatternCtrl =
        new wxGenericStaticBitmap(this, wxID_ANY, bmpPattern, wxDefaultPosition, wxSize(150, 150));

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* fgs = new wxFlexGridSizer(1, wxSize(10, 5));
    fgs->Add(lblDesignPatterns);
    fgs->Add(lstDesignPatterns, 1, wxEXPAND);
    fgs->Add(lblProDesignPatterns);
    fgs->Add(lstProDesignPatterns, 1, wxEXPAND);

    fgs->AddGrowableRow(1);
    fgs->AddGrowableRow(3);

    wxBoxSizer* imgCtrlBox = new wxBoxSizer(wxHORIZONTAL);
    imgCtrlBox->Add(bmpPatternCtrl);

    hbox->Add(fgs, 1, wxEXPAND | wxALL, 10);
    hbox->Add(imgCtrlBox, 0, wxALL, 10);

    Bind(EVT_DATA_CHANGED, &DesignPatternsView::OnDataChanged, this);
    Bind(wxEVT_LISTBOX, &DesignPatternsView::OnSelectionChanged, this);

    SetSizer(hbox);
}

void DesignPatternsView::OnDataChanged(wxCommandEvent& event) {
    wxArrayString patternNames;
    for (const auto& pattern : AppState->savedata->main.designPatterns) {
        patternNames.Add(pattern.name.str());
    }
    lstDesignPatterns->Set(patternNames);

    // I guess this could be more efficient by using a fixed size array, but for 50 items this
    // doesn't really matter.
    patternNames.Clear();
    for (const auto& pattern : AppState->savedata->main.proDesignPatterns) {
        patternNames.Add(pattern.name.str());
    }
    lstProDesignPatterns->Set(patternNames);
}

void DesignPatternsView::OnSelectionChanged(wxCommandEvent& event) {
    int idx = event.GetSelection();
    switch (event.GetId()) {
        case ID_lstDesignPatterns: {
            auto patternIdx = lstDesignPatterns->GetSelection();
            if (patternIdx == wxNOT_FOUND) break;

            auto pattern = AppState->savedata->main.designPatterns[patternIdx];
            auto rgb = pattern.getRgbData();
            auto alpha = pattern.getAlphaData();
            wxImage img = wxImage(32, 32, rgb.get(), alpha.get(), true);
            img.Rescale(320, 320);
            wxBitmap bmp = wxBitmap(img);

            bmpPatternCtrl->SetBitmap(bmp);
            lstProDesignPatterns->DeselectAll();
        } break;

        case ID_lstProDesignPatterns: {
            auto patternIdx = lstProDesignPatterns->GetSelection();
            if (patternIdx == wxNOT_FOUND) break;

            auto pattern = AppState->savedata->main.proDesignPatterns[patternIdx];
            auto rgb = pattern.getRgbData();
            auto alpha = pattern.getAlphaData();
            wxImage img = wxImage(64, 64, rgb.get(), alpha.get(), true);
            img.Rescale(320, 320);
            wxBitmap bmp = wxBitmap(img);

            bmpPatternCtrl->SetBitmap(bmp);
            lstDesignPatterns->DeselectAll();
        } break;

        default:
            // throw error, this shouldn't happen!
            break;
    }
}
