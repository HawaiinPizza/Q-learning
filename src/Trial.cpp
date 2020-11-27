#include "NodeMap.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    SetMapUp();

    vector<pair<int, int>> openPos;

    openPos = GetOpenPositions();

    for (int x = 0; x < 10000; x++) {

        std::pair<int, int> randPos = RandomPos(openPos);

        QLearning(randPos);
    }

    PrintMap();

    return 0;
}
