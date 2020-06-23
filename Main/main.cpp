#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "kernel.hpp"
#include "MainWindow.hpp"

using namespace pixi;

int main()
{
    MainWindow wnd;
    windowEventProc(&wnd, true);
    return 0;
}
