#include "NodeMap.hpp"
#include <array>
#include <cstdlib>

const string IO_EAST = ">>>>";
const string IO_WEST = "<<<<";
const string IO_NORTH = "^^^^";
const string IO_SOUTH = "VVVV";

const int MOVE_HORZ = 2;
const int MOVE_NORTH = 3;
const int MOVE_SOUTH = 1;

const double DRIFT_NORTH = .7;
const double DRIFT_LEFT = .15;
const double DRIFT_RIGHT = .15;

const double EPISON = .05;
const double GAMMA = .9;

int _GOALPOS = 100;

Node::Node(NODE_STATUS _status) {
    status = _status;
    if (_status == GOAL)
        value = _GOALPOS;
}

void PrintMap() {
    int side;

    cout << "Q grid\n";
    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    cout << ' ' << MAP[i][j].Q[NORTH] << '\t';
                    break;
                case 1: // HORTZ
                    cout << MAP[i][j].Q[EAST] << ' ' << MAP[i][j].Q[WEST] << '\t';
                    break;
                case 2: // SOUTH
                    cout << ' ' << MAP[i][j].Q[SOUTH] << '\t';
                    break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    cout << "N grid\n";
    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    cout << ' ' << MAP[i][j].N[NORTH] << '\t';
                    break;
                case 1: // HORTZ
                    cout << MAP[i][j].N[EAST] << ' ' << MAP[i][j].N[WEST] << '\t';
                    break;
                case 2: // SOUTH
                    cout << ' ' << MAP[i][j].N[SOUTH] << '\t';
                    break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    cout << "Best policy\n";
    for (int i = 0; i < MAP.size(); ++i) {
        for (int j = 0; j < MAP[i].size(); ++j) {
            DIR bestdir = NORTH;
            float bestQ = MAP[i][j].Q[NORTH];
            if (MAP[i][j].Q[EAST] > bestQ) {
                bestQ = MAP[i][j].Q[EAST];
                bestdir = EAST;
            }
            if (MAP[i][j].Q[WEST] > bestQ) {
                bestQ = MAP[i][j].Q[WEST];
                bestdir = WEST;
            }
            if (MAP[i][j].Q[SOUTH] > bestQ) {
                bestQ = MAP[i][j].Q[SOUTH];
                bestdir = SOUTH;
            }
            switch (bestdir) {
            case 0:
                cout << IO_NORTH << '\t';
                break;
            case 1:
                cout << IO_SOUTH << '\t';
                break;
            case 3:
                cout << IO_EAST << '\t';
                break;
            case 2:
                cout << IO_WEST << '\t';
                break;
            }
        }
        cout << endl;
    }
}

void SetMapUp() {
    bool north, south, east, west;
    int tmpi = 0;
    int tmpj = 0;
    for (int i = 0; i < MAP.size(); ++i) {
        for (int j = 0; j < MAP[i].size(); ++j) {
            { // Checking north pos
                tmpi = i - 1;
                if (tmpi < 0 || tmpi >= MAP.size())
                    north = false;
                else if (MAP[tmpi][j].status == CLOSE)
                    north = false;
                else
                    north = true;
            }
            { // Checking south pos
                tmpi = i + 1;
                if (tmpi < 0 || tmpi >= MAP.size())
                    south = false;
                else if (MAP[tmpi][j].status == CLOSE)
                    south = false;
                else
                    south = true;
            }
            { // Checking east pos
                tmpj = j - 1;
                if (tmpj < 0 || tmpj >= MAP[i].size())
                    east = false;
                else if (MAP[i][tmpj].status == CLOSE)
                    east = false;
                else
                    east = true;
            }
            { // Checking west pos
                tmpj = j + 1;
                if (tmpj < 0 || tmpj >= MAP[i].size())
                    west = false;
                else if (MAP[i][tmpj].status == CLOSE)
                    west = false;
                else
                    west = true;
            }
            /* MAP[i][j].BLOCKED = {NORTH, SOUTH, EAST, WEST}; */
            /* MAP[i][j].BLOCKED[NORTH]=true; */
            /* MAP[i][j].BLOCKED[SOUTH]=true; */
            /* MAP[i][j].BLOCKED[EAST]=true; */
            /* MAP[i][j].BLOCKED[WEST]=true; */

            MAP[i][j].BLOCKED[NORTH] = north;
            MAP[i][j].BLOCKED[SOUTH] = south;
            MAP[i][j].BLOCKED[EAST] = east;
            MAP[i][j].BLOCKED[WEST] = west;
        }
    }
}

pair<int, int> RandomPos(int seed) {

    vector<pair<int, int>> openPos;

    openPos = GetOpenPositions();

    srand(seed);

    int randValue = rand() / ((RAND_MAX) / openPos.size());

    return openPos[randValue];
}

vector<pair<int, int>> GetOpenPositions() {

    vector<pair<int, int>> openPos;

    for (int x = 0; x < MAP.size(); x++) {
        for (int y = 0; y < MAP[1].size(); y++) {
            if (MAP[x][y].status == OPEN) {
                openPos.push_back(pair<int, int>{x, y});
            }
        }
    }

    return openPos;
}

int GetN(std::pair<int, int> curState, DIR action) {
    return MAP[curState.first][curState.second].N[action];
}

void IncrementN(std::pair<int, int> curState, DIR action) {
    MAP[curState.first][curState.second].N[action]++;
}

int GetR(std::pair<int, int> curState, DIR action) {

    switch (action) {
    case NORTH:
        return -3;
        break;

    case SOUTH:
        return -1;
        break;

    case EAST:
    case WEST:
        return -2;
        break;

    default:
        throw invalid_argument("\nINVALD VALUE FOR ACTION\n");
        break;
    }
}

float GetQ(std::pair<int, int> curState, DIR action) {
    return MAP[curState.first][curState.second].Q[action];
}

float GetMaxQ(std::pair<int, int> nextPos) {

    float max = MAP[nextPos.first][nextPos.second].Q[0];

    for (int x = 0; x < 4; x++) {
        if (MAP[nextPos.first][nextPos.second].Q[x] > max) {
            max = MAP[nextPos.first][nextPos.second].Q[x];
        }
    }

    return max;
}

void SetQ(std::pair<int, int> curState, DIR action, float newQValue) {
    MAP[curState.first][curState.second].Q[action] = newQValue;
}

vector<vector<Node>> MAP{
    {OPEN, OPEN, OPEN, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, GOAL, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, OPEN, OPEN, OPEN},
    {OPEN, OPEN, OPEN, OPEN, OPEN}};
