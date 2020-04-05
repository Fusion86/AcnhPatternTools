#include "ui/MainLayout.hpp"

namespace ui {
    MainLayout::MainLayout() : Layout::Layout() {
        this->helloText = TextBlock::New(300, 300, "Test");

        // this->helloRect = Rectangle::New(50, 50, 200, 200, Color(255, 255, 255, 255));

        this->Add(this->helloText);
    }
}; // namespace ui
