#include "Box.hpp"
#include "Window.hpp"

pixi::ui::Box::Box(const Window *parent, const short x, const short y, const short width, const short height, const std::string title)
    : m_title(title),
      m_color(Color::FG_GREY),
      m_rect(nullptr),
      m_parent { const_cast<Window *>(parent) }
{
    m_size.X = width  < 3 ? 3 : width;
    m_size.Y = height < 3 ? 3 : height;

    m_frame = {x, y, short(x + m_size.X), short(y + m_size.Y)};

    resize(m_size.X, m_size.Y);
    setTitle(title);
}

pixi::ui::Box::~Box()
{
    delete [] m_rect;
}

CHAR_INFO &pixi::ui::Box::get(const short x, const short y)
{
    return m_rect[m_size.X * y + x];
}

void pixi::ui::Box::resize(const short width, const short height)
{
    if (m_rect != nullptr) {
        fill(); show();
        delete [] m_rect;
    }

    m_size.X = width  < 3 ? 3 : width;
    m_size.Y = height < 3 ? 3 : height;

    m_rect = new CHAR_INFO[m_size.X * m_size.Y];
    fill(m_color, ' ');

    for (short y = 0; y < m_size.Y; y++) {
        for (short x = 0; x < m_size.X; x++) {
            if (y == 0 || y == m_size.Y - 1) {
                get(x, y).Char.AsciiChar = char(196); // -
            }
            if (x == 0 || x == m_size.X - 1) {
                get(x, y).Char.AsciiChar = char(179); // |
            }
        }
    }

    get(0, 0).Char.AsciiChar                        = char(218);
    get(m_size.X - 1, 0).Char.AsciiChar             = char(191);
    get(0, m_size.Y - 1).Char.AsciiChar             = char(192);
    get(m_size.X - 1, m_size.Y - 1).Char.AsciiChar  = char(217);

    setTitle(m_title);
}

void pixi::ui::Box::move(const short x, const short y)
{
    fill(); show();
    m_frame = { x, y, short(x + m_size.X), short(y + m_size.Y) };
    fill(m_color); show();
}

void pixi::ui::Box::fill(const Color color, const char symbol)
{
    for (int i = m_size.X * m_size.Y - 1; i >= 0; --i) {
        m_rect[i].Char.AsciiChar = (symbol == UNDEF_SYMBOL) ? m_rect[i].Char.AsciiChar : symbol;
        m_rect[i].Attributes = WORD(color);
    }
}

void pixi::ui::Box::setTitle(const std::string title)
{
    for (size_t i = 1ull; i <= title.length() && i < size_t(m_size.X - 1); i++) {
        m_rect[i].Char.AsciiChar = title[i - 1];
    }
}

void pixi::ui::Box::show()
{
    WriteConsoleOutputA(m_parent->out(), m_rect, m_size, { 0, 0 }, &m_frame);
}

void pixi::ui::Box::activate()
{
    m_color = Color(WORD(m_color) | FOREGROUND_INTENSITY); fill(m_color); show();
}

void pixi::ui::Box::deactivate()
{
    m_color = Color(WORD(m_color) ^ FOREGROUND_INTENSITY); fill(m_color); show();
}

const SMALL_RECT &pixi::ui::Box::getFrame() const
{
    return m_frame;
}

