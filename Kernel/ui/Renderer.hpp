#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cassert>
#include <vector>
#include "ui/Box.hpp"
#include "vp/Entity.hpp"
#include "vp/Agent.hpp"

namespace pixi { namespace ui {

class KERNEL_EXPORT Renderer : public pixi::ui::Box
{
public:
    Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height);
    inline bool isEnable() const
    {
        return m_isEnable;
    }
    inline void enable()
    {
        m_isEnable = true;
    }
    inline void disable()
    {
        m_isEnable = false;
    }
    inline short w() const
    {
        return m_size.X - 2;
    }
    inline short h() const
    {
        return m_size.Y - 2;
    }
    void fill(const Color color = Color::FG_BLACK, const char symbol = ui::UNDEF_SYMBOL) final;
    template <class T> void update(std::vector<T *> &objects)
    {
        for (vp::Entity *e : objects) {
            assert(e->px() < w() && e->py() < h());
            if (!e->isDestroyed()) {
                Box::get(e->px() + 1, e->py() + 1).Char.AsciiChar = e->symbol();
                Box::get(e->px() + 1, e->py() + 1).Attributes = vp::Entity::dword(e->color());
            }
        }
    }

private:
    bool m_isEnable = true;
};

}}

#endif // RENDERER_HPP
