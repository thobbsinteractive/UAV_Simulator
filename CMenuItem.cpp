#include "CMenuItem.h"

CMenuItem::CMenuItem()
{

}

CMenuItem::~CMenuItem()
{
	for (int i = 0; i < (int)vSubNode.size(); i++)
	{
		// cleans up by removing all subnodes
		vSubNode.erase(vSubNode.begin() + i);
	}

}