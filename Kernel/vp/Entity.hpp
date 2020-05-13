#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "ui/Box.hpp"
#include "file.hpp"

namespace pixi { namespace vp {

enum class EntityType {
    Malware, Software, Agent
};

class KERNEL_EXPORT Entity
{
public:
    using dword = unsigned;
    inline EntityType type() const
    {
        return m_type;
    }
    inline char symbol() const
    {
        return m_symbol;
    }
    inline ui::Color color() const
    {
        return m_color;
    }
protected:
    EntityType m_type;
    char      m_symbol;
    ui::Color m_color;
};

class KERNEL_EXPORT Ware : public Entity
{
public:
    Ware(const files::file *dataFile, const dword indx, const EntityType type);
    std::string name() const;
    std::string value() const;
    std::string size() const;
    std::string company() const;
    std::string version() const;
protected:
    files::file *m_dataFile = nullptr;
    dword     m_indx = 0u;
};

class KERNEL_EXPORT Malware : public Ware
{
public:
    Malware(const files::file *dataFile, const dword indx);
};

class KERNEL_EXPORT Software : public Ware
{
public:
    Software(const files::file *dataFile, const dword indx);
};

class Model;

class KERNEL_EXPORT Agent : public Entity
{
public:
    Agent(const Model *model, const short x, const short y);
    inline int px() const
    {
        return m_px;
    }
    inline int py() const
    {
        return m_py;
    }
    void update(const float deltaTime);
private:
    Model *m_model = nullptr;
    float m_px, m_py, m_vx = 10.f, m_vy = 10.f;
};

}}

#endif // ENTITY_HPP
