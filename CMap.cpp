#include "CMap.h"

CMap::CMap(int sizeX,int sizeY)
{
	gridSizeX = sizeX;
	gridSizeY = sizeY;
}

CMap::~CMap()
{

}

int CMap::getGridSizeX()
{
	return gridSizeX;
}

int CMap::getGridSizeY()
{
	return gridSizeY;
}

char CMap::getNextBuilding(int x,int y)
{
	char m_cMap[10][10] = {{'4','R','4','2','R','3','2','1','R','4'},
						   {'R','R','R','R','R','R','R','1','R','3'},
						   {'1','R','4','4','R','1','R','1','R','4'},
						   {'2','R','4','2','R','R','R','R','R','R'},
						   {'R','R','R','R','R','3','1','R','1','2'},
						   {'1','R','3','2','R','3','2','R','3','4'},
						   {'3','R','1','1','R','2','1','R','4','3'},
						   {'R','R','R','R','R','R','R','R','R','R'},
						   {'3','R','4','4','R','3','2','R','2','3'},
						   {'2','R','4','2','R','1','2','R','2','2'}};

	char object = m_cMap[x][y];

	return object;
}