#include<vector>
#include<iostream>
using namespace std;
enum NODE_STATUS{OPEN, CLOSE, GOAL, TRAP};
enum DIR{NORTH, SOUTH, EAST, WEST};
extern int _GOALPOS;
struct Node{
	int Q[4]  = {0,0,0,0};
	int N[4]  = {0,0,0,0};
	bool BLOCKED[4]  = {false, false, false, false};
	NODE_STATUS status;
	int value=0;
	Node(NODE_STATUS _status);
};
typedef vector<vector<Node>> vmap;

void setmapup();
extern vmap MAP;
