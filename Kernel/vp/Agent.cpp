#include "Agent.hpp"

pixi::vp::Agent::Agent(const short x, const short y, const float speed, ui::ProgressBox *view, const int lifeCicles)
    : Entity(),
      m_lifeCicle(lifeCicles),
      m_layers(2 + rand() % 254),
      m_detector(16, m_layers, m_layers, 2, m_lifeCicle),
      m_view(view),
      m_speed(speed)
{
    m_symbol = 2;
    m_color = ui::Color::FG_MAGENTA;
    m_type = EntityType::Agent;
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
    float distance = distanceTo(target);
    int i = 100;
    std::swap(m_vx, target->m_vx);
    std::swap(m_vy, target->m_vy);

    while (distance <= 2.f || --i) {
        m_px += m_vx * deltaTime;
        m_py += m_vy * deltaTime;
        distance = distanceTo(target);
    }
}

void pixi::vp::Agent::operation(pixi::vp::Ware *ware)
{
    if (m_mode == AgentMode::LEARNIGN) {
        if (ware->type() == EntityType::Software) m_detector.learn_step(ware->signature(), math::vector<>(16.f, -1.f));
        if (ware->type() == EntityType::Malware)  m_detector.learn_step(ware->signature(), math::vector<>(16.f, +1.f));
    } else {
        math::vector<> res = m_detector.result(ware->signature());
        if (res[0] > 0.f) {
            ware->destroy();
            m_view->increase();
        }
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
                if (m_mode == AgentMode::LEARNIGN) {
                    min = distance;
                    target = t;
                } else if (m_mode == AgentMode::NORMAL && t->type() == EntityType::Malware) {
                    min = distance;
                    target = t;
                }
            }
            if (distance < 1.2f) {
                if (m_mode == AgentMode::LEARNIGN || (m_mode == AgentMode::NORMAL && t->type() == EntityType::Malware)) {
                    t->destroy();
                    target = nullptr;
                    min = INFINITE;
                    if (--m_lifeCicle == 0) {
//                        destroy();

                        m_color = ui::Color::FG_YELLOW;
                        m_lifeCicle = 100;
                        m_layers = 2 + rand() % 254;
                        m_detector = math::detector(16, m_layers, m_layers, 2, m_lifeCicle);
                    }
                    m_view->increase();
                }
                operation(t);
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

