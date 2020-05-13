#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "ui/Box.hpp"
#include "file.hpp"

namespace pixi { namespace vp {

class KERNEL_EXPORT Entity
{
public:
    using dword = unsigned;

    Entity(const files::file *dataFile, const dword indx);
    inline char symbol() const
    {
        return m_symbol;
    }
    inline ui::Color color() const
    {
        return m_color;
    }
    std::string name() const;
    std::string value() const;
    std::string size() const;
    std::string company() const;
    std::string version() const;
protected:

    files::file *m_dataFile = nullptr;
    dword     m_indx = 0u;
    char      m_symbol;
    ui::Color m_color;
};

class KERNEL_EXPORT Malware : public Entity
{
public:
    Malware(const files::file *dataFile, const dword indx);
};

class KERNEL_EXPORT Software : public Entity
{
public:
    Software(const files::file *dataFile, const dword indx);
};

}}

#endif // ENTITY_HPP
