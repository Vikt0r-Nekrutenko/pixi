#include "MainWindow.hpp"

MainWindow::MainWindow()
    : ui::Window(1, 2, 112, 45, "Pixi AIA"),
      m_softwareDataFile("../Kernel/data/SoftwareDataSetRaw.data"),
      m_malwareDataFile("../Kernel/data/MalwareDataSet.data"),
      m_renderer(this, 0, 25, 112, 20),
      m_agentsSpeed(this, 0, 0, 30, 3, "Agents speed:"),
      m_fileNameBox(this, 82, 0, 30, 3, "Description:"),
      m_fileValueBox(this, 82, 3, 30, 3, "Name:"),
      m_fileSizeBox(this, 82, 6, 30, 3, "Size:"),
      m_fileCompanyName(this, 82, 9, 30, 3, "Company:"),
      m_fileVersionName(this, 82, 12, 30, 3, "Version:"),
      m_agent1InfoBox(this, 0, 3, 20, 3, 100, ui::Color::FG_RED, "Agent 1 info:"),
      m_agent2InfoBox(this, 0, 6, 20, 3, 100, ui::Color::FG_RED, "Agent 2 info:")
{
    setFontParams(12, 16, true);

    addNewControl({
                      &m_renderer,
                      &m_agentsSpeed,
                      &m_fileNameBox,
                      &m_fileValueBox,
                      &m_fileSizeBox,
                      &m_fileCompanyName,
                      &m_fileVersionName,
                      &m_agent1InfoBox,
                      &m_agent2InfoBox
                  });
    m_renderer.mouseEvent = ui::TMouseEventHandler(&MainWindow::onRendererClicked);
    for (int i = 0; i < 10; i++) m_wares.push_back(new vp::Software(&m_softwareDataFile, rand() % 20, rand() % m_renderer.w(), rand() % m_renderer.h()));
    for (int i = 0; i < 10; i++) m_wares.push_back(new vp::Malware(&m_malwareDataFile, rand() % 20, rand() % m_renderer.w(), rand() % m_renderer.h()));

    for (int i = 0; i < 2;  i++) m_agents.push_back(new vp::Agent(rand() % m_renderer.w(), rand() % m_renderer.h(), 5.f));
}

vp::Entity *MainWindow::getEntityOnRenderer(const short x, const short y)
{
    short _x = x - m_renderer.getFrame().Left;
    short _y = x - m_renderer.getFrame().Top;

    for (vp::Ware  *w : m_wares)  if (w->px() + 1 == _x && w->py() + 1 == _y) return w;
    for (vp::Agent *a : m_agents) if (a->px() + 1 == _x && a->py() + 1 == _y) return a;

    return nullptr;
}

void MainWindow::update(const float deltaTime)
{
    if (m_renderer.isEnable()) {
        m_renderer.fill();
        m_renderer.update<vp::Ware>(m_wares);
        for (vp::Agent *a : m_agents) {
            if (!a->isDestroyed()) {
                for (vp::Agent *t : m_agents)
                    if (a != t)
                        if (a->distanceTo(t) <= 2.f)
                            a->collision(t, deltaTime);
                a->update(m_wares, m_renderer.w(), m_renderer.h(), deltaTime);
            }
        }
        m_renderer.update<vp::Agent>(m_agents);
        m_renderer.show();
    }
}

void MainWindow::secondElased(const float fps)
{

}

void MainWindow::onRendererClicked(const ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == ui::MouseButton::LEFT) {
        vp::Entity *entity = getEntityOnRenderer(mer.position.X, mer.position.Y);
        if (entity != nullptr && entity->type() != vp::EntityType::Agent) {
            vp::Ware *file = (vp::Ware *)entity;

            m_fileNameBox.setText(file->name());
            m_fileValueBox.setText(file->value());
            m_fileSizeBox.setText(file->size() + " bytes");
            m_fileCompanyName.setText(file->company());
            m_fileVersionName.setText(file->version());
        }
    }
}
