#include<iostream>
#include "NodeMap.hpp"
using namespace std;
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

const double EPISON = .5;
const double GAMMA = .9;

int main(int argc, char *argv[])
{
	setmapup();
	auto x=  [](Node printme){

		cout << "EAST: " <<  (printme.BLOCKED[EAST] ? "OPEN" : "CLOSE" )<< '\t';
		cout << "WEST: " <<  (printme.BLOCKED[WEST] ? "OPEN" : "CLOSE" )<< '\t';
		cout << "NORTH: " <<  (printme.BLOCKED[NORTH] ? "OPEN" : "CLOSE" )<< '\t';
		cout << "SOUTH: " <<  (printme.BLOCKED[SOUTH] ? "OPEN" : "CLOSE" )<< '\t';
		cout << endl;
	};

	for (int i = 0; i < MAP.size(); ++i) {
		for (int j = 0; j < MAP[i].size(); ++j) {
			x(MAP[i][j]);

			
		}
	}
	
	return 0;
}
