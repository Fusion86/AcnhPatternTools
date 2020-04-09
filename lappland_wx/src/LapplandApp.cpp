#include "LapplandApp.hpp"

#include <filesystem>

#include <fmt/format.h>

#include "DesignPatternsView.hpp"
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
    menuFile->Append(wxID_SAVE, "&Save\tCtrl-S", "Save savedata");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    notebook->AddPage(new GeneralInfoView(notebook), "General");
    notebook->AddPage(new DesignPatternsView(notebook), "Design Patterns");

    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    container->Add(notebook, 1, wxEXPAND | wxALL, 5);

    Bind(wxEVT_MENU, &MyFrame::OnOpenFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MyFrame::OnSaveFile, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    SetSizer(container);
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Lappland vX.X.X.X");
}

void MyFrame::OnOpenFile(wxCommandEvent& event) {
    wxDirDialog dialog(this, "Select directory containing savedata", "",
                       wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog.ShowModal() == wxID_OK) {
        std::string path(dialog.GetPath().ToStdString());
        int res = AppState->savedata->load(path);
        if (res != 0) {
            SetStatusText("Couldn't load savedata!");
        } else {
            SetTitle(fmt::format("{} - Lappland", path));
            SetStatusText(fmt::format("Loaded savedata from {}", path));

            // Notify all tabs of the change
            for (int i = 0; i < notebook->GetPageCount(); i++) {
                wxCommandEvent event(EVT_DATA_CHANGED);
                wxPostEvent(notebook->GetPage(i), event);
            }
        }
    }
}

void MyFrame::OnSaveFile(wxCommandEvent& event) {
    if (not AppState->savedata->loaded()) {
        wxMessageDialog dialog(this, "Can't save savedata, because no savedata has been loaded.",
                               "No savedata loaded");
        dialog.ShowModal();
        return;
    }

    wxDirDialog dirDialog(this, "Select directory where to save the savedata", "",
                          wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() == wxID_OK) {
        wxMessageDialog encryptDialog(this, "Do you want to encrypt the savedata?",
                                      "Encrypt savedata?", wxYES_NO);
        encryptDialog.SetYesNoLabels("Encrypt", "Don't encrypt");

        // FIX THIS
        bool encrypt = encryptDialog.ShowModal() == wxYES;

        std::string path(dirDialog.GetPath().ToStdString());
        AppState->savedata->save(path, encrypt);
        SetStatusText(
            fmt::format("Saved {} savedata to {}", encrypt ? "encrypted" : "unencrypted", path));
    }
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Lappland",
                 wxOK | wxICON_INFORMATION);
}
