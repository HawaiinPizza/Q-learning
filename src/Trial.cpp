#include "NodeMap.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    int seed = 517;

    SetMapUp();
    PrintMap();
    /* pair<int, int> wow = randomPos(seed); */
    /* cout << wow.first << '\t' << wow.second << endl; */

    std::pair<int, int> thang = RandomPos(seed);

    std::cout << "Random Position: X = " << thang.first << " Y = " << thang.second;
    std::cout << std::endl;

    return 0;
}
