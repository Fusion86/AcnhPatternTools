#pragma once

#include "wx.hpp"

#include "AppData.hpp"
#include "Events.hpp"

class GeneralInfoView : public wxWindow {
  public:
    GeneralInfoView(wxWindow* parent);

  private:
    wxTextCtrl* txtCharacter;
    wxTextCtrl* txtIsland;

    void OnDataChanged(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
