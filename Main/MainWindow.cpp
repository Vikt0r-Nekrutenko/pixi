#include "MainWindow.hpp"
#include <thread>
#include <string>
#include <sstream>

constexpr short FILE_INFO_BOX_POS   = 80;
constexpr short AGENT_INFO_BOX_POS  = 56;
constexpr short BEST_INFO_BOX_POS   = 32;

MainWindow::MainWindow()
    : ui::Window(1, 2, 112, 45, "Pixi AIA"),
//    : ui::Window(true),
      m_softwareDataFile("../Kernel/data/SoftwareDataSetRaw.data"),
      m_malwareDataFile("../Kernel/data/MalwareDataSet.data"),

      m_renderer(this, 0, 22,    112, 20),

      m_controlPanelBox(    this, 0, 0,  30, 18, "Control panel:"),
      m_agentsSpeedBox(     this, 1, 2,  28, 3, "Agents speed:"),
      m_softwareNumsBox(    this, 1, 5,  28, 3, "Software samples:"),
      m_malwareNumsBox(     this, 1, 8,  28, 3, "Malware samples:"),
      m_trainingSampleBox(  this, 1, 11, 28, 3, "Training samples:"),
      m_lifesBox(           this, 1 ,14, 28, 3, "Life cycles box:"),

      m_fileInfoBox(    this, FILE_INFO_BOX_POS,   0,    32, 18,     "File info box:"),
      m_fileNameBox(    this, FILE_INFO_BOX_POS+1, 2,    30, 3,      "Description:"),
      m_fileValueBox(   this, FILE_INFO_BOX_POS+1, 5,    30, 3,      "Name:"),
      m_fileSizeBox(    this, FILE_INFO_BOX_POS+1, 8,    30, 3,      "Size:"),
      m_fileCompanyName(this, FILE_INFO_BOX_POS+1, 11,   30, 3,      "Company:"),
      m_fileVersionName(this, FILE_INFO_BOX_POS+1, 14,   30, 3,      "Version:"),


      m_agentInfoBox(   this, AGENT_INFO_BOX_POS,   0,    22, 18,     "Agent info box:"),
      m_agentGeneration(this, AGENT_INFO_BOX_POS+1, 2,    20, 3,      "Generation:"),
      m_agentLayers(    this, AGENT_INFO_BOX_POS+1, 5,    20, 3,      "Layers:"),
      m_agentIterations(this, AGENT_INFO_BOX_POS+1, 8,    20, 3,      "Iterations:"),
      m_agentLifeCycle( this, AGENT_INFO_BOX_POS+1, 11,   20, 3,      "Life cycle:"),
      m_agentErrors(    this, AGENT_INFO_BOX_POS+1, 14,   20, 3,      "Error:"),

      m_bestAgentInfoBox(   this, BEST_INFO_BOX_POS,   0,    22, 18, "Best info:"),
      m_bestAgentGeneration(this, BEST_INFO_BOX_POS+1, 2,    20, 3,  "Best generation:"),
      m_bestAgentLayers(    this, BEST_INFO_BOX_POS+1, 5,    20, 3,  "Best layers:"),
      m_bestAgentIterations(this, BEST_INFO_BOX_POS+1, 8,    20, 3,  "Best iterations:"),
      m_bestAgentLifeCycle( this, BEST_INFO_BOX_POS+1, 11,   20, 3,  "Best life cycle:"),
      m_bestTotalError(     this, BEST_INFO_BOX_POS+1, 14,   20, 3,  "Best error:"),

      m_totalGenerations(   this, 0,   42, 112, 3, 1000, ui::Color::FG_RED, "Elapsed generations:"),

      m_learningMode(    this, 0,    19, 54, 3, ""),
      m_normalMode(      this, 56,   19, 56, 3, "")
{
    m_fileInfoBox.show();
    m_agentInfoBox.show();
    m_bestAgentInfoBox.show();
    m_controlPanelBox.show();

    addNewControl({
                      &m_renderer,
                      &m_agentsSpeedBox,
                      &m_softwareNumsBox,
                      &m_malwareNumsBox,
                      &m_trainingSampleBox,
                      &m_lifesBox,

                      &m_fileNameBox,
                      &m_fileValueBox,
                      &m_fileSizeBox,
                      &m_fileCompanyName,
                      &m_fileVersionName,

                      &m_agentGeneration,
                      &m_agentLayers,
                      &m_agentIterations,
                      &m_agentLifeCycle,
                      &m_agentErrors,

                      &m_bestAgentGeneration,
                      &m_bestAgentLayers,
                      &m_bestAgentIterations,
                      &m_bestAgentLifeCycle,
                      &m_bestTotalError,

                      &m_totalGenerations,

                      &m_learningMode,
                      &m_normalMode
                  });
    m_softwareNumsBox.keyEvent  = ui::TKeyEventHandler(&MainWindow::onSoftwareNumsKeyPressed);
    m_malwareNumsBox.keyEvent   = ui::TKeyEventHandler(&MainWindow::onMalwareNumsKeyPressed);
    m_trainingSampleBox.keyEvent= ui::TKeyEventHandler(&MainWindow::onTrainingSampleKeyPressed);
    m_lifesBox.keyEvent         = ui::TKeyEventHandler(&MainWindow::onLifeCyclesKeyPressed);

    m_renderer.mouseEvent       = ui::TMouseEventHandler(&MainWindow::onRendererClicked);
    m_renderer.keyEvent         = ui::TKeyEventHandler(&MainWindow::onRendererKeyPressed);
    m_agentsSpeedBox.keyEvent   = ui::TKeyEventHandler(&MainWindow::onAgentSpeedKeyPressed);

    m_learningMode.mouseEvent  = ui::TMouseEventHandler(&MainWindow::onLearningModeClicked);
    m_normalMode.mouseEvent    = ui::TMouseEventHandler(&MainWindow::onNormalModeClicked);

    m_agentsSpeedBox.setNumber(5.0);
    m_softwareNumsBox.setNumber(50.0);
    m_malwareNumsBox.setNumber(50.0);
    m_trainingSampleBox.setNumber(20000);
    m_lifesBox.setNumber(30.0);

    m_learningMode.setText("                  LEARNING MODE");
    m_normalMode.setText("                     NORMAL MODE");

    m_renderer.disable();
    setCursorParams();
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

    if (m_renderer.isEnable() && !m_wares.empty() && !m_agents.empty()) {
        m_totalGenerations.increase();
        startLearning();
    }
}

void MainWindow::startLearning()
{
//    std::thread t([&]{
//        std::srand(std::time(nullptr));

        math::dword lw = 2 + rand() % 256;
        math::dword li = 1000 + rand() % m_trainingSample;

        math::detector det1(16, lw, lw, 2, li);
        det1.learn(m_softwareDataFile, m_malwareDataFile, m_trainingSample, math::vector<>(16, -1.f), math::vector<>(16, +1.f));

        math::dword err = det1.statistic(m_softwareDataFile, m_malwareDataFile, m_trainingSample);
        m_totalGenerations.increase();
        if (err < m_minError) {
            m_mtx.lock();
            m_minError = err;

            m_bestAgentLayers.setNumber(lw);
            m_bestAgentIterations.setNumber(li);
            std::stringstream stream; stream << std::this_thread::get_id();
            m_bestAgentGeneration.setText(stream.str().c_str());
            m_bestTotalError.setNumber(m_minError);
            m_bestAgentLifeCycle.setNumber(100.0);
            m_mtx.unlock();
        }
//    }); t.detach();
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
        } else if (entity != nullptr && entity->type() == vp::EntityType::Agent) {
            vp::Agent *agent = (vp::Agent *)entity;

            m_agentGeneration.setNumber(agent->generation());
            m_agentLayers.setNumber(agent->layers());
            m_agentIterations.setNumber(agent->iterations());
            m_agentLifeCycle.setNumber(agent->lifeCicle());
            m_agentErrors.setNumber(agent->errors());
        }
    }
}

void MainWindow::onRendererKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_SPACE) {
        if (!m_agents.empty()) {
            if (m_renderer.isEnable()) m_renderer.disable();
            else m_renderer.enable();
        }
    }
}

void MainWindow::onAgentSpeedKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        for (vp::Agent *a : m_agents) {
            a->setSpeed(m_agentsSpeedBox.getNumber());
        }
    }
}

void MainWindow::onSoftwareNumsKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        for (int i = 0; i < m_softwareNumsBox.getNumber(); i++)
            m_wares.push_back(new vp::Software(&m_softwareDataFile, rand() % m_trainingSample, rand() % m_renderer.w(), rand() % m_renderer.h()));
    }
}

void MainWindow::onMalwareNumsKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        for (int i = 0; i < m_malwareNumsBox.getNumber(); i++)
            m_wares.push_back(new vp::Malware(&m_malwareDataFile, rand() % m_trainingSample, rand() % m_renderer.w(), rand() % m_renderer.h()));
    }
}

void MainWindow::onTrainingSampleKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        m_trainingSample = m_trainingSampleBox.getNumber();
    }
}

void MainWindow::onLifeCyclesKeyPressed(const ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN) {
        for (int i = 0; i < 10; i++)
            m_agents.push_back(new vp::Agent(rand() % m_renderer.w(), rand() % m_renderer.h(), 5.f, m_lifesBox.getNumber()));
        m_renderer.enable();
    }
}

void MainWindow::onLearningModeClicked(const ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == ui::MouseButton::LEFT) {
        m_normalMode.fill(m_normalMode.color());    m_normalMode.show();
        m_learningMode.fill(ui::Color::FG_GREEN);   m_learningMode.show();
        for (vp::Agent *a : m_agents) {
            a->enableLearnigMode();
        }
    }
}

void MainWindow::onNormalModeClicked(const ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == ui::MouseButton::LEFT) {
        m_learningMode.fill(m_learningMode.color()); m_learningMode.show();
        m_normalMode.fill(ui::Color::FG_GREEN);      m_normalMode.show();
        for (vp::Agent *a : m_agents) {
            a->enableNormalMode();
        }
    }
}
