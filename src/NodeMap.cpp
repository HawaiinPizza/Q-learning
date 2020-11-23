#include "NodeMap.hpp"
#include<cstdlib>
int _GOALPOS=100;
Node::Node(NODE_STATUS _status) {
	status=_status;
	if(_status == GOAL)
		value=_GOALPOS;
}

void setmapup(){
	bool north, south, east, west;	
	int tmpi=0;
	int tmpj=0;
	for (int i = 0; i < MAP.size(); ++i) {
		for (int j = 0; j < MAP[i].size(); ++j) {
			{ // Checking north pos
				tmpi=i-1;
				if(tmpi<0 || tmpi>= MAP.size())
					north=false;
				else if (MAP[tmpi][j].status==CLOSE)
					north=false;
				else
					north=true;
			}
			{ // Checking south pos
				tmpi=i+1;
				if(tmpi<0 || tmpi>= MAP.size())
					south=false;
				else if (MAP[tmpi][j].status==CLOSE)
					south=false;
				else
					south=true;
			}
			{ // Checking east pos
				tmpj=j-1;
				if(tmpj<0 || tmpj>= MAP[i].size())
					east=false;
				else if (MAP[i][tmpj].status==CLOSE)
					east=false;
				else
					east=true;
			}
			{ // Checking west pos
				tmpj=j+1;
				if(tmpj<0 || tmpj>= MAP[i].size())
					west=false;
				else if (MAP[i][tmpj].status==CLOSE)
					west=false;
				else
					west=true;
			}
			/* MAP[i][j].BLOCKED = {NORTH, SOUTH, EAST, WEST}; */
			/* MAP[i][j].BLOCKED[NORTH]=true; */ 
			/* MAP[i][j].BLOCKED[SOUTH]=true; */ 
			/* MAP[i][j].BLOCKED[EAST]=true; */ 
			/* MAP[i][j].BLOCKED[WEST]=true; */ 

			MAP[i][j].BLOCKED[NORTH]=north; 
			MAP[i][j].BLOCKED[SOUTH]=south; 
			MAP[i][j].BLOCKED[EAST]=east; 
			MAP[i][j].BLOCKED[WEST]=west; 

		}
	}

}


pair<int,int> randompos(){
}


vector<vector<Node>> MAP{
	{OPEN,	OPEN,	OPEN,	OPEN,	OPEN}
	,{OPEN,	CLOSE,	CLOSE,	OPEN,	OPEN}
	,{OPEN,	CLOSE,	GOAL,	OPEN,	OPEN}
	,{OPEN,	CLOSE,	CLOSE,	OPEN,	OPEN}
	,{OPEN,	CLOSE,	OPEN,	OPEN,	OPEN}
	,{OPEN,	OPEN,	OPEN,	OPEN,	OPEN}
};

