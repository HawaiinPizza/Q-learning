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

        //cout << "Random Position: X = " << randPos.first << " Y = " << randPos.second << endl;

        QLearning(randPos);
    }

    PrintMap();

    return 0;
}
