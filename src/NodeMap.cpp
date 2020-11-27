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

const double DRIFT_HORIZONTAL = .3;
const double DRIFT_LEFT = 0.15;

const double EPISON = .05;
const double GAMMA = .9;

int _GOALPOS = 100;

Node::Node(NODE_STATUS _status) {
    status = _status;
    if (_status == GOAL)
        value = _GOALPOS;
}

void QLearning(pair<int, int> state) {

    int x = 0;
    DIR dir;

    while (MAP[state.first][state.second].status != GOAL &&
           MAP[state.first][state.second].status != TRAP &&
           x < 100) {

        dir = EGreedy(state);

        // Equation 1
        IncrementN(state, dir);

        // Equation 2
        pair<int, int> nextPos = GetNxtPos(state, dir);

        float newQValue = 0.0f;

        newQValue = GetQ(state, dir) +
                    (((1 / GetN(state, dir)) *
                      (GetR(state, dir) +
                       GAMMA * GetMaxQ(nextPos) -
                       GetQ(state, dir))));

        SetQ(state, dir, newQValue);

        state = Move(state, dir);

        x++;
    }
}

// I/O
void PrintMap() {

    int side;

    PrintQGrid(side);
    PrintNGrid(side);
    PrintBestPolicy();
}

void PrintQGrid(int side) {

    cout << "Q grid\n";

    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << ' ' << ' ' << '\t';
                    } else {
                        cout << ' ' << MAP[i][j].Q[NORTH] << '\t';
                    }
                    break;
                case 1: // HORTZ
                    if (MAP[i][j].status == CLOSE) {
                        cout << "####" << '\t';
                    } else if (MAP[i][j].status == GOAL) {
                        cout << "+100" << '\t';
                    } else {
                        cout << MAP[i][j].Q[EAST] << ' ' << MAP[i][j].Q[WEST] << '\t';
                    }
                    break;
                case 2: // SOUTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << ' ' << ' ' << '\t';
                    } else {
                        cout << ' ' << MAP[i][j].Q[SOUTH] << '\t';
                    }
                    break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

void PrintNGrid(int side) {

    cout << "N grid\n";

    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << ' ' << ' ' << '\t';
                    } else {
                        cout << ' ' << MAP[i][j].N[NORTH] << '\t';
                    }
                    break;
                case 1: // HORTZ
                    if (MAP[i][j].status == CLOSE) {
                        cout << "####" << '\t';
                    } else if (MAP[i][j].status == GOAL) {
                        cout << "+100" << '\t';
                    } else {
                        cout << MAP[i][j].N[EAST] << ' ' << MAP[i][j].N[WEST] << '\t';
                    }
                    break;
                case 2: // SOUTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << ' ' << ' ' << '\t';
                    } else {
                        cout << ' ' << MAP[i][j].N[SOUTH] << '\t';
                    }
                    break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

void PrintBestPolicy() {

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

            if (MAP[i][j].status == CLOSE) {
                cout << "####" << '\t';
            } else if (MAP[i][j].status == GOAL) {
                cout << "+100" << '\t';
            } else {
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

// Position Functions
pair<int, int> RandomPos(int seed) {

    vector<pair<int, int>> openPos;

    openPos = GetOpenPositions();

    srand(seed);

    int randValue = rand() / ((RAND_MAX) / openPos.size());

    return openPos[randValue];
}

pair<int, int> GetNxtPos(std::pair<int, int> curState, DIR action) {

    switch (action) {
    case NORTH:
        if (!MAP[curState.first][curState.second].BLOCKED[NORTH])
            return pair<int, int>{curState.first - 1, curState.second};
        break;

    case EAST:
        if (!MAP[curState.first][curState.second].BLOCKED[EAST])
            return pair<int, int>{curState.first, curState.second + 1};
        break;

    case SOUTH:
        if (!MAP[curState.first][curState.second].BLOCKED[SOUTH])
            return pair<int, int>{curState.first + 1, curState.second};
        break;

    case WEST:
        if (!MAP[curState.first][curState.second].BLOCKED[WEST])
            return pair<int, int>{curState.first, curState.second - 1};
        break;
    }

    return curState;
}

pair<int, int> Move(std::pair<int, int> curState, DIR action) {

    double randValue = rand() / ((RAND_MAX) / 1);

    // Drift Straight
    if (randValue > DRIFT_HORIZONTAL) {
        return GetNxtPos(curState, action);
    } else if (randValue >= DRIFT_LEFT) {
        if (action == NORTH) {
            return GetNxtPos(curState, EAST);
        } else if (action == EAST) {
            return GetNxtPos(curState, SOUTH);
        } else if (action == SOUTH) {
            return GetNxtPos(curState, WEST);
        } else {
            return GetNxtPos(curState, NORTH);
        }
    } else {
        if (action == NORTH) {
            return GetNxtPos(curState, WEST);
        } else if (action == EAST) {
            return GetNxtPos(curState, NORTH);
        } else if (action == SOUTH) {
            return GetNxtPos(curState, EAST);
        } else {
            return GetNxtPos(curState, SOUTH);
        }
    }
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

// Get Value for equations functions
int GetN(std::pair<int, int> curState, DIR action) {
    return MAP[curState.first][curState.second].N[action];
}

int GetR(std::pair<int, int> curState, DIR action) {

    switch (action) {
    case NORTH:
        if (!MAP[curState.first][curState.second].BLOCKED[NORTH] &&
            MAP[curState.first - 1][curState.second].status == GOAL) {
            return _GOALPOS - MOVE_NORTH;
        }
        return -MOVE_NORTH;
        break;

    case SOUTH:
        if (!MAP[curState.first][curState.second].BLOCKED[SOUTH] &&
            MAP[curState.first + 1][curState.second].status == GOAL) {
            return _GOALPOS - MOVE_SOUTH;
        }
        return -MOVE_SOUTH;
        break;

    case EAST:
        if (!MAP[curState.first][curState.second].BLOCKED[EAST] &&
            MAP[curState.first][curState.second + 1].status == GOAL) {
            return _GOALPOS - MOVE_HORZ;
        }
        return -MOVE_HORZ;
        break;

    case WEST:
        if (!MAP[curState.first][curState.second].BLOCKED[WEST] &&
            MAP[curState.first][curState.second - 1].status == GOAL) {
            return _GOALPOS - MOVE_HORZ;
        }
        return -MOVE_HORZ;
        break;

    default:
        throw invalid_argument("\nINVALD VALUE FOR ACTION\n");
        break;
    }
}

float GetQ(std::pair<int, int> curState, DIR action) {
    return MAP[curState.first][curState.second].Q[action];
}

float GetMaxQ(std::pair<int, int> nextState) {

    float max = MAP[nextState.first][nextState.second].Q[0];

    for (int x = 0; x < 4; x++) {
        if (MAP[nextState.first][nextState.second].Q[x] > max) {
            max = MAP[nextState.first][nextState.second].Q[x];
        }
    }

    return max;
}

DIR EGreedy(std::pair<int, int> curState) {

    double randValue = rand() / ((RAND_MAX) / 1);
    DIR bestDir;

    if (randValue > EPISON) {

        float max = MAP[curState.first][curState.second].Q[0];

        for (int x = 0; x < 4; x++) {
            if (MAP[curState.first][curState.second].Q[x] > max) {
                max = MAP[curState.first][curState.second].Q[x];
                if (x == 0)
                    bestDir = NORTH;
                else if (x == 1)
                    bestDir = EAST;
                else if (x == 2)
                    bestDir = SOUTH;
                else
                    bestDir = WEST;
            }
        }

    } else {

        int directionChoice = rand() / ((RAND_MAX) / 4);

        if (directionChoice == 0) {
            bestDir = NORTH;
        } else if (directionChoice == 1) {
            bestDir = EAST;
        } else if (directionChoice == 2) {
            bestDir = SOUTH;
        } else if (directionChoice == 2) {
            bestDir = WEST;
        }
    }

    return bestDir;
}

// Set Value for equations functions
void SetQ(std::pair<int, int> curState, DIR action, float newQValue) {
    MAP[curState.first][curState.second].Q[action] = newQValue;
}

void IncrementN(std::pair<int, int> curState, DIR action) {
    MAP[curState.first][curState.second].N[action] += 1;
}

// The Map
vector<vector<Node>> MAP{
    {OPEN, OPEN, OPEN, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, GOAL, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, OPEN, OPEN, OPEN},
    {OPEN, OPEN, OPEN, OPEN, OPEN}};
