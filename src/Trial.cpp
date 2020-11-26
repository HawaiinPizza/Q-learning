#include "NodeMap.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    int seed = 517;

    /* pair<int, int> wow = randomPos(seed); */
    /* cout << wow.first << '\t' << wow.second << endl; */

    std::pair<int, int> thang = RandomPos(seed);

    for (int x = 0; x < 10000; x++) {

        QLearning(thang);
    }

    SetMapUp();
    PrintMap();

    return 0;
}
