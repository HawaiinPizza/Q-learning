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
    bool BLOCKED[4] = {false, false, false, false};
    NODE_STATUS status;
    Node(NODE_STATUS _status);
};

typedef vector<vector<Node>> vmap;

void QLearning(pair<int, int>);
void EnvironmentSimulation();
void PrintMap();
void PrintBestPolicy(time_t);
void PrintNGrid(int, time_t);
void PrintQGrid(int, time_t);
void IncrementN(std::pair<int, int>, DIR);
void SetQ(std::pair<int, int>, DIR, float);

int GetN(std::pair<int, int>, DIR);
int GetR(std::pair<int, int>, DIR);

float GetQ(std::pair<int, int>, DIR);
float GetMaxQ(std::pair<int, int>);

DIR EGreedy(std::pair<int, int>);

pair<int, int> RandomPos(vector<pair<int, int>>);
pair<int, int> GetNxtPos(std::pair<int, int>, DIR);
pair<pair<int, int>, DIR> Move(std::pair<int, int>, DIR);

vector<pair<int, int>> GetOpenPositions();

extern vmap MAP;
