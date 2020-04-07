#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(500,500)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

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

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(hbox);
    Centre();
}

void GeneralInfoView::OnUpdate(wxCommandEvent& event) {}
