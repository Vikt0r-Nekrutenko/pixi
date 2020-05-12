#include "Window.hpp"
#include "Box.hpp"
#include <ctime>

pixi::ui::Window::Window(const bool fullscreenIsEnabled)
    : BasicTemplate(),
      m_out(GetStdHandle(STD_OUTPUT_HANDLE)),
      m_in(GetStdHandle(STD_INPUT_HANDLE)),
      m_wnd(GetConsoleWindow())
{
    if (fullscreenIsEnabled) {
        m_size = GetLargestConsoleWindowSize(m_out);
        m_size.X += 10; m_size.Y += 10;
        resize(m_size.X, m_size.Y);

        SetWindowLongPtrA(m_wnd, GWL_STYLE, WS_CHILD);
        SetWindowPos(m_wnd, HWND_TOPMOST, -2, -2, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    } else {
        resize(DEF_WND_WIDTH, DEF_WND_HEIGHT);
        move(GetSystemMetrics(SM_CXSCREEN) / 2 - windowSize().X / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - windowSize().Y / 2);
    }
}

pixi::ui::Window::Window(const short x, const short y, const short width, const short height)
    : BasicTemplate(),
      m_out(GetStdHandle(STD_OUTPUT_HANDLE)),
      m_in(GetStdHandle(STD_INPUT_HANDLE)),
      m_wnd(GetConsoleWindow())
{
    resize(width, height);
    move(x == CENTER ? GetSystemMetrics(SM_CXSCREEN) / 2 - windowSize().X / 2 : x,
         y == CENTER ? GetSystemMetrics(SM_CYSCREEN) / 2 - windowSize().Y / 2 : y);
}

void pixi::ui::Window::setFontParams(const short width, const short height, const bool isBold, const wchar_t *fontFaceName)
{
    const short oldWidth = placeSize().X / width;
    const short oldHeight = placeSize().Y / height;

    CONSOLE_FONT_INFOEX fontInfo;
    ZeroMemory(&fontInfo, sizeof(CONSOLE_FONT_INFOEX));
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    lstrcpyW(fontInfo.FaceName, fontFaceName);
    fontInfo.FontFamily = (wcscmp(fontFaceName, L"Terminal")) ? 54 : FF_MODERN;
    fontInfo.FontWeight = isBold ? FW_BOLD : FW_NORMAL;
    fontInfo.dwFontSize.X = width;
    fontInfo.dwFontSize.Y = height;
    SetCurrentConsoleFontEx(m_out, FALSE, &fontInfo);

    resize(oldWidth, oldHeight);
}

void pixi::ui::Window::setCursorPosition(const short x, const short y) const
{
    SetConsoleCursorPosition(m_out, { x, y });
}

void pixi::ui::Window::setCursorParams(const bool isVisible, const short size) const
{
    CONSOLE_CURSOR_INFO inf;
    inf.dwSize = DWORD(size);
    SetConsoleCursorInfo(m_out, &inf);
    inf.bVisible = isVisible;
    SetConsoleCursorInfo(m_out, &inf);
}

void pixi::ui::Window::addNewControl(const std::initializer_list<pixi::ui::Box *> controls)
{
    for (std::initializer_list<Box *>::iterator it = controls.begin(); it != controls.end(); it++) {
        m_controls.push_back(*it);
        (*it)->show();
    }
}

const HANDLE &pixi::ui::Window::out() const
{
    return m_out;
}

COORD pixi::ui::Window::windowSize() const
{
    RECT windowRect;
    GetWindowRect(m_wnd, &windowRect);
    return { short(windowRect.right - windowRect.left), short(windowRect.bottom - windowRect.top) };
}

COORD pixi::ui::Window::placeSize() const
{
    RECT clientRect;
    GetClientRect(m_wnd, &clientRect);
    return { short(clientRect.right - clientRect.left), short(clientRect.bottom - clientRect.top) };
}

void pixi::ui::Window::move(const short x, const short y)
{
    SetWindowPos(m_wnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
}

void pixi::ui::Window::resize(const short width, const short height)
{
    CONSOLE_SCREEN_BUFFER_INFOEX csbi;
    csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(m_out, &csbi);
    m_size = csbi.dwMaximumWindowSize = csbi.dwSize = { width, height };
    csbi.srWindow = { 0, 0, width, height };
    SetConsoleScreenBufferInfoEx(m_out, &csbi);
}

void KERNEL_EXPORT pixi::ui::windowEventProc(pixi::ui::Window *window, const bool updateIsEnabled)
{
    INPUT_RECORD ir[32];
    DWORD n;
    Box *activeControl = nullptr;
    float deltaTime = 0.f, elapsedTime = 0.f;
    size_t frames = 0ull;

    while (true)
    {
        if (elapsedTime > 1.f && updateIsEnabled) {
            window->secondElased(frames / elapsedTime);
            elapsedTime = frames = 0;
        }

        time_t begin = clock();

        if (updateIsEnabled) window->update(deltaTime);

        GetNumberOfConsoleInputEvents(window->m_in, &n);
        if (n > 0) {
            ReadConsoleInputA(window->m_in, ir, n, &n);

            for (DWORD i = 0; i < n; i++) {
                switch (ir[i].EventType)
                {
                case MOUSE_EVENT:
                {
                    short x = ir[i].Event.MouseEvent.dwMousePosition.X;
                    short y = ir[i].Event.MouseEvent.dwMousePosition.Y;

                    auto cursorInControl = [x, y](const Box *control) -> const Box * {
                        if (x > control->getFrame().Left && x < control->getFrame().Right && y > control->getFrame().Top && y < control->getFrame().Bottom) {
                            return control;
                        }
                        return nullptr;
                    };

                    auto findControl = [&activeControl, cursorInControl, &window](void) -> bool {
                        for (Box *control : window->m_controls) {
                            const Box *foundCtrl = cursorInControl(control);
                            if (foundCtrl != nullptr) {
                                if (activeControl != nullptr && foundCtrl != activeControl) {
                                    activeControl->deactivate();
                                }
                                if (foundCtrl != activeControl) {
                                    activeControl = control;
                                    activeControl->activate();
                                }
                                return true;
                            }
                        }
                        if (activeControl != nullptr) {
                            activeControl->deactivate();
                            activeControl = nullptr;
                        }
                        return false;
                    };

                    if (ir[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        if (findControl()){
                            activeControl->mouseEventHandler(window, { MouseButton::LEFT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::LEFT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    } else if (ir[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                        if (findControl()){
                            activeControl->mouseEventHandler(window, { MouseButton::RIGHT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::RIGHT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    }
                    if (ir[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
                        if (activeControl != nullptr && cursorInControl(activeControl)){
                            activeControl->mouseEventHandler(window, { MouseButton::NONE, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::NONE, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    }
                    break;
                }
                case KEY_EVENT:
                {
                    if (ir[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
                        return;
                    } else if (ir[i].Event.KeyEvent.bKeyDown) {
                        if (activeControl == nullptr) {
                            window->keyEventHandler(window, { true, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        } else {
                            activeControl->keyEventHandler(window, { true, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        }
                    } else {
                        if (activeControl == nullptr) {
                            window->keyEventHandler(window, { false, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        } else {
                            activeControl->keyEventHandler(window, { false, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        }
                    }
                    break;
                }
                }
            }
        }

        deltaTime = (clock() - begin) / 1000.f;
        elapsedTime += deltaTime;
        ++frames;
    }
}
