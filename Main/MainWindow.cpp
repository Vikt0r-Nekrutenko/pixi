#include "MainWindow.hpp"

MainWindow::MainWindow()
    : ui::Window(1, 2, 112, 45, "Pixi AIA"),
      m_softwareDataFile("../Kernel/data/SoftwareDataSetRaw.data"),
      m_malwareDataFile("../Kernel/data/MalwareDataSet.data"),
      m_renderer(this, 0, 25, 112, 20),
      m_agentsSpeed(this, 0, 0, 20, 3, "Agents speed:"),
      m_fileNameBox(this, 82, 0, 30, 3, "Description:"),
      m_fileValueBox(this, 82, 3, 30, 3, "Name:"),
      m_fileSizeBox(this, 82, 6, 30, 3, "Size:"),
      m_fileCompanyName(this, 82, 9, 30, 3, "Company:"),
      m_fileVersionName(this, 82, 12, 30, 3, "Version:"),
      m_iterations(100u),
      m_agent1InfoBox(this, 0, 3, 20, 3, m_iterations, ui::Color::FG_RED, "Agent 1 info:"),
      m_agent2InfoBox(this, 0, 6, 20, 3, m_iterations, ui::Color::FG_RED, "Agent 2 info:"),
      m_agent1Stat(this, 20, 3, 20),
      m_agent2Stat(this, 20, 6, 20),
      m_changeMode(this, 0, 9)
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
                      &m_agent2InfoBox,
                      &m_agent1Stat,
                      &m_agent2Stat,
                      &m_changeMode
                  });
    m_renderer.mouseEvent = ui::TMouseEventHandler(&MainWindow::onRendererClicked);
    m_renderer.keyEvent   = ui::TKeyEventHandler(&MainWindow::onRendererKeyPressed);
    m_agentsSpeed.keyEvent= ui::TKeyEventHandler(&MainWindow::onAgentSpeedKeyPressed);
    m_changeMode.keyEvent = ui::TMouseEventHandler(&MainWindow::onChangeModeClicked);

    m_agentsSpeed.setNumber(5.0);
    m_changeMode.setText("Change mode");

    for (int i = 0; i < 10; i++)
        m_wares.push_back(new vp::Software(&m_softwareDataFile, rand() % m_iterations, rand() % m_renderer.w(), rand() % m_renderer.h()));
    for (int i = 0; i < 10; i++)
        m_wares.push_back(new vp::Malware(&m_malwareDataFile, rand() % m_iterations, rand() % m_renderer.w(), rand() % m_renderer.h()));

    m_agents.push_back(new vp::Agent(rand() % m_renderer.w(), rand() % m_renderer.h(), 5.f, &m_agent1InfoBox, m_iterations));
    m_agents.push_back(new vp::Agent(rand() % m_renderer.w(), rand() % m_renderer.h(), 5.f, &m_agent2InfoBox, m_iterations));
}

MainWindow::~MainWindow()
{
    for (vp::Ware *w : m_wares) delete w;
    for (vp::Agent *a: m_agents) delete a;
}

vp::Entity *MainWindow::getEntityOnRenderer(const short x, const short y)
{
    short _x = x - m_renderer.getFrame().Left;
    short _y = y - m_renderer.getFrame().Top;

    for (vp::Ware  *w : m_wares)  if (w->px() + 1 == _x && w->py() + 1 == _y) return w;
    for (vp::Agent *a : m_agents) if (a->px() + 1 == _x && a->py() + 1 == _y) return a;

    return nullptr;
}

void MainWindow::respawnWares()
{
    for (vp::Ware *w : m_wares) {
        if (w->isDestroyed()) {
            w->setPosition(rand() % m_renderer.w(), rand() % m_renderer.h());
            w->respawn();
        }
    }
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
    respawnWares();
    m_agent1Stat.setNumber(m_agents.at(0)->getDetector().statistic(m_softwareDataFile, m_malwareDataFile, m_iterations));
    m_agent2Stat.setNumber(m_agents.at(1)->getDetector().statistic(m_softwareDataFile, m_malwareDataFile, m_iterations));
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

void MainWindow::onRendererKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_SPACE) {
        if (m_renderer.isEnable()) m_renderer.disable();
        else m_renderer.enable();
    }
}

void MainWindow::onAgentSpeedKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        for (vp::Agent *a : m_agents) {
            a->setSpeed(m_agentsSpeed.getNumber());
        }
    }
}

void MainWindow::onChangeModeClicked(const ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == ui::MouseButton::LEFT) {
        for (vp::Agent *a : m_agents) {
            a->changeMode();
        }
    }
}
