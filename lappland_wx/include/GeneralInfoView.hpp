#pragma once

#include "wx.hpp"

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class GeneralInfoView : public wxPanel {
  public:
    GeneralInfoView(wxWindow* parent);

  private:
    wxTextCtrl* txtCharacter;
    wxTextCtrl* txtIsland;

    void onDataChanged(wxCommandEvent& event);
    void onCharacterNameChanged(wxCommandEvent& event);

    DECLARE_BASE_ID(GeneralInfoView);
    DECLARE_WIDGET_ID(txtCharacter, 0);
};
