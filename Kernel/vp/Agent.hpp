#ifndef AGENT_HPP
#define AGENT_HPP

#include "Entity.hpp"

namespace pixi { namespace vp {

class Renderer;

class KERNEL_EXPORT Agent : public Entity
{
public:
    Agent(const short x, const short y, const float speed, const int lifeCicles = 10);
    inline void setSpeed(const float speed)
    {
        m_speed = speed;
    }
    inline int lifeCicle() const
    {
        return m_lifeCicle;
    }
    inline float distanceTo(Entity *target)
    {
        return sqrtf(powf(m_px - target->px(), 2.f) + powf(m_py - target->py(), 2.f));
    }
    void update(std::vector<Ware *> &wares, const short rightBorder, const short bottomBorder, const float deltaTime);
    void collision(Agent *target, const float deltaTime);
    float findNearestTarget(std::vector<Ware *> wares);
private:
    float   m_vx = -5.f, m_vy = 5.f, m_speed = 1.f;
    bool    m_targetIsFound = false;
    int     m_lifeCicle;
};

}}

#endif // AGENT_HPP
