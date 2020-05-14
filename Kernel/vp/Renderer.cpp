#include "Renderer.hpp"
#include "Entity.hpp"
#include "math/vector.hpp"

pixi::vp::Renderer::Renderer(const pixi::ui::Window *parent, const short x, const short y, const short width, const short height)
    : pixi::ui::Box(parent, x, y, width, height, "")
{}

pixi::vp::Renderer::~Renderer()
{
    for (Ware *e : m_entities) {
        delete e;
    }
    for (Agent *a : m_agents) {
        delete a;
    }
}

pixi::vp::Entity *pixi::vp::Renderer::at(const short x, const short y)
{
    for (Ware *e : m_entities) {
        if (e->px() + 1 == x && e->py() + 1 == y && !e->isDestroyed()) {
            return e;
        }
    }
    for (Agent *a : m_agents) {
        if (a->px() + 1 == x && a->py() + 1 == y) {
            return a;
        }
    }
    return nullptr;
}

pixi::vp::Entity *pixi::vp::Renderer::get(const short x, const short y)
{
    short _x = x - m_frame.Left;
    short _y = y - m_frame.Top;

    for (Ware *e : m_entities) {
        if (e->px() + 1 == _x && e->py() + 1 == _y && !e->isDestroyed()) {
            return e;
        }
    }
    for (Agent *a : m_agents) {
        if (a->px() + 1 == x && a->py() + 1 == y) {
            return a;
        }
    }
    return nullptr;
}

void pixi::vp::Renderer::fill(const pixi::ui::Color color, const char symbol)
{
    for (int i = m_size.Y - 3; i >= 0; --i) {
        for (int j = m_size.X - 3; j >= 0; --j) {
            Box::get(j + 1, i + 1).Char.AsciiChar = symbol;
            Box::get(j + 1, i + 1).Attributes = unsigned(color);
        }
    }
}

void pixi::vp::Renderer::update(const float deltaTime)
{
    for (Ware *e : m_entities) {
        assert(e->px() < w() && e->py() < h());
        if (!e->isDestroyed()) {
            Box::get(e->px() + 1, e->py() + 1).Char.AsciiChar = e->symbol();
            Box::get(e->px() + 1, e->py() + 1).Attributes = Entity::dword(e->color());
        }
    }
    for (Agent *a : m_agents) {
        for (Agent *t : m_agents) {
            if (a != t) {
                float distance = sqrtf(powf(a->px() - t->px(), 2.f) + powf(a->py() - t->py(), 2.f));
                while (distance <= 1.f) {
                    a->collision(t, deltaTime);
                    distance = sqrtf(powf(a->px() - t->px(), 2.f) + powf(a->py() - t->py(), 2.f));
                }
            }
        }
        a->update(this, deltaTime);

        Box::get(a->px() + 1, a->py() + 1).Char.AsciiChar = a->symbol();
        Box::get(a->px() + 1, a->py() + 1).Attributes = Entity::dword(a->color());
    }
}

void pixi::vp::Renderer::changeAgentSpeed(const float speed)
{
    for (Agent *a : m_agents) {
        a->setSpeed(speed);
    }
}

void pixi::vp::Renderer::init(const pixi::files::file *software, const pixi::files::file *malware, const int wares, const int agents)
{
    for (int i = 0; i < wares; i++) {
        short x = rand() % w();
        short y = rand() % h();
        if (at(x, y) != nullptr) i--;
        else m_entities.push_back(rand() % 2 == 0 ? (Ware *)new Software(software, rand() % 20, x, y) :
                                                    (Ware *)new Malware(malware,   rand() % 20, x, y));
    }
    for (int i = 0; i < agents; i++) {
        short x = 2 + rand() % (w() - 2);
        short y = 2 + rand() % (h() - 2);
        if (at(x, y) != nullptr) i--;
        else m_agents.push_back(new Agent(x, y, 1.f));
    }
}

void pixi::vp::Renderer::spawnEntities(const int spawnRate)
{
    for (Ware *e : m_entities) {
        if (e->isDestroyed()) {
            if (rand() % 100 < spawnRate) {
                short x = rand() % w();
                short y = rand() % h();
                while (at(x, y) != nullptr) {
                    x = rand() % w();
                    y = rand() % h();
                }
                e->m_indx = rand() % 20;
                e->m_px = x;
                e->m_py = y;
                e->m_isDestroyed = false;
            }
        }
    }
}
