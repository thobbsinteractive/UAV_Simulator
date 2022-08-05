// Holds buildings data (their locations on the map and the building objects themselves)

#ifndef INC_CMAP_H
#define INC_CMAP_H

#include "SBuildingAttributes.h"

class CMap
{
public:
	CMap(int sizeX,int sizeY);
	~CMap();

	// allows for different sizes of map
	int getGridSizeX(); 
	int getGridSizeY();
	char getNextBuilding(int x,int y);

private:
	char m_cMap[10][10];
	int gridSizeX;
	int gridSizeY;
};

#endif // INC_CMAP_H