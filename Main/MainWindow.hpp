#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "kernel.hpp"

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
    void onRendererClicked(const ui::MouseRecord mer);
    void onRendererKeyPressed(const ui::KeyRecord ker);
    void onAgentSpeedKeyPressed(const ui::KeyRecord ker);
    void onChangeModeClicked(const ui::MouseRecord mer);
private:
    const files::file m_softwareDataFile;
    const files::file m_malwareDataFile;
    ui::Renderer m_renderer;
    ui::NumericBox m_agentsSpeed;

    ui::OutputBox m_fileNameBox;
    ui::OutputBox m_fileValueBox;
    ui::OutputBox m_fileSizeBox;
    ui::OutputBox m_fileCompanyName;
    ui::OutputBox m_fileVersionName;

    math::dword m_iterations;

    ui::ProgressBox m_agent1InfoBox;
    ui::ProgressBox m_agent2InfoBox;

    std::vector<vp::Agent *> m_agents;
    std::vector<vp::Ware  *> m_wares;

    ui::NumericBox m_agent1Stat;
    ui::NumericBox m_agent2Stat;

    ui::OutputBox m_changeMode;
};

#endif // MAINWINDOW_HPP
