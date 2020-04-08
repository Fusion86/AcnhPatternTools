#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* lblCharacter = new wxStaticText(this, wxID_ANY, "Character");
    wxStaticText* lblIsland = new wxStaticText(this, wxID_ANY, "Island");

    txtCharacter = new wxTextCtrl(this, wxID_ANY);
    txtIsland = new wxTextCtrl(this, wxID_ANY);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2, wxSize(10, 5));
    fgs->Add(lblCharacter, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtCharacter, 1, wxEXPAND);
    fgs->Add(lblIsland, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtIsland, 1, wxEXPAND);

    fgs->AddGrowableCol(1);
    hbox->Add(fgs, 1, wxEXPAND | wxALL, 10);
    SetSizer(hbox);
}

void GeneralInfoView::OnDataChanged(wxCommandEvent& event) {
    txtCharacter->ChangeValue(AppState->savedata->main.character.string.str());
    txtIsland->ChangeValue(AppState->savedata->main.island.string.str());
}

void GeneralInfoView::OnCharacterNameChanged(wxCommandEvent& event) {
    std::cout << "Yeet" << std::endl;
}

// clang-format off
wxBEGIN_EVENT_TABLE(GeneralInfoView, wxWindow)
    EVT_COMMAND(wxID_ANY, EVT_DATA_CHANGED, GeneralInfoView::OnDataChanged)
    EVT_TEXT(ID_txtCharacter, GeneralInfoView::OnCharacterNameChanged)
wxEND_EVENT_TABLE();
// clang-format on
