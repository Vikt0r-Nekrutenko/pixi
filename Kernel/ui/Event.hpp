#ifndef EVENT_HPP
#define EVENT_HPP

#include "Kernel_global.hpp"

namespace pixi { namespace ui {

class KERNEL_EXPORT Window;
struct KERNEL_EXPORT MouseRecord;
struct KERNEL_EXPORT KeyRecord;

typedef void (Window::*TMouseEventHandler)(const MouseRecord mouseRecord);
typedef void (Window::*TKeyEventHandler)(const KeyRecord keyRecord);

class KERNEL_EXPORT Event
{
public:
    void operator = (const TMouseEventHandler handler);
    void operator = (const TKeyEventHandler handler);

    void call(Window *sender, const MouseRecord mouseRecord);
    void call(Window *sender, const KeyRecord keyRecord);
private:
    TMouseEventHandler m_mouseHandler = nullptr;
    TKeyEventHandler   m_keyHandler = nullptr;
};

}}
#endif // EVENT_HPP
