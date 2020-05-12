#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include "Box.hpp"
#include <string>

namespace pixi { namespace ui {

class KERNEL_EXPORT InputBox : public Box
{
public:
    InputBox(const Window *parent, const short x, const short y, const short width = 10, const short height = 3, const std::string title = "Input");
    void show() override;
    void resize(const short width, const short height) override;
    void setText(const std::string &text);

    const std::string &getText() const;

protected:
    size_t m_lenVisibleText = 0ull;
    size_t m_symbolInText = 0ull, m_innerCursor = 0ull;
    size_t m_firstSymbol  = 0ull, m_lastSymbol  = 0ull;
    std::string m_text;

    CHAR_INFO &get(const size_t index);
    virtual bool isControlKeyPressed(unsigned short key);
    void keyEventHandler(Window *sender, const KeyRecord keyRecord) override;
    void showText();
    void scrollRight();
    void scrollLeft();
};

}}
#endif // INPUTBOX_HPP
