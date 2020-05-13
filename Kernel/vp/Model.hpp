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
    inline short w() const
    {
        return m_size.X;
    }
    inline short h() const
    {
        return m_size.Y;
    }
    inline Entity *get(const short x, const short y)
    {
        assert(x < m_size.X && y < m_size.Y);
        return m_entities[m_size.X * y + x];
    }
    void spawnEntities(const files::file *softwareDataFile, const files::file *malwareDataFile);
    void removeEntity(const short x, const short y);
    void moveEntity(const short x, const short y, Entity *ent);
    void update(const float deltaTime);

private:
    std::vector<Entity *> m_entities;
    std::vector<Agent *> m_agents;
    Renderer *m_renderer = nullptr;
    COORD m_size { 0, 0 };
};

}}
#endif // MODEL_HPP
