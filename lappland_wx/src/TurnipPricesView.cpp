#include "TurnipPricesView.hpp"

#include <string>

TurnipPricesView::TurnipPricesView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxStaticText* lblBuyPrice = new wxStaticText(this, wxID_ANY, "Buy price");

    txtBuyPrice = new wxTextCtrl(this, ID_txtBuyPrice);

    wxFlexGridSizer* fgs0 = new wxFlexGridSizer(2, wxSize(10, 5));
    fgs0->Add(lblBuyPrice, 0, wxALIGN_CENTER_VERTICAL);
    fgs0->Add(txtBuyPrice, 1, wxEXPAND);

    fgs0->AddGrowableCol(1);

    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(7, 2);

    grid->SetColLabelValue(0, "Morning");
    grid->SetColLabelValue(1, "Evening");

    grid->SetRowLabelValue(0, "Sunday");
    grid->SetRowLabelValue(1, "Monday");
    grid->SetRowLabelValue(2, "Tuesday");
    grid->SetRowLabelValue(3, "Wednesday");
    grid->SetRowLabelValue(4, "Thursday");
    grid->SetRowLabelValue(5, "Friday");
    grid->SetRowLabelValue(6, "Saturday");

    wxFlexGridSizer* fgs1 = new wxFlexGridSizer(1, wxSize(10, 10));
    fgs1->Add(fgs0, 1, wxEXPAND);
    fgs1->Add(grid, 1, wxEXPAND);

    fgs1->AddGrowableCol(0);
    fgs1->AddGrowableRow(1);

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(fgs1, 1, wxEXPAND | wxALL, 10);
    SetSizer(hbox);

    Bind(EVT_DATA_CHANGED, &TurnipPricesView::onDataChanged, this);
    Bind(wxEVT_GRID_CELL_CHANGED, &TurnipPricesView::onCellChanged, this);
}

void TurnipPricesView::onDataChanged(wxCommandEvent& event) {
    txtBuyPrice->ChangeValue(std::to_string(AppState->savedata->main.turnipPrices.buyPrice));

    for (int i = 0; i < 7; i++) {
        grid->SetCellValue(i, 0, std::to_string(AppState->savedata->main.turnipPrices.days[i][0]));
        grid->SetCellValue(i, 1, std::to_string(AppState->savedata->main.turnipPrices.days[i][1]));
    }
}

void TurnipPricesView::onCellChanged(wxGridEvent& event) {
    if (not AppState->savedata->loaded()) return;

    try {
        int part = event.GetCol();
        int day = event.GetRow();
        int val = std::stoi(grid->GetCellValue(day, part).ToStdString());
        AppState->savedata->main.turnipPrices.days[day][part] = val;
    } catch (const std::exception&) {
        return;
    }
}
