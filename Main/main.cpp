#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "file.hpp"
#include "ui/Window.hpp"
#include "vp/Renderer.hpp"
#include "ui/NumericBox.hpp"

using namespace pixi::ui;

class window : public Window
{
public:
    window() : Window(),
        swdf("../Kernel/data/SoftwareDataSetRaw.data"),
        mwdf("../Kernel/data/MalwareDataSet.data"),
        renderer(this, 10, 5, 60, 15),
        speed(this, 0, 0, 10, 3, "Speed:")
    {
        addNewControl({ &renderer, &speed });

        renderer.mouseEvent = TMouseEventHandler(&window::onRendererClicked);
        renderer.keyEvent   = TKeyEventHandler(&window::onRendererKeyPressed);
        speed.keyEvent      = TKeyEventHandler(&window::onSpeedKeyPressed);
        renderer.init(&swdf, &mwdf);
    }
    void update(const float deltaTime) override
    {
        if (renderer.isEnable()) {
            renderer.fill();
            renderer.update(deltaTime);
            renderer.show();
        }
    }
    void secondElased(const float fps) override
    {
        renderer.spawnEntities(10);
    }
    void onSpeedKeyPressed(const KeyRecord ker)
    {
        if (ker.isPressed && ker.keyCode == VK_RETURN) {
            renderer.changeAgentSpeed(speed.getNumber());
        }
    }
    void onRendererKeyPressed(const KeyRecord ker) {
        if (ker.isPressed && ker.keyCode == VK_SPACE) {
            if (renderer.isEnable()) renderer.disable();
            else renderer.enable();
        }
    }
    void onRendererClicked(const MouseRecord mer) {
        if (mer.isPressed && mer.button == MouseButton::LEFT) {
            std::cout << "\rEntity: ";
            pixi::vp::Entity *ent = renderer.get(mer.position.X, mer.position.Y);
            if (ent != nullptr && ent->type() != pixi::vp::EntityType::Agent)
                std::cout << ((pixi::vp::Ware*)ent)->name() << "\r";
        }
    }

    const pixi::files::file swdf;
    const pixi::files::file mwdf;
    pixi::vp::Renderer renderer;
    pixi::ui::NumericBox speed;
};

int main()
{
    window wnd;
    windowEventProc(&wnd, true);

    return 0;
}
