#pragma once

#include "wx.hpp"
#include <wx/generic/statbmpg.h>

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class DesignPatternsView : public wxPanel {
  public:
    DesignPatternsView(wxWindow* parent);

  private:
    wxListBox* lstDesignPatterns;
    wxListBox* lstProDesignPatterns;

    wxTextCtrl* txtPatternName;

    wxGenericStaticBitmap* bmpPatternCtrl;

    DesignPattern* selectedDesignPattern = nullptr;
    ProDesignPattern* selectedProDesignPattern = nullptr;

    void onDataChanged(wxCommandEvent& event);
    void onSelectionChanged(wxCommandEvent& event);

    DECLARE_BASE_ID(DesignPatternsView);
    DECLARE_WIDGET_ID(lstDesignPatterns, 0);
    DECLARE_WIDGET_ID(lstProDesignPatterns, 1);
    DECLARE_WIDGET_ID(txtPatternName, 2);
};
