#include <iostream>
#include "detector.hpp"
#include <cassert>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>

using namespace pixi::math;

dword random(dword min, dword max) {
    return min + rand() % (max - min);
}

int main()
{
//    std::srand(std::time(nullptr));
    pixi::files::file swds("../Kernel/data/SoftwareDataSetRaw.data");
//    pixi::files::file swds("../Kernel/data/SoftwareDataSet.data");
    pixi::files::file mwds("../Kernel/data/MalwareDataSet.data");
//    pixi::files::file swds("../../Kernel/data/SoftwareDataSetRaw.data");
//    pixi::files::file mwds("../../Kernel/data/MalwareDataSet.data");

    dword files_num = 20000;//469 * 40;

    std::atomic<dword> minerr(0xffffffff);

    detector d1(16, 1024, 1024, 2, 40000);
    d1.learn(swds, mwds, files_num, vector<>(16, -1.f), vector<>(16, +1.f));
    //    d1.load("63.txt");

    std::mutex mtx;
    auto www = [&](const dword min_it, const dword max_it) {
        int c = 100;
        while (c--)
        {
            dword lw = random(2, 4096);
            dword li = random(min_it, max_it);

            detector det1(16, lw, lw, 2, li);
            det1.learn(swds, mwds, files_num, vector<>(16, -1.f), vector<>(16, +1.f));

            dword err = det1.statistic(swds, mwds, files_num);

            if (err < minerr) {
                minerr = err;
                std::cout << "NNID" << std::this_thread::get_id() << ": " << minerr << std::endl
                          << "Kohonen layer: " << lw << " Iterations: " << li << std::endl;
                mtx.lock();
                d1 = det1;
                mtx.unlock();
            }
        }
        std::cout << "NNID" << std::this_thread::get_id() << ": work done!" << std::endl;
    };

//    std::thread t1(www, 100, 5000), t2(www, 5000, 10000);
//    std::thread t3(www, 10000, 15000), t4(www, 15000, 20000);

//    t1.join(); t2.join();
//    t3.join(); t4.join();

    std::cout << "END!" << std::endl;

    d1.statistic(swds, mwds, files_num, true);

    char v[64];
    while (v[0] != 'q')
    {
        std::cin >> v;
             if (v[0] == 'w') d1.result(swds.signature(random(0, files_num))).print();
        else if (v[0] == 'e') d1.result(mwds.signature(random(0, files_num))).print();
    }
    return 0;
}
