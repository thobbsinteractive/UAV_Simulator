#ifndef INC_CAPPLICATION_H
#define INC_CAPPLICATION_H

#include "CD3DDisplay.h"
#include "SBuildingAttributes.h"
#include "SSimResults.h"
#include "CMap.h"
#include <chrono>

class CApplication
{
public:
	CApplication(CD3DDisplay *displayGraphics);
	~CApplication();

	void CreateMap();
	void Start();

private:
	CD3DDisplay *m_pDisplayGraphics;
	std::vector<SimResults> *m_pSimResultsArray;
};

#endif // INC_CAPPLICATION_H