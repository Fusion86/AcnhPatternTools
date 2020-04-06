#pragma once

#include "wx.hpp"
#include <wx/notebook.h>

#include "AppData.hpp"

class LapplandApp : public wxApp {
  public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame, public UsesAppData {
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

  private:
    wxNotebook* notebook;

    void OnOpenFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum {
    // ID_OpenFile = 1,
};

// clang-format off
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
    EVT_MENU(wxID_SAVE, MyFrame::OnSaveFile)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE();
// clang-format on
