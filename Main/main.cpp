#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "ui/Window.hpp"
#include "vp/Renderer.hpp"
#include "vp/Model.hpp"

using namespace pixi::ui;

class window : public Window
{
public:
    window() : Window(),
        swdf("../Kernel/data/SoftwareDataSetRaw.data"),
        mwdf("../Kernel/data/MalwareDataSet.data"),
        renderer(this, 10, 5, 70, 20),
        model(&renderer, &swdf, &mwdf)
    {
        addNewControl({ &renderer });
        renderer.setModel(&model);

        renderer.mouseEvent = TMouseEventHandler(&window::onRendererClicked);
    }
    void update(const float deltaTime) override
    {
        renderer.fill();
        renderer.update(deltaTime);
        renderer.show();
    }
    void secondElased(const float fps) override
    {

    }
    void onRendererClicked(const MouseRecord mer) {
        if (mer.isPressed && mer.button == MouseButton::LEFT) {
            std::cout << "\rEntity: ";
            if (renderer.get(mer.position.X, mer.position.Y) != nullptr)
                std::cout << renderer.get(mer.position.X, mer.position.Y)->name() << "\r";
        }
    }

    pixi::files::file swdf;
    pixi::files::file mwdf;
    pixi::vp::Renderer renderer;
    pixi::vp::Model model;
};

int main()
{
    window wnd;
    windowEventProc(&wnd, true);

    return 0;
}
