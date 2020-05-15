#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "ui/Box.hpp"
#include "math/vector.hpp"
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
    inline int px() const
    {
        return m_px;
    }
    inline int py() const
    {
        return m_py;
    }
    inline bool isDestroyed() const
    {
        return m_isDestroyed;
    }
    inline void destroy()
    {
        m_isDestroyed = true;
    }
    inline void setPosition(const short x, const short y)
    {
        m_px = x; m_py = y;
    }
    inline void respawn()
    {
        m_isDestroyed = false;
    }
protected:
    EntityType  m_type;
    ui::Color   m_color;
    float       m_px = 0.f, m_py = 0.f;
    bool        m_isDestroyed = false;
    char        m_symbol;
};

class KERNEL_EXPORT Ware : public Entity
{
    friend class Renderer;
public:
    Ware(const files::file *dataFile, const dword indx, const EntityType type);
    std::string name() const;
    std::string value() const;
    std::string size() const;
    std::string company() const;
    std::string version() const;
    math::vector<> signature() const;
protected:
    files::file *m_dataFile = nullptr;
    dword     m_indx = 0u;
};

class KERNEL_EXPORT Malware : public Ware
{
public:
    Malware(const files::file *dataFile, const dword indx, const short x, const short y);
};

class KERNEL_EXPORT Software : public Ware
{
public:
    Software(const files::file *dataFile, const dword indx, const short x, const short y);
};

}}

#endif // ENTITY_HPP
