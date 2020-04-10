#pragma once

#include "wx.hpp"
#include <wx/generic/statbmpg.h>

#include <AcnhTypes.hpp>
#include <fmt/format.h>

#include "AppData.hpp"
#include "Events.hpp"
#include "Identifiers.hpp"

class IDesignPatternProxy {
  public:
    virtual ~IDesignPatternProxy();

    virtual bool isProPattern() = 0;
    virtual int getResolution() const = 0;

    virtual std::string getName() const = 0;
    virtual void setName(const std::string& str) = 0;
    virtual std::string getCreatorName() const = 0;
    virtual std::string getCreatorId() const = 0;
    virtual std::string getIslandName() const = 0;
    virtual std::string getIslandId() const = 0;
    virtual std::unique_ptr<uint8_t[]> getRgbData() const = 0;
    virtual std::unique_ptr<uint8_t[]> getAlphaData() const = 0;
};

template <class T>
class DesignPatternProxyImpl : public IDesignPatternProxy {
  private:
    T& src;
    int idx;
    wxListBox* lstBox;

  public:
    DesignPatternProxyImpl(T& src, int idx, wxListBox* lstBox)
        : src(src), idx(idx), lstBox(lstBox) {}

    bool isProPattern() {
        return typeid(T) == typeid(ProDesignPattern);
    }

    int getResolution() const {
        return src.getResolution();
    }

    std::string getName() const {
        return src.name.str();
    }

    void setName(const std::string& str) {
        src.name = str; // Update source object
        lstBox->SetString(idx, src.name.str()); // Update item in listbox
    }

    std::string getCreatorName() const {
        return src.creator.string.str();
    }

    std::string getCreatorId() const {
        return fmt::format("{:04X}", src.creator.id);
    }

    std::string getIslandName() const {
        return src.island.string.str();
    }

    std::string getIslandId() const {
        return fmt::format("{:04X}", src.island.id);
    }

    std::unique_ptr<uint8_t[]> getRgbData() const {
        return src.getRgbData();
    }

    std::unique_ptr<uint8_t[]> getAlphaData() const {
        return src.getAlphaData();
    }
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
    wxTextCtrl* txtPatternCreator;
    wxTextCtrl* txtPatternCreatorId;
    wxTextCtrl* txtPatternIsland;
    wxTextCtrl* txtPatternIslandId;

    wxGenericStaticBitmap* bmpPatternCtrl;

    IDesignPatternProxy* selectedDesignPattern = nullptr;

    std::vector<DesignPatternProxy> designPatterns;
    std::vector<ProDesignPatternProxy> proDesignPatterns;

    void onDataChanged(wxCommandEvent& event);
    void onSelectionChanged(wxCommandEvent& event);
    void onPatternNameChanged(wxCommandEvent& event);

    void setSelectedPattern(IDesignPatternProxy* pattern);

    DECLARE_BASE_ID(DesignPatternsView);
    DECLARE_WIDGET_ID(lstDesignPatterns, 0);
    DECLARE_WIDGET_ID(lstProDesignPatterns, 1);
    DECLARE_WIDGET_ID(txtPatternName, 2);
    DECLARE_WIDGET_ID(txtPatternCreator, 3);
    DECLARE_WIDGET_ID(txtPatternCreatorId, 4);
    DECLARE_WIDGET_ID(txtPatternIsland, 4);
    DECLARE_WIDGET_ID(txtPatternIslandId, 4);
};
