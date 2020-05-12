#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "BasicTemplate.hpp"
#include <initializer_list>
#include <vector>

namespace pixi { namespace ui {

namespace
{
    const short CENTER { 0xfff };
    const short DEF_WND_WIDTH { 90 };
    const short DEF_WND_HEIGHT{ 30 };
}

class Box;

class KERNEL_EXPORT Window : public BasicTemplate
{
    friend void KERNEL_EXPORT windowEventProc(Window *window, const bool updateIsEnabled);
public:
    Window(const bool fullscreenIsEnabled);
    Window(const short x = CENTER, const short y = CENTER, const short width = DEF_WND_WIDTH, const short height = DEF_WND_HEIGHT);
    void setFontParams(const short width = 8, const short height = 12, const bool isBold = false, const wchar_t *fontFaceName = L"Terminal");
    void setCursorPosition(const short x, const short y) const;
    void setCursorParams(const bool isVisible = false, const short size = 1) const;
    void addNewControl(const std::initializer_list<Box *> controls);

    virtual void secondElased(const float fps) {};
    virtual void update(const float deltaTime) {};

    const HANDLE &out() const;
    COORD windowSize() const;
    COORD placeSize() const;

protected:
    std::vector <Box *> m_controls;
    COORD  m_size;
    HANDLE m_out, m_in;
    HWND m_wnd;

private:
    void move(const short x, const short y) final;
    void resize(const short width, const short height) final;
};

void KERNEL_EXPORT windowEventProc(Window *window, const bool updateIsEnabled);

}}
#endif // WINDOW_HPP
