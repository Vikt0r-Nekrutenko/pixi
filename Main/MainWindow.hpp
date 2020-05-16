#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "kernel.hpp"
#include <mutex>

using namespace pixi;

class MainWindow : public ui::Window
{
public:
    MainWindow();
    ~MainWindow() final;
    vp::Entity* getEntityOnRenderer(const short x, const short y);
    void respawnWares();
    void update(const float deltaTime) override;
    void secondElased(const float fps) override;
    void startLearning();
    void onRendererClicked(const ui::MouseRecord mer);
    void onRendererKeyPressed(const ui::KeyRecord ker);
    void onAgentSpeedKeyPressed(const ui::KeyRecord ker);
    void onSoftwareNumsKeyPressed(const ui::KeyRecord ker);
    void onMalwareNumsKeyPressed(const ui::KeyRecord ker);
    void onTrainingSampleKeyPressed(const ui::KeyRecord ker);
    void onLifeCyclesKeyPressed(const ui::KeyRecord ker);
    void onLearningModeClicked(const ui::MouseRecord mer);
    void onNormalModeClicked(const ui::MouseRecord mer);
private:
    const files::file m_softwareDataFile;
    const files::file m_malwareDataFile;
    ui::Renderer    m_renderer;
    ui::Box         m_controlPanelBox;
    ui::NumericBox  m_agentsSpeedBox;
    ui::NumericBox  m_softwareNumsBox;
    ui::NumericBox  m_malwareNumsBox;
    ui::NumericBox  m_trainingSampleBox;
    ui::NumericBox  m_lifesBox;

    ui::Box       m_fileInfoBox;
    ui::OutputBox m_fileNameBox;
    ui::OutputBox m_fileValueBox;
    ui::OutputBox m_fileSizeBox;
    ui::OutputBox m_fileCompanyName;
    ui::OutputBox m_fileVersionName;

    ui::Box        m_agentInfoBox;
    ui::NumericBox m_agentGeneration;
    ui::NumericBox m_agentLayers;
    ui::NumericBox m_agentIterations;
    ui::NumericBox m_agentLifeCycle;
    ui::NumericBox m_agentErrors;

    ui::Box        m_bestAgentInfoBox;
    ui::OutputBox  m_bestAgentGeneration;
    ui::NumericBox m_bestAgentLayers;
    ui::NumericBox m_bestAgentIterations;
    ui::NumericBox m_bestAgentLifeCycle;
    ui::NumericBox m_bestTotalError;

    math::dword m_minError = INFINITE;
    math::dword m_trainingSample = 20000;

    ui::ProgressBox m_totalGenerations;

    std::vector<vp::Agent *> m_agents;
    std::vector<vp::Ware  *> m_wares;

    ui::OutputBox m_learningMode;
    ui::OutputBox m_normalMode;

    std::mutex m_mtx;
};

#endif // MAINWINDOW_HPP
