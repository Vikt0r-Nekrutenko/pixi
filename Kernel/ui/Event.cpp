#include "Event.hpp"
#include "BasicTemplate.hpp"

void pixi::ui::Event::operator =(const TMouseEventHandler handler)
{
    m_mouseHandler = handler;
}

void pixi::ui::Event::operator =(const TKeyEventHandler handler)
{
    m_keyHandler = handler;
}

void pixi::ui::Event::call(Window *sender, const MouseRecord mouseRecord)
{
    if (m_mouseHandler != nullptr) {
        (sender->*m_mouseHandler)(mouseRecord);
    }
}

void pixi::ui::Event::call(Window *sender, const KeyRecord keyRecord)
{
    if (m_keyHandler != nullptr) {
        (sender->*m_keyHandler)(keyRecord);
    }
}
