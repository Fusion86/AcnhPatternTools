#include "DesignPatternsView.hpp"

DesignPatternsView::DesignPatternsView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    //
    // Column 0 - Listboxes
    //

    wxStaticText* lblDesignPatterns = new wxStaticText(this, wxID_ANY, "Design Patterns");
    wxStaticText* lblProDesignPatterns = new wxStaticText(this, wxID_ANY, "Pro Design Patterns");

    lstDesignPatterns = new wxListBox(this, ID_lstDesignPatterns, wxDefaultPosition, wxDefaultSize);
    lstProDesignPatterns =
        new wxListBox(this, ID_lstProDesignPatterns, wxDefaultPosition, wxDefaultSize);

    wxFlexGridSizer* col0 = new wxFlexGridSizer(1, wxSize(10, 5));
    col0->Add(lblDesignPatterns);
    col0->Add(lstDesignPatterns, 1, wxEXPAND);
    col0->Add(lblProDesignPatterns);
    col0->Add(lstProDesignPatterns, 1, wxEXPAND);

    col0->AddGrowableRow(1);
    col0->AddGrowableRow(3);

    //
    // Column 1 - Info panel with name, creator, etc
    //

    wxStaticText* lblName = new wxStaticText(this, wxID_ANY, "Name");

    txtPatternName = new wxTextCtrl(this, ID_txtPatternName);

    wxFlexGridSizer* col1 = new wxFlexGridSizer(2, wxSize(10, 5));
    col1->Add(lblName, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternName, 1, wxEXPAND);

    col1->AddGrowableCol(1);

    //
    // Column 2- Pattern preview image
    //

    wxBitmap bmpPattern = wxBitmap();
    bmpPattern.Create(wxSize(320, 320));
    bmpPatternCtrl = new wxGenericStaticBitmap(this, wxID_ANY, bmpPattern);

    wxBoxSizer* col2 = new wxBoxSizer(wxHORIZONTAL);
    col2->Add(bmpPatternCtrl);

    //
    // Connect all the things
    //

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(col0, 0, wxEXPAND | wxALL, 10);
    hbox->Add(col1, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 10);
    hbox->Add(col2, 0, wxALL, 10);

    Bind(EVT_DATA_CHANGED, &DesignPatternsView::onDataChanged, this);
    Bind(wxEVT_LISTBOX, &DesignPatternsView::onSelectionChanged, this);

    SetSizer(hbox);
}

void DesignPatternsView::onDataChanged(wxCommandEvent& event) {
    wxArrayString patternNames;
    wxArrayString proPatternNames;

    designPatterns.clear();
    proDesignPatterns.clear();

    for (int i = 0; i < 50; i++) {
        designPatterns.push_back(DesignPatternProxy(&AppState->savedata->main.designPatterns[i]));
        proDesignPatterns.push_back(
            ProDesignPatternProxy(&AppState->savedata->main.proDesignPatterns[i]));

        patternNames.Add(designPatterns[i].getName());
        proPatternNames.Add(proDesignPatterns[i].getName());
    }

    lstDesignPatterns->Set(patternNames);
    lstProDesignPatterns->Set(patternNames);
}

void DesignPatternsView::onSelectionChanged(wxCommandEvent& event) {
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

            // selectedDesignPattern = &pattern;
            // selectedProDesignPattern = nullptr;

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

            // selectedDesignPattern = nullptr;
            // selectedProDesignPattern = &pattern;

            bmpPatternCtrl->SetBitmap(bmp);
            lstDesignPatterns->DeselectAll();
        } break;

        default:
            // throw error, this shouldn't happen!
            break;
    }
}
