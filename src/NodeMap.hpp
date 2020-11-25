#include <iostream>
#include <vector>
using namespace std;
enum NODE_STATUS { OPEN,
                   CLOSE,
                   GOAL,
                   TRAP };

enum DIR { NORTH,
           SOUTH,
           EAST,
           WEST };

extern int _GOALPOS;

struct Node {
    float Q[4] = {0, 0, 0, 0};
    int N[4] = {0, 0, 0, 0};
    // If you want to see hwat the resluts are, use these
    /* float Q[4] = {1, 2, 3, 4}; */
    /* int N[4] = {0, 1, 2, 3}; */
    bool BLOCKED[4] = {false, false, false, false};
    NODE_STATUS status;
    int value = 0;
    Node(NODE_STATUS _status);
};

typedef vector<vector<Node>> vmap;

void SetMapUp();
void PrintMap();
pair<int, int> RandomPos(int seed = 1);
vector<pair<int, int>> GetOpenPositions();

extern vmap MAP;
