#pragma once

#include "wx.hpp"

// Could also use a "CompileTimeCounter" here
#define DECLARE_BASE_ID(x) static constexpr int BASE_ID = x##_ID_BASE
#define DECLARE_WIDGET_ID(x, id) static constexpr int ID_##x = BASE_ID + id
#define DECLARE_CLASS_BASE_ID(x, id) x##_ID_BASE = wxID_HIGHEST + id

enum {
    DECLARE_CLASS_BASE_ID(GeneralInfoView, 0),
    DECLARE_CLASS_BASE_ID(DesignPatternsView, 100),
    DECLARE_CLASS_BASE_ID(TurnipPricesView, 200),
    DECLARE_CLASS_BASE_ID(MainFrame, 300),
};
