#pragma once

#include "wx.hpp"
#include <wx/notebook.h>

#include "AppData.hpp"

class LapplandApp : public wxApp {
  public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

  private:
    wxNotebook* notebook;

    void onOpenFile(wxCommandEvent& event);
    void onSaveFile(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
};
