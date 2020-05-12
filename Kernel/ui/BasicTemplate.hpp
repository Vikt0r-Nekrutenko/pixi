#ifndef BASICTEMPLATE_HPP
#define BASICTEMPLATE_HPP

#include "Event.hpp"
#include <windows.h>

namespace pixi { namespace ui {

enum class MouseButton {
    NONE = 0x0000,
    LEFT = 0x0001,
    RIGHT = 0x0002,
};

struct KERNEL_EXPORT KeyRecord {
    bool isPressed;
    char symbol;
    unsigned short keyCode;
};

struct KERNEL_EXPORT MouseRecord {
    MouseButton button;
    COORD position;
    bool isPressed;
};

class KERNEL_EXPORT BasicTemplate
{
    friend void KERNEL_EXPORT windowEventProc(Window *window, const bool updateIsEnabled);
public:
    virtual ~BasicTemplate() = default;
    virtual void resize(const short width, const short height) = 0;
    virtual void move(const short x, const short y) = 0;

protected:
    virtual void mouseEventHandler(Window *sender, const MouseRecord mouseRecord);
    virtual void keyEventHandler(Window *sender, const KeyRecord keyRecord);

    Event mouseEvent;
    Event keyEvent;
};

}}
#endif // BASICTEMPLATE_HPP
