#include "LapplandApp.hpp"

#include <wx/settings.h>

wxIMPLEMENT_APP(LapplandApp);

bool LapplandApp::OnInit() {
    wxFrame* frame = new MyFrame("Hello World", wxPoint(500, 500), wxSize(500, 500));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    CreateStatusBar();
    SetStatusText("So far so good.");
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Hello World", wxOK | wxICON_INFORMATION);
}
