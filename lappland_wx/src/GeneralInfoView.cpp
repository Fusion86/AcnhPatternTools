#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 500)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* lblCharacter = new wxStaticText(panel, wxID_ANY, "Character");
    wxStaticText* lblIsland = new wxStaticText(panel, wxID_ANY, "Island");

    txtCharacter = new wxTextCtrl(panel, wxID_ANY);
    txtIsland = new wxTextCtrl(panel, wxID_ANY);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2, wxSize(10, 5));
    fgs->Add(lblCharacter, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtCharacter, 1, wxEXPAND);
    fgs->Add(lblIsland, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtIsland, 1, wxEXPAND);

    // Doesn't work :(
    // fgs->AddGrowableCol(1, 1);

    hbox->Add(fgs, 1, wxEXPAND | wxALL, 15);
    panel->SetSizerAndFit(hbox);
}

void GeneralInfoView::OnDataChanged(wxCommandEvent& event) {
    txtCharacter->ChangeValue(AppState->savedata->main.character.string.str());
    txtIsland->ChangeValue(AppState->savedata->main.island.string.str());
}

// clang-format off
wxBEGIN_EVENT_TABLE(GeneralInfoView, wxWindow)
    EVT_COMMAND(wxID_ANY, EVT_DATA_CHANGED, GeneralInfoView::OnDataChanged)
wxEND_EVENT_TABLE();
// clang-format on
