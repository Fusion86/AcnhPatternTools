#include "ui/LapplandApplication.hpp"

#include <iostream>

using namespace ui;
using namespace pu::ui::render;

int main(int argc, char** argv) {
    // consoleDebugInit(debugDevice_SVC);
    // std::cout << "Test" << std::endl;

    // while (appletMainLoop())
    // {}

    auto renderer = Renderer::New(SDL_INIT_EVERYTHING, RendererInitOptions::RendererNoSound,
                                  RendererHardwareFlags);

    auto app = LapplandApplication::New(renderer);
    app->Prepare();
    app->ShowWithFadeIn();

    return 0;
}
