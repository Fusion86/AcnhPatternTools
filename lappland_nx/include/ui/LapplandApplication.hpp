#pragma once

#include "plutonium.hpp"

#include "ui/MainLayout.hpp"

namespace ui {
    class LapplandApplication : public Application {
      public:
        using Application::Application;
        PU_SMART_CTOR(LapplandApplication)

        void OnLoad() override;

      private:
        MainLayout::Ref mainLayout;
    };
} // namespace ui
