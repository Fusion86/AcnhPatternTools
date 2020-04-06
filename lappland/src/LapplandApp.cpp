#include "LapplandApp.hpp"

#include <filesystem>

#include <fmt/format.h>

namespace fs = std::filesystem;

wxIMPLEMENT_APP(LapplandApp);

bool LapplandApp::OnInit() {
    wxFrame* frame = new MyFrame("Hello World", wxPoint(500, 500), wxSize(500, 500));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_OpenFile, "&Open\tCtrl-O", "Open savedata");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("So far so good.");
}

void MyFrame::OnOpenFile(wxCommandEvent& event) {
    wxDirDialog* dialog = new wxDirDialog(this, "Select directory containing savedata", "",
                                          wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog->ShowModal() == wxID_OK) {
        fs::path path(dialog->GetPath().ToStdString());
        int res = getData()->savedata->load(path);
        if (res != 0) {
            SetStatusText("Couldn't load savedata!");
        } else {
            SetStatusText(fmt::format("Loaded savedata from {}", path.string()));
        }
    }

    dialog->Destroy();
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Hello World",
                 wxOK | wxICON_INFORMATION);
}
