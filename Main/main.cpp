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
        renderer(this, 0, 0, 90, 25),
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
//        model.spawnEntities(&swdf, &mwdf);
    }
    void onRendererClicked(const MouseRecord mer) {
        if (mer.isPressed && mer.button == MouseButton::LEFT) {
            std::cout << "\rEntity: ";
            pixi::vp::Entity *ent = renderer.get(mer.position.X, mer.position.Y);
            if (ent != nullptr && ent->type() != pixi::vp::EntityType::Agent)
                std::cout << ((pixi::vp::Ware*)ent)->name() << "\r";
            else if (ent->type() == pixi::vp::EntityType::Agent)
                std::cout << ((pixi::vp::Agent*)ent)->px() << " " << ((pixi::vp::Agent*)ent)->py() << std::endl;
        }
    }

    const pixi::files::file swdf;
    const pixi::files::file mwdf;
    pixi::vp::Renderer renderer;
    pixi::vp::Model model;
};

int main()
{
    window wnd;
    windowEventProc(&wnd, true);

    return 0;
}
