#include "NodeMap.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    int seed = 517;

    /* pair<int, int> wow = randomPos(seed); */
    /* cout << wow.first << '\t' << wow.second << endl; */

    SetMapUp();

    std::pair<int, int> randPos = RandomPos(seed);

    for (int x = 0; x < 10; x++) {

        QLearning(randPos);
    }

    PrintMap();

    return 0;
}
