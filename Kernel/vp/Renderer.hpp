#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cassert>
#include <vector>
#include "ui/Box.hpp"
#include "Entity.hpp"

namespace pixi { namespace vp {

class KERNEL_EXPORT Renderer : public pixi::ui::Box
{
    friend KERNEL_EXPORT class Agent;
public:
    Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height);
    ~Renderer() override;
    Entity *at(const short x, const short y);
    Entity *get(const short x, const short y);
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
    void fill(const pixi::ui::Color color = pixi::ui::Color::FG_BLACK, const char symbol = ui::UNDEF_SYMBOL) final;
    void update(const float deltaTile);
    void changeAgentSpeed(const float speed);
    void init(const files::file *software, const files::file *malware, const int wares, const int agents);
    void spawnEntities(const int spawnRate = 100);
private:
    std::vector<Ware *> m_entities;
    std::vector<Agent *> m_agents;
    bool m_isEnable = true;
};

}}

#endif // RENDERER_HPP
