#pragma once

#include "wx.hpp"

#include "AppData.hpp"

class GeneralInfoView : public wxWindow {
  public:
    GeneralInfoView(wxWindow* parent);

  private:
    wxTextCtrl* txtCharacter;

    void OnUpdate(wxCommandEvent& event);
    // wxDECLARE_EVENT_TABLE();
};
