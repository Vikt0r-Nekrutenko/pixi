#include "Agent.hpp"

pixi::vp::Agent::Agent(const short x, const short y, const float speed, const int lifeCicles)
    : Entity(),
      m_speed(speed),
      m_lifeCicle(lifeCicles)
{
    m_symbol = 2;
    m_color = ui::Color::FG_MAGENTA;
    m_px = x;
    m_py = y;
}

void pixi::vp::Agent::update(std::vector<Ware *> &wares, const short rightBorder, const short bottomBorder, const float deltaTime)
{
    float targetAngle = findNearestTarget(wares);

    if (m_targetIsFound) {
        m_vx = m_speed * sinf(targetAngle);
        m_vy = m_speed * -cosf(targetAngle);
    }

    if (abs(m_vx) < 0.01f && abs(m_vy) < 0.01f) return;

    float ax = -m_vx * 0.8f;
    float ay = -m_vy * 0.8f;

    m_vx += ax * deltaTime;
    m_vy += ay * deltaTime;

    m_px += m_vx * deltaTime;
    m_py += m_vy * deltaTime;


    if (m_px <= 0.f)            { m_px = 1.f; m_vx = -m_vx; }
    if (m_py <= 0.f)            { m_py = 1.f; m_vy = -m_vy; }
    if (m_px > rightBorder - 2) { m_px = rightBorder - 2; m_vx = -m_vx; }
    if (m_py > bottomBorder - 2) { m_py = bottomBorder - 2; m_vy = -m_vy; }
}

void pixi::vp::Agent::collision(pixi::vp::Agent *target, const float deltaTime)
{
    std::swap(m_vx, target->m_vx);
    std::swap(m_vy, target->m_vy);

    float distance = distanceTo(target);
    while (distance <= 2.f) {
        m_px += m_vx * deltaTime;
        m_py += m_vy * deltaTime;
        distance = distanceTo(target);
    }
}

float pixi::vp::Agent::findNearestTarget(std::vector<Ware *> wares)
{
    float min = INFINITE;
    Ware *target = nullptr;
    for (Ware *t : wares) {
        if (!t->isDestroyed()) {
            float distance = distanceTo(t);
            if (distance < min) {
                min = distance;
                target = t;
            }
            if (distance < 1.2f) {
                t->destroy();
                target = nullptr;
                min = INFINITE;
                if (--m_lifeCicle == 0) {
                    destroy();
                }
            }
        }
    }
    if (target != nullptr) {
        m_targetIsFound = true;
        return atan2f(target->py() - m_py, target->px() - m_px) + atanf(45.f);
    } else {
        m_targetIsFound = false;
    }
    return 0.f;
}

