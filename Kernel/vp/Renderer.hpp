#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "ui/Box.hpp"
#include <cassert>

namespace pixi { namespace vp {

class Model;
class Entity;

class KERNEL_EXPORT Renderer : public pixi::ui::Box
{
public:
    Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height);
    Entity *get(const short x, const short y);
    inline short w() const
    {
        return m_size.X - 2;
    }
    inline short h() const
    {
        return m_size.Y - 2;
    }
    inline void setModel(const Model *model)
    {
        m_model = const_cast<Model *>(model);
    }
    void fill(const pixi::ui::Color color = pixi::ui::Color::FG_RED, const char symbol = '$') final;
    void update(const float deltaTile);
private:
    Model *m_model = nullptr;
};

}}

#endif // RENDERER_HPP
