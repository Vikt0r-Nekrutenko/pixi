#ifndef AGENT_HPP
#define AGENT_HPP

#include "Entity.hpp"

namespace pixi { namespace vp {

enum class AgentMode {
    LEARNIGN,
    NORMAL
};

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
    inline math::dword generation() const
    {
        return m_generation;
    }
    inline math::dword layers() const
    {
        return 2 + rand() % 254;
    }
    inline math::dword iterations() const
    {
        return 1000 + rand() % 99000;
    }
    inline math::dword errors() const
    {
        return 1000 + rand() % 30000;
    }
    inline float distanceTo(Entity *target)
    {
        return sqrtf(powf(m_px - target->px(), 2.f) + powf(m_py - target->py(), 2.f));
    }
    inline void enableNormalMode()
    {
        m_mode = AgentMode::NORMAL;
    }
    inline void enableLearnigMode()
    {
        m_mode = AgentMode::LEARNIGN;
    }
    void update(std::vector<Ware *> &wares, const short rightBorder, const short bottomBorder, const float deltaTime);
    void collision(Agent *target, const float deltaTime);
    float findNearestTarget(std::vector<Ware *> wares);
private:
    AgentMode m_mode = AgentMode::LEARNIGN;
    math::dword m_lifeCicle;
    math::dword m_generation;
    float   m_vx = -5.f, m_vy = 5.f, m_speed = 1.f;
    bool    m_targetIsFound = false;
};

}}

#endif // AGENT_HPP
