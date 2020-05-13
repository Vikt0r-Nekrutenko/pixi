#ifndef MODEL_HPP
#define MODEL_HPP

#include "Renderer.hpp"
#include "Entity.hpp"
#include <vector>

namespace pixi { namespace vp {

class KERNEL_EXPORT Model
{
public:
    Model(const Renderer *renderer, const files::file *softwareDataFile, const files::file *malwareDataFile);
    ~Model();
    inline Entity *get(const short x, const short y)
    {
        assert(x < m_size.X && y < m_size.Y);
        return m_entities[m_size.X * y + x];
    }
    inline void addEnt(const short x, const short y, const Entity &ent)
    {
        assert(x < m_size.X && y < m_size.Y);
        m_entities[m_size.X * y + x] = const_cast<Entity *>(&ent);
    }

private:
    std::vector<Entity *> m_entities;
    Renderer *m_renderer = nullptr;
    COORD m_size { 0, 0 };
};

}}
#endif // MODEL_HPP
