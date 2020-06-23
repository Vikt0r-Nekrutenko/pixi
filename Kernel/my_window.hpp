#ifndef MY_WINDOW_HPP
#define MY_WINDOW_HPP

#include <cassert>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>
#include "math/detector.hpp"
#include "ui/Window.hpp"
#include "ui/OutputBox.hpp"
#include "ui/ProgressBox.hpp"

using namespace pixi::math;

dword random(dword min, dword max) {
    return min + rand() % (max - min);
}

class my_window : public pixi::ui::Window
{
public:
    my_window();
    ~my_window();

    void startLearingBtnOnClicked(const pixi::ui::MouseRecord mer);
    void startLearning(const dword min_it, const dword max_it);
    void save(const pixi::ui::KeyRecord ker);
private:
    pixi::files::file swds, mwds;

    pixi::ui::ProgressBox thread1Output;
    pixi::ui::ProgressBox thread2Output;
    pixi::ui::OutputBox *startLearningBtn;
    pixi::ui::InputBox *saveFileName;

    detector d1;
    std::atomic<dword> minerr;
    std::mutex mtx;
    const dword files_num = 20000;
};

my_window::my_window()
    : pixi::ui::Window(),
      swds("../Kernel/data/SoftwareDataSetRaw.data"),
      mwds("../Kernel/data/MalwareDataSet.data"),
      thread1Output(this, 0, 0, 30, 3, 100, pixi::ui::Color::FG_RED, "Thread 2 INFO"),
      thread2Output(this, 0, 4, 30, 3, 100, pixi::ui::Color::FG_RED, "Thread 3 INFO"),
      startLearningBtn(new pixi::ui::OutputBox(this, 0, 8, 50)),
      saveFileName(new pixi::ui::InputBox(this, 0, 12)),
      d1(4, 4, 4, 4, 4),
      minerr(0xffffffff)
{
    addNewControl({ &thread1Output, &thread2Output, startLearningBtn, saveFileName });
    setCursorParams();
    setFontParams(8, 12);

    startLearningBtn->mouseEvent = pixi::ui::TMouseEventHandler(&my_window::startLearingBtnOnClicked);
    saveFileName->keyEvent = pixi::ui::TKeyEventHandler(&my_window::save);
}

my_window::~my_window()
{
    delete saveFileName;
    delete startLearningBtn;
}

void my_window::startLearingBtnOnClicked(const pixi::ui::MouseRecord mer)
{
    if (mer.isPressed && mer.button == pixi::ui::MouseButton::LEFT)
    {
        std::thread t1(&my_window::startLearning, this, 5000, 10000), t2(&my_window::startLearning, this, 10000, 15000);
        //    std::thread t3(www, 10000, 15000), t4(www, 15000, 20000);

        t1.join(); t2.join();
        //        t3.join(); t4.join();
    }
}

void my_window::startLearning(const dword min_it, const dword max_it)
{
    int c = 100;
    while (c--)
    {
        dword lw = random(2, 256);
        dword li = random(min_it, max_it);

        detector det1(16, lw, lw, 2, li);
        det1.learn(swds, mwds, files_num, vector<>(16, -1.f), vector<>(16, +1.f));

        dword err = det1.statistic(swds, mwds, files_num);

        if (err < minerr) {
            minerr = err;
            std::stringstream stream; stream << "NNID" << std::this_thread::get_id() << ": " << minerr << " Kohonen layer: " << lw << " Iterations: " << li;
            startLearningBtn->setText(stream.str());

            mtx.lock();
            d1 = det1;
            mtx.unlock();
        }
        if (std::this_thread::get_id() == std::thread::id(2))
            thread1Output.increase();
        else
            thread2Output.increase();
    }
}

void my_window::save(const pixi::ui::KeyRecord ker)
{
    if (ker.isPressed && ker.keyCode == VK_RETURN)
    {
        d1.save(saveFileName->getText().c_str());
    }
}

#endif // MY_WINDOW_HPP
