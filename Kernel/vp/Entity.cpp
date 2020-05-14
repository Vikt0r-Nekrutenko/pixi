#include "Entity.hpp"

pixi::vp::Ware::Ware(const pixi::files::file *dataFile, const dword indx, const EntityType type)
    : m_dataFile(const_cast<files::file *>(dataFile)),
      m_indx(indx)
{
    m_type = type;
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

pixi::vp::Malware::Malware(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx, const short x, const short y)
    : Ware(dataFile, indx, EntityType::Malware)
{
    m_symbol = 15;
    m_color = ui::Color::FG_RED;
    m_px = x;
    m_py = y;
}

pixi::vp::Software::Software(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx, const short x, const short y)
    : Ware(dataFile, indx, EntityType::Software)
{
    m_symbol = 7;
    m_color = ui::Color::FG_GREEN;
    m_px = x;
    m_py = y;
}
