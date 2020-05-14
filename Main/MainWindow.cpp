#include "MainWindow.hpp"

MainWindow::MainWindow()
    : ui::Window(1, 2, 112, 45, "Pixi AIA"),
      m_softwareDataFile("../Kernel/data/SoftwareDataSetRaw.data"),
      m_malwareDataFile("../Kernel/data/MalwareDataSet.data"),
      m_view(this, 0, 25, 112, 20),
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
                      &m_view,
                      &m_agentsSpeed,
                      &m_fileNameBox,
                      &m_fileValueBox,
                      &m_fileSizeBox,
                      &m_fileCompanyName,
                      &m_fileVersionName,
                      &m_agent1InfoBox,
                      &m_agent2InfoBox
                  });
    m_view.mouseEvent = ui::TMouseEventHandler(&MainWindow::onViewClicked);
    m_view.init(&m_softwareDataFile, &m_malwareDataFile, 200, 2);
    m_view.changeAgentSpeed(10);

    m_view.agent(0)->setProgressBox(m_agent1InfoBox);
    m_view.agent(1)->setProgressBox(m_agent2InfoBox);
}

void MainWindow::update(const float deltaTime)
{
    if (m_view.isEnable()) {
        m_view.fill();
        m_view.update(deltaTime);
        m_view.show();
    }
}

void MainWindow::secondElased(const float fps)
{

}

void MainWindow::onViewClicked(const ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == ui::MouseButton::LEFT) {
        if (m_view.get(mer.position.X, mer.position.Y) != nullptr && m_view.get(mer.position.X, mer.position.Y)->type() != vp::EntityType::Agent) {
            vp::Ware *file = (vp::Ware *)m_view.get(mer.position.X, mer.position.Y);
            m_fileNameBox.setText(file->name());
            m_fileValueBox.setText(file->value());
            m_fileSizeBox.setText(file->size() + " bytes");
            m_fileCompanyName.setText(file->company());
            m_fileVersionName.setText(file->version());
        }
    }
}
