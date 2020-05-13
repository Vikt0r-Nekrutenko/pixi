#include "Entity.hpp"
#include "Model.hpp"

pixi::vp::Ware::Ware(const pixi::files::file *dataFile, const dword indx, const EntityType type)
    : m_dataFile(const_cast<files::file *>(dataFile)),
      m_indx(indx)
{

}

std::string pixi::vp::Ware::name() const
{
    return m_dataFile->field(m_indx, 0);
}

std::string pixi::vp::Ware::value() const
{
    return m_dataFile->field(m_indx, 1);
}

std::string pixi::vp::Ware::size() const
{
    return m_dataFile->field(m_indx, 2);
}

std::string pixi::vp::Ware::company() const
{
    return m_dataFile->field(m_indx, 3);
}

std::string pixi::vp::Ware::version() const
{
    return m_dataFile->field(m_indx, 4);
}

pixi::vp::Malware::Malware(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx)
    : Ware(dataFile, indx, EntityType::Malware)
{
    m_symbol = '#';
    m_color = ui::Color::FG_RED;
}

pixi::vp::Software::Software(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx)
    : Ware(dataFile, indx, EntityType::Software)
{
    m_symbol = '*';
    m_color = ui::Color::FG_GREEN;
}

pixi::vp::Agent::Agent(const Model *model, const short x, const short y, const float vx, const float vy)
    : m_model(const_cast<Model*>(model)), m_px(x), m_py(y), m_vx(vx), m_vy(vy)
{
    m_type = EntityType::Agent;
    m_symbol = 1;
    m_color = ui::Color::FG_MAGENTA;
}

void pixi::vp::Agent::update(const float deltaTime)
{
    m_model->removeEntity(m_px, m_py);

//    if (m_targetFound == false) {
        m_targetPos = findNearestTarget();
//        m_targetFound = true;
//    }
    int targetAngle = atan2f(m_targetPos.second - m_py, m_targetPos.first - m_px) + atanf(45.f);

    m_vx = 10.f * sinf(targetAngle);
    m_vy = 10.f * -cosf(targetAngle);

    m_px += m_vx * deltaTime;
    m_py += m_vy * deltaTime;

    if (m_px < 0.f) {m_px = 0; m_vx = -m_vx;}
    if (m_px >= m_model->w()) {m_px = m_model->w() - 1; m_vx = -m_vx;}
    if (m_py < 0.f) {m_py = 0; m_vy = -m_vy;}
    if (m_py >= m_model->h()) {m_py = m_model->h() - 1; m_vy = -m_vy;}

    if (m_model->get(m_px, m_py) != nullptr) {
        if (m_model->get(m_px, m_py)->type() == EntityType::Agent) {
            m_px -= m_vx * deltaTime;
            m_py -= m_vy * deltaTime;
        } else {
            delete m_model->get(m_px, m_py);
            m_model->removeEntity(m_px, m_py);
            m_targetPos = { 0, 0 };
            m_targetFound = false;
        }
    }

    m_model->moveEntity(m_px, m_py, this);
}

std::pair<int, int> pixi::vp::Agent::findNearestTarget()
{
    float min = INFINITE;
    int tx = 0.f, ty = 0.f;

    for (int y = m_model->h() - 1; y >= 0; --y) {
        for (int x = m_model->w() - 1; x >= 0; --x) {
            Entity *tptr = m_model->get(x, y);
            if (tptr != nullptr &&
//                !tptr->isMarked() &&
                tptr->type() != EntityType::Agent){
                float distance = sqrtf(powf(m_px - x, 2.f) + powf(m_py - y, 2.f));
                if (distance < min) {
                    min = distance;
                    tx = x;
                    ty = y;
                }
            }
        }
    }
    if (m_model->get(tx, ty) != nullptr) {
        m_model->get(tx, ty)->mark();
    }
    return { tx, ty };
}
