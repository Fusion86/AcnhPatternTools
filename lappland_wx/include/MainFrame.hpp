#pragma once

#include "wx.hpp"
#include <wx/notebook.h>

#include "AppData.hpp"
#include "Identifiers.hpp"

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

  private:
    wxNotebook* notebook;

    void onOpenFile(wxCommandEvent& event);
    void onSaveFile(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);

    DECLARE_BASE_ID(MainFrame);
    DECLARE_WIDGET_ID(menuSaveCrypto, 0);
};
