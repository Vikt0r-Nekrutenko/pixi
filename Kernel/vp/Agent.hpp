#ifndef AGENT_HPP
#define AGENT_HPP

#include "Entity.hpp"
#include "ui/ProgressBox.hpp"
#include "math/detector.hpp"

namespace pixi { namespace vp {

enum class AgentMode {
    LEARNIGN,
    NORMAL
};

class Renderer;

class KERNEL_EXPORT Agent : public Entity
{
public:
    Agent(const short x, const short y, const float speed, ui::ProgressBox *view, const int lifeCicles = 10);
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
    inline math::detector &getDetector()
    {
        return m_detector;
    }
    inline void changeMode()
    {
        if (m_mode == AgentMode::LEARNIGN) m_mode = AgentMode::NORMAL;
        else m_mode = AgentMode::LEARNIGN;
    }
    void update(std::vector<Ware *> &wares, const short rightBorder, const short bottomBorder, const float deltaTime);
    void collision(Agent *target, const float deltaTime);
    void operation(Ware *ware);
    float findNearestTarget(std::vector<Ware *> wares);
private:
    AgentMode m_mode = AgentMode::LEARNIGN;
    math::dword m_lifeCicle;
    math::dword m_layers;
    math::detector m_detector;
    ui::ProgressBox *m_view = nullptr;
    float   m_vx = -5.f, m_vy = 5.f, m_speed = 1.f;
    bool    m_targetIsFound = false;
};

}}

#endif // AGENT_HPP
