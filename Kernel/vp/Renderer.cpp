#include "Renderer.hpp"
#include "Model.hpp"

pixi::vp::Renderer::Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height)
    : pixi::ui::Box(parent, x, y, width, height, "")
{

}

pixi::vp::Entity *pixi::vp::Renderer::get(const short x, const short y)
{
    short _x = x - m_frame.Left;
    short _y = y - m_frame.Top;

    return m_model->get(_x - 1, _y - 1);
}

void pixi::vp::Renderer::fill(const pixi::ui::Color color, const char symbol)
{
    for (int i = m_size.Y - 3; i >= 0; --i) {
        for (int j = m_size.X - 3; j >= 0; --j) {
            Box::get(j + 1, i + 1).Char.AsciiChar = symbol;
            Box::get(j + 1, i + 1).Attributes = unsigned(color);
        }
    }
}

void pixi::vp::Renderer::update(const float deltaTile)
{
    for (int i = m_size.Y - 3; i >= 0; --i) {
        for (int j = m_size.X - 3; j >= 0; --j) {
            if (m_model->get(j, i) != nullptr) {
                Box::get(j + 1, i + 1).Char.AsciiChar = m_model->get(j, i)->symbol();
                Box::get(j + 1, i + 1).Attributes = unsigned(m_model->get(j, i)->color());
            } else {
                Box::get(j + 1, i + 1).Char.AsciiChar = pixi::ui::UNDEF_SYMBOL;
                Box::get(j + 1, i + 1).Attributes = unsigned(pixi::ui::Color::FG_BLACK);
            }
        }
    }
}
