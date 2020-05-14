#include "Renderer.hpp"
#include "math/vector.hpp"
#include "vp/Entity.hpp"

pixi::ui::Renderer::Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height)
    : pixi::ui::Box(parent, x, y, width, height, "")
{

}

void pixi::ui::Renderer::fill(const pixi::ui::Color color, const char symbol)
{
    for (int i = m_size.Y - 3; i >= 0; --i) {
        for (int j = m_size.X - 3; j >= 0; --j) {
            Box::get(j + 1, i + 1).Char.AsciiChar = symbol;
            Box::get(j + 1, i + 1).Attributes = unsigned(color);
        }
    }
}
