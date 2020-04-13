#include "ImageViewFrame.hpp"

ImageViewFrame::ImageViewFrame(wxWindow* parent, const wxString& title, const wxPoint& pos,
                               const wxSize& size)
    : wxFrame(parent, wxID_ANY, title, pos, size) {
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_SAVE, "&Save\tCtrl-S", "Save image");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");

    Bind(wxEVT_MENU, &ImageViewFrame::onSaveFile, this, wxID_SAVE);
    Bind(wxEVT_PAINT, &ImageViewFrame::onPaint, this);

    SetMenuBar(menuBar);
    // CreateStatusBar();
    // SetStatusText("Lappland vX.X.X.X"); // TODO: Maybe show what tiles are shown?

    // TODO: Scale bitmap when user resizes window
    // Maybe something like this https://forums.wxwidgets.org/viewtopic.php?t=8810
}

void ImageViewFrame::SetBitmap(const wxBitmap& bmp) {
    bitmap = bmp;
}

void ImageViewFrame::onSaveFile(wxCommandEvent& event) {
    wxFileDialog dialog(this, "Where to save the image?", "", "image.png",
                        "PNG Image (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dialog.ShowModal() == wxID_OK) {
        bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
    }
}

void ImageViewFrame::onPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    render(dc);
}

void ImageViewFrame::render(wxPaintDC& dc) {
    const wxSize size = GetSize();
    int x_scale = size.GetWidth() / bitmap.GetWidth();
    int y_scale = size.GetHeight() / bitmap.GetHeight();
    int scale = std::min(x_scale, y_scale);

    if (scale > 0) {
        dc.SetUserScale(scale, scale);
        dc.DrawBitmap(bitmap, 0, 0, true);
    }
}
