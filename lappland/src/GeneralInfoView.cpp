#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(500,500)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText* lblCharacter = new wxStaticText(panel, wxID_ANY, "Character");
    wxStaticText* lblIsland = new wxStaticText(panel, wxID_ANY, "Island");

    txtCharacter = new wxTextCtrl(panel, wxID_ANY);
    txtIsland = new wxTextCtrl(panel, wxID_ANY);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2, 2, 9, 25);
    fgs->Add(lblCharacter);
    fgs->Add(txtCharacter, 1, wxEXPAND);
    fgs->Add(lblIsland);
    fgs->Add(txtIsland, 1, wxEXPAND);

    fgs->AddGrowableCol(1, 1);

    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(vbox);
    Centre();
}

void GeneralInfoView::OnUpdate(wxCommandEvent& event) {}
