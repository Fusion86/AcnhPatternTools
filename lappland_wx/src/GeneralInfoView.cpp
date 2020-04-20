#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxStaticText* lblCharacter = new wxStaticText(this, wxID_ANY, "Character");
    wxStaticText* lblIsland = new wxStaticText(this, wxID_ANY, "Island");

    txtCharacter = new wxTextCtrl(this, ID_txtCharacter);
    txtIsland = new wxTextCtrl(this, wxID_ANY);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2, wxSize(10, 5));
    fgs->Add(lblCharacter, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtCharacter, 1, wxEXPAND);
    fgs->Add(lblIsland, 0, wxALIGN_CENTER_VERTICAL);
    fgs->Add(txtIsland, 1, wxEXPAND);

    fgs->AddGrowableCol(1);

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(fgs, 1, wxEXPAND | wxALL, 10);
    SetSizer(hbox);

    Bind(EVT_DATA_CHANGED, &GeneralInfoView::onDataChanged, this);
    Bind(wxEVT_TEXT, &GeneralInfoView::onCharacterNameChanged, this, ID_txtCharacter);
}

void GeneralInfoView::onDataChanged(wxCommandEvent& event) {
    txtCharacter->ChangeValue(AppState->savedata->main.character.string.str());
    txtIsland->ChangeValue(AppState->savedata->main.island.string.str());
}

void GeneralInfoView::onCharacterNameChanged(wxCommandEvent& event) {
    std::cout << "Yeet" << std::endl;
}
