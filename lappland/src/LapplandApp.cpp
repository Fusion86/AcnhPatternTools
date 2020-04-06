#include "LapplandApp.hpp"

#include <filesystem>

#include <fmt/format.h>

namespace fs = std::filesystem;

wxIMPLEMENT_APP(LapplandApp);

bool LapplandApp::OnInit() {
    wxFrame* frame = new MyFrame("No file open - Lappland", wxDefaultPosition, wxSize(700, 500));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open savedata");
    menuFile->Append(wxID_SAVE, "&Save");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    notebook->AddPage(new wxStaticText(notebook, -1, "Test 1"), "Title 1");
    notebook->AddPage(new wxStaticText(notebook, -1, "Test 2"), "Title 2");

    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    container->Add(notebook, 1, wxEXPAND | wxALL, 5);
    SetSizer(container);

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Lappland vX.X.X.X");

    // Layout(); // Probably not needed
    SetAutoLayout(true);
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
            SetTitle(fmt::format("{} - Lappland", path.string()));
            SetStatusText(fmt::format("Loaded savedata from {}", path.string()));
        }
    }

    dialog->Destroy();
}

void MyFrame::OnSaveFile(wxCommandEvent& event) {}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Lappland",
                 wxOK | wxICON_INFORMATION);
}
