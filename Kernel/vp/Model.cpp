#include "Model.hpp"

pixi::vp::Model::Model(const pixi::vp::Renderer *renderer, const files::file *softwareDataFile, const files::file *malwareDataFile)
    : m_renderer(const_cast<Renderer *>(renderer)),
      m_size({ renderer->w(), renderer->h() })
{
    m_entities.resize(m_renderer->w() * m_renderer->h(), nullptr);

    for (int i = m_entities.size() - 1; i >= 0; --i) {
        int variant = rand() % 50;
        if (variant == 0) m_entities[i] = rand() % 2 == 0 ?
                    (Entity*)new Software(softwareDataFile, rand() % 10) :
                    (Entity*)new Malware(malwareDataFile, rand() % 20);
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
