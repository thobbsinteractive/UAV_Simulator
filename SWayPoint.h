// My input structure that stores my games nav point data
#ifndef INC_WAYPOINT_H
#define INC_WAYPOINT_H

struct WayPoint
{
	char Type;
	D3DXVECTOR3 location;
	bool visited;

};

#endif // INC_WAYPOINT_H