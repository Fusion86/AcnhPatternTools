#include "MainFrame.hpp"

#include <filesystem>

#include <fmt/format.h>

#include "DesignPatternsView.hpp"
#include "GeneralInfoView.hpp"
#include "TurnipPricesView.hpp"

#define VERSION_TEXT "Lappland " lappland_wx_VERSION_STR

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open savedata");
    menuFile->Append(wxID_SAVE, "&Save\tCtrl-S", "Save savedata");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuTools = new wxMenu();
    menuTools->Append(ID_menuSaveCrypto, "Encrypt/Decrypt savedata");

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuHelp, "&Help");

    wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    notebook = new wxNotebook(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    notebook->AddPage(new GeneralInfoView(notebook), "General");
    notebook->AddPage(new DesignPatternsView(notebook), "Design Patterns");
    notebook->AddPage(new TurnipPricesView(notebook), "Turnip Prices");

    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    container->Add(notebook, 1, wxEXPAND | wxALL, 5);

    Bind(wxEVT_MENU, &MainFrame::onOpenFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::onSaveFile, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainFrame::onExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::onAbout, this, wxID_ABOUT);

    panel->SetSizer(container);
    SetMinClientSize(wxSize(800, 600));

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText(VERSION_TEXT);
}

void MainFrame::onOpenFile(wxCommandEvent& event) {
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
            for (size_t i = 0; i < notebook->GetPageCount(); i++) {
                wxCommandEvent event(EVT_DATA_CHANGED);
                wxPostEvent(notebook->GetPage(i), event);
            }
        }
    }
}

void MainFrame::onSaveFile(wxCommandEvent& event) {
    if (not AppState->savedata->loaded()) {
        wxMessageDialog dialog(this, "Can't save savedata, because no savedata has been loaded.",
                               "No savedata loaded");
        dialog.ShowModal();
        return;
    }

    wxDirDialog dirDialog(this, "Select directory where to save the savedata", "",
                          wxDD_DEFAULT_STYLE);

    if (dirDialog.ShowModal() == wxID_OK) {
        wxMessageDialog encryptDialog(this, "Do you want to encrypt the savedata?",
                                      "Encrypt savedata?", wxYES_NO | wxCANCEL);
        encryptDialog.SetYesNoLabels("Encrypt", "Don't encrypt");

        int res = encryptDialog.ShowModal();
        if (res == wxID_CANCEL) return;
        bool encrypt = res == wxID_YES;

        std::string path(dirDialog.GetPath().ToStdString());
        AppState->savedata->save(path, encrypt);
        SetStatusText(
            fmt::format("Saved {} savedata to {}", encrypt ? "encrypted" : "unencrypted", path));
    }
}

void MainFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample", "About Lappland",
                 wxOK | wxICON_INFORMATION);
}
