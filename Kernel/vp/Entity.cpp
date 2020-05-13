#include "Entity.hpp"

pixi::vp::Entity::Entity(const pixi::files::file *dataFile, const dword indx)
    : m_dataFile(const_cast<files::file *>(dataFile)),
      m_indx(indx)
{

}

std::string pixi::vp::Entity::name() const
{
    return m_dataFile->field(m_indx, 0);
}

std::string pixi::vp::Entity::value() const
{
    return m_dataFile->field(m_indx, 1);
}

std::string pixi::vp::Entity::size() const
{
    return m_dataFile->field(m_indx, 2);
}

std::string pixi::vp::Entity::company() const
{
    return m_dataFile->field(m_indx, 3);
}

std::string pixi::vp::Entity::version() const
{
    return m_dataFile->field(m_indx, 4);
}

pixi::vp::Malware::Malware(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx)
    : Entity(dataFile, indx)
{
    m_symbol = '#';
    m_color = ui::Color::FG_RED;
}

pixi::vp::Software::Software(const pixi::files::file *dataFile, const pixi::vp::Entity::dword indx)
    : Entity(dataFile, indx)
{
    m_symbol = '*';
    m_color = ui::Color::FG_GREEN;
}
