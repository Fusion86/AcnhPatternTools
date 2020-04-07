#include "GeneralInfoView.hpp"

GeneralInfoView::GeneralInfoView(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    wxPanel* panel = new wxPanel(this, -1);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* lblCharacter = new wxStaticText(panel, wxID_ANY, "Character");

    txtCharacter = new wxTextCtrl(panel, wxID_ANY);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2);
    fgs->Add(lblCharacter);
    fgs->Add(txtCharacter, 1, wxEXPAND);

    fgs->AddGrowableCol(1, 1);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(hbox);
    Centre();
}

void GeneralInfoView::OnUpdate(wxCommandEvent& event) {}
