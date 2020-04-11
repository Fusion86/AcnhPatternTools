#include "ColorRectButton.hpp"

ColorRectButton::ColorRectButton(wxWindow* parent, wxWindowID id, wxColour color, wxPoint pos,
                                 wxSize size)
    : wxWindow(parent, id, pos, size) {
    SetMinSize(size);
    setColor(color);
    // Bind(wxEVT_PAINT, &ColorRectButton::onPaint, this);
}

wxColour ColorRectButton::getColor() const {
    return GetBackgroundColour();
}

void ColorRectButton::setColor(wxColour color) {
    SetBackgroundColour(color);
}

void ColorRectButton::setColor(AcnhColor color) {
    setColor(wxColour(color.red, color.green, color.blue));
}
