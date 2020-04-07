#include "LapplandApp.hpp"

#include <filesystem>

#include <fmt/format.h>

#include "GeneralInfoView.hpp"

namespace fs = std::filesystem;

wxIMPLEMENT_APP(LapplandApp);

bool LapplandApp::OnInit() {
    wxFrame* frame =
        new MyFrame("No savedata loaded - Lappland", wxDefaultPosition, wxSize(700, 500));
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

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    notebook = new wxNotebook(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    // notebook->Show(false);
    notebook->AddPage(new GeneralInfoView(notebook), "General");

    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    container->Add(notebook, 1, wxEXPAND);

    panel->SetSizerAndFit(container);

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Lappland vX.X.X.X");
}

void MyFrame::OnOpenFile(wxCommandEvent& event) {
    wxDirDialog* dialog = new wxDirDialog(this, "Select directory containing savedata", "",
                                          wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog->ShowModal() == wxID_OK) {
        fs::path path(dialog->GetPath().ToStdString());
        int res = AppState->savedata->load(path);
        if (res != 0) {
            SetStatusText("Couldn't load savedata!");
        } else {
            SetTitle(fmt::format("{} - Lappland", path.string()));
            SetStatusText(fmt::format("Loaded savedata from {}", path.string()));
            // notebook->Show(true);

            for (int i = 0; i < notebook->GetPageCount(); i++) {
                wxCommandEvent event(EVT_DATA_CHANGED);
                wxPostEvent(notebook->GetPage(i), event);
            }
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

// clang-format off
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)
    EVT_MENU(wxID_SAVE, MyFrame::OnSaveFile)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE();
// clang-format on
