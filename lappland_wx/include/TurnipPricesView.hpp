#pragma once

#include "wx.hpp"
#include <wx/grid.h>

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class TurnipPricesView : public wxPanel {
  public:
    TurnipPricesView(wxWindow* parent);

  private:
    wxTextCtrl* txtBuyPrice;
    wxGrid* grid;

    void onDataChanged(wxCommandEvent& event);
    void onCellChanged(wxGridEvent& event);

    DECLARE_BASE_ID(TurnipPricesView);
    DECLARE_WIDGET_ID(txtBuyPrice, 0);
};
