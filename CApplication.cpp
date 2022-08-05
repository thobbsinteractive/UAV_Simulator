#include "CApplication.h"
#include "SUavAttributes.h"

CApplication::CApplication(CD3DDisplay *displayGraphics)
{
	m_pDisplayGraphics = displayGraphics;

	// Has to be called before others
	m_pDisplayGraphics->LoadMeshData();
	

	// temp
	// populate your Uavs for sim
	UavAttributes attributes;
	
	attributes.startPosX = -200000.0f;
	attributes.startAltitude = 400000.0f;
	attributes.startPosZ = -1000000;

	attributes.startRotZ = 0.0f;
	attributes.startRotX = 0.0f;
	attributes.startRotY = 0.0f;

	attributes.rescuePosX = -800000.0f;
	attributes.rescueAltitude = 200000.0f;
	attributes.rescuePosZ = 800000.0f;

	attributes.startFuel = 100000.0f;
	attributes.flightSpeed = 28100.0f; // 281 Kph (175 mph)
	attributes.turnSpeed = 16000; // 160 Kph (100 mph)
	
	attributes.radarAspectWidth = 100;
	attributes.radarAspectHeight = 100;
	attributes.radarRange = 150000.0f; // 150 meters
	attributes.radarViewAngle = 0.25; // 90 Degrees

	m_pDisplayGraphics->AddUAV(attributes);

	attributes.startPosX = 600000.0f;
	attributes.startAltitude = 400000.0f;
	attributes.startPosZ = -1000000;

	attributes.startRotZ = 0.0f;
	attributes.startRotX = 0.0f;
	attributes.startRotY = 0.0f;

	attributes.rescuePosX = -800000.0f;
	attributes.rescueAltitude = 200000.0f;
	attributes.rescuePosZ = 800000.0f;

	attributes.startFuel = 100000.0f;
	attributes.flightSpeed = 28100.0f; // 281 Kph (175 mph)
	attributes.turnSpeed = 16000; // 160 Kph (100 mph)
	
	attributes.radarAspectWidth = 100;
	attributes.radarAspectHeight = 100;
	attributes.radarRange = 150000.0f; // 150 meters
	attributes.radarViewAngle = 0.25; // 90 Degrees

	m_pDisplayGraphics->AddUAV(attributes);

	CreateMap();
}

CApplication::~CApplication()
{
	delete m_pDisplayGraphics;

	for(int i =0;i < (int)m_pSimResultsArray->size();i++)
	{
		//erase all Sim results
		m_pSimResultsArray->erase(m_pSimResultsArray->begin() + i);
		printf("Sim Results deleted");
	}
}

void CApplication::CreateMap()
{
	CMap *map = new CMap(10,10);

	// All this messing about with offsetting means the maps relates directy to the Array in CMap
	int xOffSet = (map->getGridSizeX() * 200000) /2;
	int yOffSet = (map->getGridSizeY() * 200000) /2;

	for(int i = 0; i < map->getGridSizeX();i++)
	{
		for(int j = 0; j < map->getGridSizeY();j++)
		{
			m_pDisplayGraphics->AddBuilding(((float)j * 200000.0f)-xOffSet,0.0f,((float)i * -200000.0f)+yOffSet,map->getNextBuilding(i,j));
		}
	}
}

void CApplication::Start()
{
	// Message loop

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();

	while((msg.message != WM_QUIT)&&(m_pDisplayGraphics->SimulationComplete() == false))
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			// Caculates time between each frame
			std::chrono::system_clock::time_point currTime  = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> timeDelta = (currTime - lastTime) * 0.001f;

			m_pDisplayGraphics->UpdateObjects(timeDelta.count());
			m_pDisplayGraphics->Draw(timeDelta.count());

			std::chrono::duration<double, std::milli> renderTimeMS = std::chrono::system_clock::now() - currTime;

			//lock to 60 fps
			if (renderTimeMS.count() > 0 && renderTimeMS.count() < 16.6f)
			{
				Sleep(16.6f - renderTimeMS.count());
			}

			lastTime = currTime;	
        }
    }

	// Get the sim results
	m_pSimResultsArray = m_pDisplayGraphics->getSimResults();

	// do something with them
	//SimResults mySimResult = m_pSimResultsArray->at(0);
	//mySimResult = mySimResult;

	// ?
}