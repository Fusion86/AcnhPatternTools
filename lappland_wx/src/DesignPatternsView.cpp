#include "DesignPatternsView.hpp"

DesignPatternsView::DesignPatternsView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* lblDesignPatterns = new wxStaticText(this, wxID_ANY, "Design Patterns");
    wxStaticText* lblProDesignPatterns = new wxStaticText(this, wxID_ANY, "Pro Design Patterns");

    lstDesignPatterns = new wxListBox(this, ID_lstDesignPatterns, wxDefaultPosition, wxDefaultSize);
    lstProDesignPatterns =
        new wxListBox(this, ID_lstProDesignPatterns, wxDefaultPosition, wxDefaultSize);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(1, wxSize(10, 5));
    fgs->Add(lblDesignPatterns);
    fgs->Add(lstDesignPatterns, 1, wxEXPAND);
    fgs->Add(lblProDesignPatterns);
    fgs->Add(lstProDesignPatterns, 1, wxEXPAND);

    fgs->AddGrowableRow(1);
    fgs->AddGrowableRow(3);
    hbox->Add(fgs, 1, wxEXPAND | wxALL, 10);

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
        case ID_lstDesignPatterns:
            break;

        case ID_lstProDesignPatterns:
            break;

        default:
            // throw error, this shouldn't happen!
            break;
    }
}
