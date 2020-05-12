#include "BasicTemplate.hpp"

void pixi::ui::BasicTemplate::mouseEventHandler(Window *sender, const MouseRecord mouseRecord)
{
    mouseEvent.call(sender, mouseRecord);
}

void pixi::ui::BasicTemplate::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    keyEvent.call(sender, keyRecord);
}
