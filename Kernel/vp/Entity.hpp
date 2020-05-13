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
    inline void mark()
    {
        m_isMarked = true;
    }
    inline bool isMarked() const
    {
        return m_isMarked;
    }
protected:
    EntityType m_type;
    char      m_symbol;
    ui::Color m_color;
    bool      m_isMarked = false;
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
    Agent(const Model *model, const short x, const short y, const float vx = 1.f, const float vy = 1.f);
    inline int px() const
    {
        return m_px;
    }
    inline int py() const
    {
        return m_py;
    }
    void update(const float deltaTime);
    std::pair<int, int> findNearestTarget();
//private:
    Model *m_model = nullptr;
    float m_px, m_py, m_vx = 0.f, m_vy = 0.f;
    std::pair<int, int> m_targetPos;
    bool m_targetFound = false;
};

}}

#endif // ENTITY_HPP
