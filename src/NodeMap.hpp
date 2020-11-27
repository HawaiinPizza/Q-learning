#include <iostream>
#include <vector>
using namespace std;
enum NODE_STATUS { OPEN,
                   CLOSE,
                   GOAL,
                   TRAP };

enum DIR { NORTH,
           EAST,
           SOUTH,
           WEST };

extern int _GOALPOS;

struct Node {
    //{NORTH,EAST,SOUTH,WEST}
    float Q[4] = {0, 0, 0, 0};
    //{NORTH,EAST,SOUTH,WEST}
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

void QLearning(pair<int, int>);
void SetMapUp();
void PrintMap();
void PrintBestPolicy();
void PrintNGrid(int);
void PrintQGrid(int);
void IncrementN(std::pair<int, int>, DIR);
void SetQ(std::pair<int, int>, DIR, float);

int GetN(std::pair<int, int>, DIR);
int GetR(std::pair<int, int>, DIR);

float GetQ(std::pair<int, int>, DIR);
float GetMaxQ(std::pair<int, int>);

DIR EGreedy(std::pair<int, int>);

pair<int, int> RandomPos(int seed = 1);
pair<int, int> GetNxtPos(std::pair<int, int>, DIR);
pair<int, int> Move(std::pair<int, int>, DIR);

vector<pair<int, int>> GetOpenPositions();

extern vmap MAP;
