#include "NodeMap.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;



pair<int, int> randomPos(int seed = 1) {
    //Fun fact: the seed is automatcailly 1.
    srand(seed);

    int x = (rand()) / ((RAND_MAX) / MAP.size());
    int y = rand() / ((RAND_MAX) / MAP[x].size());
    /* IMPORTANT
	 * This will bascially check all columns of the current row
	 * for a new posistion
	 * If the walls are surronding the perimeter of the map
	 * This will not work.
	 * The only reason this is here now
	 * isb ecause fuck jtryng to do it the professional way
	 * TODO FIXME TODO XXX 
	 */
    { // I put it in it's own scope to indicate this is a hack.
        /* while(MAP[x][y].status==CLOSE || MAP[x][y].status == GOAL){ */
        while (MAP[x][y].status == CLOSE || MAP[x][y].status == GOAL) {
            y = y + 1;
        }
    }
    return pair<int, int>{x, y};
}

int main(int argc, char *argv[]) {

    int seed = 517;

    setmapup();
    printMap();
    /* pair<int, int> wow = randomPos(seed); */
    /* cout << wow.first << '\t' << wow.second << endl; */


    return 0;
}
