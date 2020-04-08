#pragma once

#include "wx.hpp"

// Could also use a "CompileTimeCounter" here
#define DECLARE_BASE_ID(x) static constexpr int BASE_ID = x##_ID_BASE
#define DECLARE_WIDGET_ID(x, id) static constexpr int ID_##x = BASE_ID + id

enum { GeneralInfoView_ID_BASE = wxID_HIGHEST + 1 };
