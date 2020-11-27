#include "NodeMap.hpp"
#include <array>
#include <cstdlib>
#include <iomanip>

const int ___E = 0;
const int ___N = 1;
const int ___S = 2;
const int ___W = 3;
const int ___WALL = -1;

vector<vector<int>> CORR{
    {___E, ___E, ___E, ___S, ___S},
    {___N, ___WALL, ___WALL, ___S, ___S},
    {___S, ___WALL, 100, ___W, ___W},
    {___S, ___WALL, ___WALL, ___N, ___W},
    {___S, ___WALL, ___E, ___N, ___N},
    {___E, ___E, ___E, ___N, ___N}};

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

        float qVal = GetQ(state, dir);
        int nVal = GetN(state, dir);

        pair<pair<int, int>, DIR> nextMove = Move(state, dir);

        float maxQVal = GetMaxQ(nextMove.first);
        int rVal = GetR(state, nextMove.second);

        // cout << "QVal: " << qVal << endl;
        // cout << "MaxQ: " << maxQVal << endl;
        // cout << "NVal: " << nVal << endl;
        // cout << "RVal: " << rVal << endl;

        float newQValue;

        newQValue = qVal + (1.0f / nVal) * (rVal + (GAMMA * maxQVal) - qVal);

        // cout << "NewQ: " << newQValue << endl
        //      << endl;

        SetQ(state, dir, newQValue);

        state = nextMove.first;

        x++;
    }
}

// I/O
void PrintMap() {

    int side;

    PrintNGrid(side);
    PrintQGrid(side);
    PrintBestPolicy();
}

void PrintQGrid(int side) {

    cout << "Q(s,a):\n";

    cout << setprecision(1);
    cout << fixed;

    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << '\t' << ' ' << '\t';
                    } else {
                        cout << setw(12);
                        cout << MAP[i][j].Q[NORTH];
                        cout << "    ";
                    }
                    break;
                case 1: // HORIZ
                    if (MAP[i][j].status == CLOSE) {
                        cout << setw(12);
                        cout << "####";
                        cout << "    ";
                    } else if (MAP[i][j].status == GOAL) {
                        cout << setw(12);
                        cout << "+100";
                        cout << "    ";
                    } else {
                        cout << setw(8);
                        cout << MAP[i][j].Q[WEST];
                        cout << setw(8);
                        cout << MAP[i][j].Q[EAST];
                    }
                    break;
                case 2: // SOUTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << '\t' << ' ' << '\t';
                    } else {
                        cout << setw(12);
                        cout << MAP[i][j].Q[SOUTH];
                        cout << "    ";
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

    cout << "N(s,a):\n";

    for (int i = 0; i < MAP.size(); ++i) {
        for (int k = 0; k < 3; k++) {
            side = k;
            for (int j = 0; j < MAP[i].size(); ++j) {
                switch (side) {
                case 0: //NORTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << '\t' << ' ' << '\t';
                    } else {
                        cout << setw(12);
                        cout << MAP[i][j].N[NORTH];
                        cout << "    ";
                    }
                    break;
                case 1: // HORIZ
                    if (MAP[i][j].status == CLOSE) {
                        cout << setw(12);
                        cout << "####";
                        cout << "    ";
                    } else if (MAP[i][j].status == GOAL) {
                        cout << setw(12);
                        cout << "+100";
                        cout << "    ";
                    } else {
                        cout << setw(8);
                        cout << MAP[i][j].N[WEST];
                        cout << setw(8);
                        cout << MAP[i][j].N[EAST];
                    }
                    break;
                case 2: // SOUTH
                    if (MAP[i][j].status == CLOSE ||
                        MAP[i][j].status == GOAL) {
                        cout << '\t' << ' ' << '\t';
                    } else {
                        cout << setw(12);
                        cout << MAP[i][j].N[SOUTH];
                        cout << "    ";
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

    cout << "Optimal Policy:\n";

    vector<pair<string, pair<int, int>>> diff;
    cout << '\t';
    for (int i = 0; i < MAP[0].size(); i++) {
        cout << i << ":\t";
    }
    cout << endl;
    for (int i = 0; i < MAP.size(); ++i) {
        cout << i << ":\t";
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
                /* pair<int,int> difference={i,j}; */
                switch (bestdir) {
                case NORTH:
                    if (___N != CORR[i][j]) {
                        pair<string, pair<int, int>> difference = {IO_NORTH, {i, j}};
                        diff.push_back(difference);
                    }
                    cout << IO_NORTH << '\t';
                    break;
                case SOUTH:
                    if (___S != CORR[i][j]) {
                        pair<string, pair<int, int>> difference = {IO_SOUTH, {i, j}};
                        diff.push_back(difference);
                    }
                    cout << IO_SOUTH << '\t';
                    break;
                case EAST:
                    if (___E != CORR[i][j]) {
                        pair<string, pair<int, int>> difference = {IO_EAST, {i, j}};
                        diff.push_back(difference);
                    }
                    cout << IO_EAST << '\t';
                    break;
                case WEST:
                    if (___W != CORR[i][j]) {
                        pair<string, pair<int, int>> difference = {IO_WEST, {i, j}};
                        diff.push_back(difference);
                    }
                    cout << IO_WEST << '\t';
                    break;
                }
            }
        }
        cout << endl;
    }
    for (int i = 0; i < diff.size(); ++i) {
        string correct;
        switch (CORR[diff[i].second.first][diff[i].second.second]) {
        case ___E:
            correct = IO_EAST;
            break;
        case ___W:
            correct = IO_WEST;
            break;
        case ___N:
            correct = IO_NORTH;
            break;
        case ___S:
            correct = IO_SOUTH;
            break;
        }
        cout << "DIFF: positon " << diff[i].second.first << '\t' << diff[i].second.second << '\t' << "values: " << diff[i].first << '\t' << correct << endl;
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
                    north = true;
                else if (MAP[tmpi][j].status == CLOSE)
                    north = true;
                else
                    north = false;
            }
            { // Checking south pos
                tmpi = i + 1;
                if (tmpi < 0 || tmpi >= MAP.size())
                    south = true;
                else if (MAP[tmpi][j].status == CLOSE)
                    south = true;
                else
                    south = false;
            }
            { // Checking east pos
                tmpj = j + 1;
                if (tmpj < 0 || tmpj >= MAP[i].size())
                    east = true;
                else if (MAP[i][tmpj].status == CLOSE)
                    east = true;
                else
                    east = false;
            }
            { // Checking west pos
                tmpj = j - 1;
                if (tmpj < 0 || tmpj >= MAP[i].size())
                    west = true;
                else if (MAP[i][tmpj].status == CLOSE)
                    west = true;
                else
                    west = false;
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
pair<int, int> RandomPos(vector<pair<int, int>> openPos) {

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

pair<pair<int, int>, DIR> Move(std::pair<int, int> curState, DIR action) {

    float randValue = rand() / (float)(RAND_MAX);

    // Drift Straight
    if (randValue >= DRIFT_HORIZONTAL) {
        return pair<pair<int, int>, DIR>{GetNxtPos(curState, action), action};
    } else if (randValue > DRIFT_LEFT) {
        if (action == NORTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, WEST), WEST};
        } else if (action == EAST) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, NORTH), NORTH};
        } else if (action == SOUTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, EAST), EAST};
        } else {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, SOUTH), SOUTH};
        }
    } else {
        if (action == NORTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, EAST), EAST};
        } else if (action == EAST) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, SOUTH), SOUTH};
        } else if (action == SOUTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, WEST), WEST};
        } else {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, NORTH), NORTH};
        }
    }
}

vector<pair<int, int>> GetOpenPositions() {

    vector<pair<int, int>> openPos;

    for (int x = 0; x < MAP.size(); x++) {
        for (int y = 0; y < MAP[x].size(); y++) {
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

    float randValue = rand() / (float)(RAND_MAX);

    DIR bestDir = (DIR)((int)(rand() / ((RAND_MAX) / 4)));

    if (randValue > EPISON) {

        float max = MAP[curState.first][curState.second].Q[0];

        for (int x = 0; x < 4; x++) {
            if (MAP[curState.first][curState.second].Q[x] > max) {

                max = MAP[curState.first][curState.second].Q[x];
                bestDir = (DIR)x;
            }
        }

    } else {

        bestDir = (DIR)(rand() / ((RAND_MAX) / 4));
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
