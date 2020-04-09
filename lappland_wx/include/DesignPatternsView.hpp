#pragma once

#include "wx.hpp"
#include <wx/generic/statbmpg.h>

#include <AcnhTypes.hpp>

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class IDesignPatternProxy {
  public:
    virtual std::string getName() const = 0;
    virtual void setName(const std::string& str) = 0;
};

template <class T>
class DesignPatternProxyImpl : public IDesignPatternProxy {
  private:
    T* src;

  public:
    DesignPatternProxyImpl(T* src) : src(src) {}

    std::string getName() const {
        return src->name.str();
    }

    void setName(const std::string& str) {}
};

typedef DesignPatternProxyImpl<DesignPattern> DesignPatternProxy;
typedef DesignPatternProxyImpl<ProDesignPattern> ProDesignPatternProxy;

class DesignPatternsView : public wxPanel {
  public:
    DesignPatternsView(wxWindow* parent);

  private:
    wxListBox* lstDesignPatterns;
    wxListBox* lstProDesignPatterns;

    wxTextCtrl* txtPatternName;

    wxGenericStaticBitmap* bmpPatternCtrl;

    IDesignPatternProxy* selectedDesignPattern = nullptr;

    std::vector<DesignPatternProxy> designPatterns;
    std::vector<ProDesignPatternProxy> proDesignPatterns;

    void onDataChanged(wxCommandEvent& event);
    void onSelectionChanged(wxCommandEvent& event);

    DECLARE_BASE_ID(DesignPatternsView);
    DECLARE_WIDGET_ID(lstDesignPatterns, 0);
    DECLARE_WIDGET_ID(lstProDesignPatterns, 1);
    DECLARE_WIDGET_ID(txtPatternName, 2);
};
