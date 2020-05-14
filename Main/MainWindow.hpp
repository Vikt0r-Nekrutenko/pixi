#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "kernel.hpp"

using namespace pixi;

class MainWindow : public ui::Window
{
public:
    MainWindow();
    void update(const float deltaTime) override;
    void secondElased(const float fps) override;
    void onViewClicked(const ui::MouseRecord mer);
private:
    const files::file m_softwareDataFile;
    const files::file m_malwareDataFile;
    vp::Renderer m_view;
    ui::NumericBox m_agentsSpeed;

    ui::OutputBox m_fileNameBox;
    ui::OutputBox m_fileValueBox;
    ui::OutputBox m_fileSizeBox;
    ui::OutputBox m_fileCompanyName;
    ui::OutputBox m_fileVersionName;

    ui::ProgressBox m_agent1InfoBox;
    ui::ProgressBox m_agent2InfoBox;
};

#endif // MAINWINDOW_HPP
