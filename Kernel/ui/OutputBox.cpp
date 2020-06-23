#include "OutputBox.hpp"

pixi::ui::OutputBox::OutputBox(const Window *parent, const short x, const short y, const short width, const short height, const std::string title)
    : InputBox(parent, x, y, width, height, title)
{

}

void pixi::ui::OutputBox::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    if (keyRecord.isPressed) {
        isControlKeyPressed(keyRecord.keyCode);
    }
    keyEvent.call(sender, keyRecord);
}

bool pixi::ui::OutputBox::isControlKeyPressed(unsigned short key)
{
    switch (key) {
    case VK_LEFT:
    {
        scrollLeft();
        show();
        return true;
    }
    case VK_RIGHT:
    {
        scrollRight();
        show();
        return true;
    }
    default:
        return false;
    }
}
