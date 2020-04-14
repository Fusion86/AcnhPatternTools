#include "ImageViewFrame.hpp"

#include <wx/graphics.h>

#include <fmt/format.h>

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
    CreateStatusBar();
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
    const wxSize size = GetClientSize();
    int x_scale = size.GetWidth() / bitmap.GetWidth();
    int y_scale = size.GetHeight() / bitmap.GetHeight();
    int scale = std::min(x_scale, y_scale);

    SetStatusText(fmt::format("Scale: {}", scale));

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
    gc->SetInterpolationQuality(wxINTERPOLATION_NONE);
    gc->Scale(scale, scale);

    if (scale > 0) {
        // Center image
        int x = (size.GetWidth() / scale - bitmap.GetWidth()) / 2;
        int y = (size.GetHeight() / scale - bitmap.GetHeight()) / 2;

        // Draw image
        dc.DrawBitmap(bitmap, x, y, true);
    }

    delete gc;
}
