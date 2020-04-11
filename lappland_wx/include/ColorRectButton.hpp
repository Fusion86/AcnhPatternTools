#pragma once

#include "wx.hpp"

#include <AcnhTypes.hpp>

class ColorRectButton : public wxWindow {
  public:
    ColorRectButton(wxWindow* parent, wxWindowID id, wxColour color,
                    wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

    wxColour getColor() const;
    void setColor(wxColour color);
    void setColor(AcnhColor color);
};
