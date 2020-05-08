#include <iostream>
#include "file.hpp"

using namespace std;

int main()
{
    pixi::files::file sw("../Kernel/data/SoftwareDataSet.data");
    cout << sw.field(0, 0) << endl;
    return 0;
}
