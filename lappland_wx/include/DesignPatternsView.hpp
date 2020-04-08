#pragma once

#include "wx.hpp"

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class DesignPatternsView : public wxPanel {
  public:
    DesignPatternsView(wxWindow* parent);

  private:
    wxListBox* lstDesignPatterns;
    wxListBox* lstProDesignPatterns;

    void OnDataChanged(wxCommandEvent& event);
    void OnSelectionChanged(wxCommandEvent& event);

    DECLARE_BASE_ID(DesignPatternsView);
    DECLARE_WIDGET_ID(lstDesignPatterns, 0);
    DECLARE_WIDGET_ID(lstProDesignPatterns, 1);
};
