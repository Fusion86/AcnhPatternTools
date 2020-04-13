#pragma once

#include "wx.hpp"

class ImageViewFrame : public wxFrame {
  private:
    wxBitmap bitmap;

  public:
    ImageViewFrame(wxWindow* parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    void SetBitmap(const wxBitmap& bmp);

  private:
    void onSaveFile(wxCommandEvent& event);
    void onPaint(wxPaintEvent& event);

    void render(wxPaintDC& dc);
};
