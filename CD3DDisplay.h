// Inherrits from and implements functions of CDisplay (so that other 3D languages like Open GL 
// can be implement later).

#ifndef INC_CD3DISPLAY_H
#define INC_CD3DISPLAY_H

#include "Enumerated.h"
#include "CD3DInitialiseDevice.h"
#include "CD3DMyCamera.h"
#include "CD3DViewObject.h"
#include "CD3DObject.h"
#include "CD3DBuilding.h"
#include "CD3DUAV.h"
#include "d3dfont.h"
#include "CD3DLight.h"

class CD3DDisplay
{
public:
	CD3DDisplay();
	~CD3DDisplay();

	bool InitialiseDisplay(void *pArg1,void **pArg2,int iWidth = 800,int iHeight = 600);
	void Draw(float timeDelta);
	void DisplayMenu();
	bool LoadMeshData();
	bool AddUAV(UavAttributes attributes);
	bool AddBuilding(float posX,float posY, float posZ,char type);
	void ChangeUAV();
	void ChangeCamera();
	void UpdateObjects(float timeDelta);

	bool SimulationComplete();
	std::vector<SimResults> *getSimResults();
	static int m_sMessage;
	static int m_wParam;

private:
	bool InitWindowsApp(int iWidth = 800,int iHeight = 600);
	bool InitialiseDevice(int iWidth = 800,int iHeight = 600);
	void UpdateCamera();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Identifies the window we are going to create

	HINSTANCE m_hInstance;
	HWND m_hwnd;

	CD3DViewObject *uavMesh;
	CD3DViewObject *buildingMesh1;
	CD3DViewObject *buildingMesh2;
	CD3DViewObject *buildingMesh3;
	CD3DViewObject *buildingMesh4;
	CD3DViewObject *roadMesh;
	CD3DViewObject *objectiveMesh;


	IDirect3DDevice9 *m_pDevice;
	CD3DMyCamera *m_pCamera;
	float m_fAerialCameraZoom;
	std::vector<CD3DObject*> m_D3DObjectsArray;
	CD3DFont *m_pFont;
	CD3DLight *m_pSun;

	DWORD m_dFrameCount;        // The number of frames that have occurred.
	float m_fTimeElapsed; // The time that has elapsed so far
	float m_fFPS;         // The frames rendered per second.
	
	char m_cFPSString[50];     // String to display current frames per second
	char m_cDebugString[250];     // String to display current frames per second

	int m_iCurrentCameraIndex;
	int m_iCurrentUAVCameraIndex;
	std::vector<int> m_UAVIndexArray;
	enum eCAMERATYPE { CHASE, AERIAL};

};

#endif // INC_CD3DISPLAY_H