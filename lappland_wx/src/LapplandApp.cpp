#include "LapplandApp.hpp"
#include "MainFrame.hpp"

wxIMPLEMENT_APP(LapplandApp);

bool LapplandApp::OnInit() {
    // Init some general stuff
    wxInitAllImageHandlers();

    // Create and show main window
    wxFrame* frame =
        new MainFrame("No savedata loaded - Lappland", wxDefaultPosition, wxSize(700, 500));
    frame->Show(true);
    return true;
}
