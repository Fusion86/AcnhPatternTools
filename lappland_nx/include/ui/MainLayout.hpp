#pragma once

#include "plutonium.hpp"

namespace ui {
    class MainLayout : public Layout {
      public:
        MainLayout();
        PU_SMART_CTOR(MainLayout)

      private:
        TextBlock::Ref helloText;
        // Rectangle::Ref helloRect;
    };
} // namespace ui
