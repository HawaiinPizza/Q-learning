#include "NodeMap.hpp"
#include <array>
#include <cstdlib>
#include <iomanip>

const string IO_EAST = ">>>>";
const string IO_WEST = "<<<<";
const string IO_NORTH = "^^^^";
const string IO_SOUTH = "VVVV";

const int MOVE_HORZ = 2;
const int MOVE_NORTH = 3;
const int MOVE_SOUTH = 1;

const double DRIFT_HORIZONTAL = .3;
const double DRIFT_RIGHT = 0.15;

const double EPSILON = .05;
const double GAMMA = .9;

int _GOALPOS = 100;

Node::Node(NODE_STATUS _status) {
    status = _status;
}

void QLearning(pair<int, int> state) {

    int x = 0;
    DIR dir;

    // Run the trial for 100 turns
    // stop if the goal or a trap is reached
    while (MAP[state.first][state.second].status != GOAL &&
           MAP[state.first][state.second].status != TRAP &&
           x < 100) {

        // Find the next direction using E-Greedy
        dir = EGreedy(state);

        // Equation 1
        IncrementN(state, dir);

        // Equation 2
        // Find the next position in order to get the reward
        // and Max Q value
        pair<pair<int, int>, DIR> nextMove = Move(state, dir);

        float qVal = GetQ(state, dir);
        float maxQVal = GetMaxQ(nextMove.first);

        int rVal = GetR(state, nextMove.second);
        int nVal = GetN(state, dir);

        float newQValue;

        // This is Q learning equation
        newQValue = qVal + (1.0f / nVal) * (rVal + (GAMMA * maxQVal) - qVal);

        SetQ(state, dir, newQValue);

        // Moving to the next state
        state = nextMove.first;

        // Increment the turn
        x++;
    }
}

// I/O
void PrintMap() {

    int side;

    // Call all of the printing functions
    // and print all of the different required maps
    PrintNGrid(side);
    PrintQGrid(side);
    PrintBestPolicy();
}

void PrintQGrid(int side) {

    cout << "Q(s,a):\n";

    // Printing all Q values of each position
    //      -2.1
    // 23.5      22.6
    //      38.7

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

    // Printing all N values of each position
    //      25
    //  25      25
    //      25
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

    // Column numbering at the top
    for (int i = 0; i < MAP[0].size(); i++) {
        cout << setw(4);
        cout << i << ":";
    }

    cout << endl;

    // Loop through the maze and print out the best direction
    // based on the highest Q value in that position
    for (int i = 0; i < MAP.size(); ++i) {
        // Row numbering on the right side
        cout << i << ":";
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

            cout << setw(5);

            // Printing out the approptiate symbols of:
            // >>>>, <<<<, ^^^^, VVVV, ####, and +100
            if (MAP[i][j].status == CLOSE) {
                cout << "####";
            } else if (MAP[i][j].status == GOAL) {
                cout << "+100";
            } else {
                switch (bestdir) {
                case NORTH:
                    cout << IO_NORTH;
                    break;
                case SOUTH:
                    cout << IO_SOUTH;
                    break;
                case EAST:
                    cout << IO_EAST;
                    break;
                case WEST:
                    cout << IO_WEST;
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

    /* 
        Loop through the maze and find whether the surounding
        areas of each position is blocked or not.
        This will help later when finding whether the next position
        is blocked.
    */
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

            MAP[i][j].BLOCKED[NORTH] = north;
            MAP[i][j].BLOCKED[SOUTH] = south;
            MAP[i][j].BLOCKED[EAST] = east;
            MAP[i][j].BLOCKED[WEST] = west;
        }
    }
}

// Position Functions
pair<int, int> RandomPos(vector<pair<int, int>> openPos) {

    // Taking in a list of the open positions in the maze

    // Findin a random value from the range of
    // 0 to size of the open positions array
    int randValue = rand() / ((RAND_MAX) / openPos.size());

    // Choosing a random position
    return openPos[randValue];
}

pair<int, int> GetNxtPos(std::pair<int, int> curState, DIR action) {

    // Taking in a state and action
    switch (action) {
    // Checking whether the next position is open or blocked
    // If it is not blocked then return the next position
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

    // Returning the current position if the next position is blocked
    // AKA Bouncing off
    return curState;
}

pair<pair<int, int>, DIR> Move(std::pair<int, int> curState, DIR action) {

    // Roll for drifting
    float randValue = rand() / (float)(RAND_MAX);

    // Drift Straight if the random roll is greater than 30%
    if (randValue >= DRIFT_HORIZONTAL) {
        return pair<pair<int, int>, DIR>{GetNxtPos(curState, action), action};
        // If the roll is between 30% - 15% we drift LEFT
    } else if (randValue > DRIFT_RIGHT) {
        if (action == NORTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, WEST), WEST};
        } else if (action == EAST) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, NORTH), NORTH};
        } else if (action == SOUTH) {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, EAST), EAST};
        } else {
            return pair<pair<int, int>, DIR>{GetNxtPos(curState, SOUTH), SOUTH};
        }
        // If the roll is between 15% - 0% we drift RIGHT
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

    // Loop through the entire maze and record the
    // x and y of the open positions
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
    // Return the N value based on the chosen direction
    // and the current position
    return MAP[curState.first][curState.second].N[action];
}

int GetR(std::pair<int, int> curState, DIR action) {

    // Based on the current action return the cost for that direction
    // If the next position based on the action is the goal and
    // is not blocked, then return GOALVALUE - COST TO GET TO GOAL
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
    // Return the appropriate Q value based on the action
    // and the current position
    return MAP[curState.first][curState.second].Q[action];
}

float GetMaxQ(std::pair<int, int> nextState) {

    // This is a standard algorithm to find the largest
    // value in an array
    float max = MAP[nextState.first][nextState.second].Q[0];

    // Loop though all of the values and keep a record of the
    // largest one
    for (int x = 0; x < 4; x++) {
        if (MAP[nextState.first][nextState.second].Q[x] > max) {
            max = MAP[nextState.first][nextState.second].Q[x];
        }
    }

    return max;
}

DIR EGreedy(std::pair<int, int> curState) {

    // Roll for exploration
    float randValue = rand() / (float)(RAND_MAX);

    DIR bestDir = (DIR)((int)(rand() / ((RAND_MAX) / 4)));

    // There is a 5% chance to pick a random direction
    // If the roll is greater than 5% we go with the best direction
    if (randValue > EPSILON) {

        float max = MAP[curState.first][curState.second].Q[0];

        for (int x = 0; x < 4; x++) {
            if (MAP[curState.first][curState.second].Q[x] > max) {

                max = MAP[curState.first][curState.second].Q[x];
                bestDir = (DIR)x;
            }
        }

    } else {
        // Picking a random direction
        bestDir = (DIR)(rand() / ((RAND_MAX) / 4));
    }

    return bestDir;
}

// Set Value for equations functions
void SetQ(std::pair<int, int> curState, DIR action, float newQValue) {
    // Given a State, Action, and NewQ
    // Assign the Q value to the appropriate direction and position
    MAP[curState.first][curState.second].Q[action] = newQValue;
}

void IncrementN(std::pair<int, int> curState, DIR action) {
    // Increment N to be used in Equation 2 of P3.pdf
    MAP[curState.first][curState.second].N[action] += 1;
}

// The Map used in the entire program
vector<vector<Node>> MAP{
    {OPEN, OPEN, OPEN, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, GOAL, OPEN, OPEN},
    {OPEN, CLOSE, CLOSE, OPEN, OPEN},
    {OPEN, CLOSE, OPEN, OPEN, OPEN},
    {OPEN, OPEN, OPEN, OPEN, OPEN}};
