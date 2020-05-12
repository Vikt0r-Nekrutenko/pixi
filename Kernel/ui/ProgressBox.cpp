#include "ProgressBox.hpp"
#include <cassert>

pixi::ui::ProgressBox::ProgressBox(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height, const float value, const pixi::ui::Color barColor, const std::string title)
    : Box(parent, x, y, width, height, title),
      m_barColor(barColor),
      m_value(value)
{
    assert(m_value > 1.f);
}

void pixi::ui::ProgressBox::increase() {
    float realLen = m_size.X - 2;
    m_progress += realLen / m_value;

    for (int i = 0; i < m_progress && m_progress < realLen; i++) {
        m_rect[i + m_size.X + 1].Char.AsciiChar = -37;
        m_rect[i + m_size.X + 1].Attributes = unsigned(m_barColor) + ((unsigned(m_barColor) * unsigned(Color::BG_BLUE)));
    }
    std::string valueStr = std::to_string(int(m_real++));
    for (int i = realLen / 2 + m_size.X - valueStr.length() / 2, j = 0; j < valueStr.length() && i < realLen + m_size.X; ++j, ++i) {
        m_rect[i].Char.AsciiChar = valueStr[j];
        m_rect[i].Attributes |= unsigned(m_color);
    }
    show();
}
