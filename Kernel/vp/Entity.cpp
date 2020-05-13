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

pixi::vp::Agent::Agent(const Model *model, const short x, const short y)
    : m_model(const_cast<Model*>(model)), m_px(x), m_py(y)
{
    m_type = EntityType::Agent;
    m_symbol = 1;
    m_color = ui::Color::FG_MAGENTA;
}

void pixi::vp::Agent::update(const float deltaTime)
{
    m_model->removeEntity(m_px, m_py);

    m_px += m_vx * deltaTime;
    m_py += m_vy * deltaTime;


    if (m_px < 0.f) {m_px = 0; m_vx = -m_vx;}
    if (m_px >= m_model->w()) {m_px = m_model->w() - 1; m_vx = -m_vx;}
    if (m_py < 0.f) {m_py = 0; m_vy = -m_vy;}
    if (m_py >= m_model->h()) {m_py = m_model->h() - 1; m_vy = -m_vy;}

    if (m_model->get(m_px, m_py) != nullptr) { delete m_model->get(m_px, m_py); m_model->removeEntity(m_px, m_py); }

    m_model->moveEntity(m_px, m_py, this);
}
