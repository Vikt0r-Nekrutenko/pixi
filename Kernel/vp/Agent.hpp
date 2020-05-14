#ifndef AGENT_HPP
#define AGENT_HPP

#include "Entity.hpp"
#include "ui/ProgressBox.hpp"

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
    inline void setProgressBox(const ui::ProgressBox &pb)
    {
        m_progress = const_cast<ui::ProgressBox *>(&pb);
    }
    inline int lifeCicle() const
    {
        return m_lifeCicle;
    }
    void update(Renderer *renderer, const float deltaTime);
    void collision(Agent *target, const float deltaTime);
    float findNearestTarget(Renderer *renderer);
private:
    ui::ProgressBox *m_progress;
    float m_vx = -5.f, m_vy = 5.f, m_speed = 1.f;
    bool m_targetIsFound = false;
    int m_lifeCicle = 1;
};

}}

#endif // AGENT_HPP
