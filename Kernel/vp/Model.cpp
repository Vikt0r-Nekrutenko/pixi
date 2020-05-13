#include "Model.hpp"

pixi::vp::Model::Model(const pixi::vp::Renderer *renderer, const files::file *softwareDataFile, const files::file *malwareDataFile)
    : m_renderer(const_cast<Renderer *>(renderer)),
      m_size({ renderer->w(), renderer->h() })
{
    m_entities.resize(m_renderer->w() * m_renderer->h(), nullptr);

    spawnEntities(softwareDataFile, malwareDataFile);

    for (int i = 0; i < 4; i++) {
        int agentPos = agentPos = rand() % m_entities.size();
        while (m_entities[agentPos] != nullptr) {
            agentPos = rand() % m_entities.size();
        }
        m_agents.push_back(new Agent(this, agentPos % m_size.X, agentPos / m_size.X, 5.f, 5.f));
        m_entities.at(agentPos) = m_agents.back();
    }
}

pixi::vp::Model::~Model()
{
    for (int i = m_entities.size() - 1; i >= 0; --i) {
        if (m_entities[i] != nullptr) {
            delete m_entities[i];
        }
    }
}

void pixi::vp::Model::spawnEntities(const files::file *softwareDataFile, const files::file *malwareDataFile)
{
    for (int i = m_entities.size() - 1; i >= 0; --i) {
        int variant = rand() % 10;
        if (variant == 0 && m_entities[i] == nullptr) m_entities[i] = rand() % 2 == 0 ?
                                                                            (Ware*)new Software(softwareDataFile, rand() % 10) :
                                                                            (Ware*)new Malware(malwareDataFile,   rand() % 20);
    }
}

void pixi::vp::Model::removeEntity(const short x, const short y)
{
    m_entities[m_size.X * y + x] = nullptr;
}

void pixi::vp::Model::moveEntity(const short x, const short y, Entity *ent)
{
    assert(x >= 0 && x < m_size.X && y >= 0 && y < m_size.Y);
    assert(m_entities[m_size.X * y + x] == nullptr);
    m_entities[m_size.X * y + x] = ent;
}

void pixi::vp::Model::update(const float deltaTime)
{
    for (Agent *a : m_agents) {
        a->update(deltaTime);
        for (Agent *t : m_agents) {
            if (a != t) {
                float distance = sqrtf(powf(a->px() - t->px(), 2.f) + powf(a->py() - t->py(), 2.f));
                if (distance <= 5.f) {
                    std::swap(a->m_vx, t->m_vx);
                    std::swap(a->m_vy, t->m_vy);
                }
            }
        }
    }
}
