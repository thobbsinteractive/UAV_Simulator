#include "CD3DDisplay.h"

int CD3DDisplay::m_sMessage = 0;
int CD3DDisplay::m_wParam = 0;

CD3DDisplay::CD3DDisplay()
{
	m_hwnd = 0;
	m_iCurrentCameraIndex = 0;
	m_iCurrentUAVCameraIndex = 0;
	m_fTimeElapsed = 0.0f;
	m_fAerialCameraZoom = 100000;
}

CD3DDisplay::~CD3DDisplay()
{
	if(m_pCamera)
	{
		delete m_pCamera;
		printf("Camera deleted");
	}

	for(int i =0;i < (int)m_D3DObjectsArray.size();i++)
	{
		//erase all meshes
		m_D3DObjectsArray.erase(m_D3DObjectsArray.begin() + i);
		printf("Meshes deleted");
	}

	if(m_pFont)
	{
		delete m_pFont;
	}

	if(m_pDevice)
	{
		m_pDevice->Release();
		printf("Device Released");
	}

	if(uavMesh)
	{
		delete uavMesh;
	}

	if(buildingMesh1)
	{
		delete buildingMesh1;
	}
	if(buildingMesh2)
	{
		delete buildingMesh2;
	}
	if(buildingMesh3)
	{
		delete buildingMesh3;
	}
	if(buildingMesh4)
	{
		delete buildingMesh4;
	}
	if(roadMesh)
	{
		delete roadMesh;
	}
	if(objectiveMesh)
	{
		delete objectiveMesh;
	}
}

LRESULT CALLBACK CD3DDisplay::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	CD3DDisplay::m_sMessage = message;
	CD3DDisplay::m_wParam = wParam;

	switch (message) 
	{
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				::DestroyWindow(hWnd);
			}
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
	}

	// Forward any other message we didn't handle
	return ::DefWindowProc(hWnd,message,wParam,lParam);
}

bool CD3DDisplay::InitWindowsApp(int iWidth,int iHeight) // Create main application window
{
	//
	// Create the main windows application window.
	//
	
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC) CD3DDisplay::WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"UAV";

	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}
	
	m_hwnd = ::CreateWindow(L"UAV", L"UAV", 
		WS_EX_TOPMOST,
		0, 
		0, 
		iWidth, 
		iHeight,
		0 /*parent hwnd*/, 
		0 /* menu */, 
		m_hInstance, 
		0 /*extra*/); 

	if( !m_hwnd )
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
	
	return true;
}

bool CD3DDisplay::InitialiseDevice(int iWidth,int iHeight)
{
	CD3DInitialiseDevice d3DInitDevice;
	SETTINGS settings;
	settings.Resolution.Height = iHeight;
	settings.Resolution.Width = iWidth;

	return d3DInitDevice.getCapabilities(settings,m_hInstance,&m_hwnd,&m_pDevice);
}

bool CD3DDisplay::InitialiseDisplay(void *pArg1,void **pArg2,int iWidth,int iHeight)
{
	bool success = true;

	m_hInstance = (HINSTANCE)pArg1;

	if(!InitWindowsApp(iWidth,iHeight))
	{
		// On error display this message and exit
		::MessageBox(0, L"Windows App Initialisation - Failed", L"Error", MB_OK);
		success = false;
	}

	if(!InitialiseDevice())
	{
		// On error display this message and exit
		::MessageBox(0, L"Display Device Initialisation - Failed", L"Error", MB_OK);
		success = false;
	}

	// Setup camera

	if(success)
	{
		// Set up basic camera
		m_pCamera = new CD3DMyCamera(m_pDevice);
		m_pCamera->SetUpCamera(iWidth,iHeight);
		m_pCamera->SetPosition(D3DXVECTOR3(0.0f,100000.0f,1000.0));
		m_pCamera->SetTarget(D3DXVECTOR3(0.0f,0.0f,0.0f));

		//
		// Set alpha blending states.
		//

		// use alpha in material's diffuse component for alpha
		//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	

		// Default render states (temp post effects file)
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); //Default shademode
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);        //Turn culling on

		// Device texture setup: probably not necessary in this sime
		m_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		//m_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		// Lighting (basiclighting)
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		m_pSun = new CD3DLight(m_pDevice);
		m_pSun->InitPointLight(D3DXVECTOR3(1000000.0f,1000000.0f,1000000.0f),D3DXCOLOR(0.8f,0.8f,0.8f,0.0f));

		LoadMeshData();
	}

	return success;
}

// Loads meshes for the simulation
bool CD3DDisplay::LoadMeshData()
{

	uavMesh = new CD3DViewObject();

	buildingMesh1 = new CD3DViewObject();;
	buildingMesh2 = new CD3DViewObject();;
	buildingMesh3 = new CD3DViewObject();;
	buildingMesh4 = new CD3DViewObject();;
	roadMesh = new CD3DViewObject();
	objectiveMesh = new CD3DViewObject();

	if(uavMesh->loadMeshIntoBuffer("Meshes/uav.x",m_pDevice))
	{

	}

	if(objectiveMesh->loadMeshIntoBuffer("Meshes/objective.x",m_pDevice))
	{

	}

	if(buildingMesh1->loadMeshIntoBuffer("Meshes/buildingType1_500meters.X",m_pDevice))
	{
		buildingMesh1->computeCD3DBoundingSphere();
	}

	if(buildingMesh2->loadMeshIntoBuffer("Meshes/buildingType2_300meters.X",m_pDevice))
	{
		buildingMesh2->computeCD3DBoundingSphere();
	}

	if(buildingMesh3->loadMeshIntoBuffer("Meshes/buildingType3_100meters.X",m_pDevice))
	{
		buildingMesh3->computeCD3DBoundingSphere();
	}

	if(buildingMesh4->loadMeshIntoBuffer("Meshes/buildingType4_800meters.X",m_pDevice))
	{
		buildingMesh4->computeCD3DBoundingSphere();
	}

	if(roadMesh->loadMeshIntoBuffer("Meshes/buildingTypeRoad_0meters.X",m_pDevice))
	{
		roadMesh->computeCD3DBoundingSphere();
	}

	m_pFont = new CD3DFont(L"Times New Roman", 16, 0);
	m_pFont->InitDeviceObjects(m_pDevice);
	m_pFont->RestoreDeviceObjects();

	return true;
}


// Run After LoadMeshData!
bool CD3DDisplay::AddUAV(UavAttributes attributes)
{
	
	// Create a new Object (wraps mesh and position/drawing methods)
	CD3DObject *uavObject;
	uavObject = new CD3DUAV(attributes,uavMesh,m_pDevice);
	uavObject->addMesh(objectiveMesh);

	if(m_D3DObjectsArray.size() < 250)
	{
		m_UAVIndexArray.push_back(m_D3DObjectsArray.size());
		m_D3DObjectsArray.push_back(uavObject);
		return true;
	}
	return false;
}

std::vector<SimResults> *CD3DDisplay::getSimResults()
{
	std::vector<SimResults> thisSimResultArray;

	for(int i =0;i < (int)m_D3DObjectsArray.size();i++)
	{
		
		// Check type
		char type[250];
		m_D3DObjectsArray[i]->getType(type);

		// If UAV we need to get it sim results and place it in our array for extraction
		if(strcmp(type,"CD3DUAV") == 0)
		{
			SimResults results;
			results = static_cast<CD3DUAV *>(m_D3DObjectsArray[i])->getSimResults();
			results.UAVIndex = m_UAVIndexArray[i];
			thisSimResultArray.push_back(results);
		}
	}
	
	return &thisSimResultArray;
}

// Run After LoadMeshData!
bool CD3DDisplay::AddBuilding(float posX,float posY,float posZ,char type)
{
	D3DXVECTOR3 position = D3DXVECTOR3(posX, posY, posZ);
	// Create a new Object (wraps mesh and position/drawing methods)
	CD3DObject *buildingObject;

	// Decides type (there are different ones)
	switch(type)
	{
		case 'R': buildingObject = new CD3DBuilding(position,roadMesh,m_pDevice);
			break;
		case '1': buildingObject = new CD3DBuilding(position,buildingMesh1,m_pDevice);
			break;
		case '2': buildingObject = new CD3DBuilding(position,buildingMesh2,m_pDevice);
			break;
		case '3': buildingObject = new CD3DBuilding(position,buildingMesh3,m_pDevice);
			break;
		case '4': buildingObject = new CD3DBuilding(position,buildingMesh4,m_pDevice);
			break;
		default:
			buildingObject = new CD3DBuilding(position,roadMesh,m_pDevice);
	}

	if(m_D3DObjectsArray.size() < 250)
	{
		m_D3DObjectsArray.push_back(buildingObject);
		return true;
	}
	return false;
}

void CD3DDisplay::ChangeUAV()
{
	// Change UAV we are following
	if(m_iCurrentUAVCameraIndex < m_UAVIndexArray.size()-1)
	{
		m_iCurrentUAVCameraIndex++;
	// Go back to first UAV
	}else
	{
		m_iCurrentUAVCameraIndex = 0;
	}
}

void CD3DDisplay::ChangeCamera()
{
	// Change between cameras
	if(m_iCurrentCameraIndex < 1)
	{
		m_iCurrentCameraIndex++;
	}else
	{
		m_iCurrentCameraIndex = 0;
	}
}

void CD3DDisplay::UpdateObjects(float timeDelta)
{
	// Update behavior of all objects
	for(int i =0;i < (int)m_D3DObjectsArray.size();i++)
	{
		
		// Check type
		char type[250];
		m_D3DObjectsArray[i]->getType(type);

		// If UAV we need to update it (move it)
		if(strcmp(type,"CD3DUAV") == 0)
		{
			static_cast<CD3DUAV *>(m_D3DObjectsArray[i])->behavior(m_D3DObjectsArray,timeDelta);
		}
	}

	// Really should not deal with key presses in the draw code
	if(m_fTimeElapsed > 0.05f)
	{
		m_fTimeElapsed = 0.0f;

		switch (CD3DDisplay::m_sMessage) 
		{
			case WM_KEYDOWN:
				if(CD3DDisplay::m_wParam == VK_HOME)
				{		
					m_fAerialCameraZoom -= (100000.0f*timeDelta);
				}
				
				if(CD3DDisplay::m_wParam == VK_END)
				{	
					m_fAerialCameraZoom += (100000.0f*timeDelta);
				}

				if(CD3DDisplay::m_wParam == VK_UP)
				{		
					m_D3DObjectsArray[0]->pitch(5.0,timeDelta);
				}
				
				if(CD3DDisplay::m_wParam == VK_DOWN)
				{	
					m_D3DObjectsArray[0]->pitch(-5.0,timeDelta);
				}
				
				if(CD3DDisplay::m_wParam == VK_LEFT)
				{
					m_D3DObjectsArray[0]->yaw(-5.0f,timeDelta);
				}
				
				if(CD3DDisplay::m_wParam == VK_RIGHT)
				{
					m_D3DObjectsArray[0]->yaw(5.0f,timeDelta);
				}

				// Change between cameras
				if(CD3DDisplay::m_wParam == VK_INSERT)
				{
					ChangeCamera();
				}

				// Change UAV we are following
				if(CD3DDisplay::m_wParam == VK_DELETE)
				{
					ChangeUAV();
				}
			break;
		}
	}
}


bool CD3DDisplay::SimulationComplete()
{
	bool Complete = true;

	// Update behavior of all objects
	for(int i =0;i < (int)m_D3DObjectsArray.size();i++)
	{
		
		// Check type
		char type[250];
		m_D3DObjectsArray[i]->getType(type);

		// If UAV we need to check it is still alive
		if(strcmp(type,"CD3DUAV") == 0)
		{
			// Will only return true when all UAVs have either crashed or completed their objectives
			if(!static_cast<CD3DUAV *>(m_D3DObjectsArray[i])->simulationComplete())
			{
				Complete = false;
			}
		}
	}

	return Complete;
}

/// Main game display code here
void CD3DDisplay::Draw(float timeDelta)
{

	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0L);
	m_pDevice->BeginScene();

	// Draw all objects
	for(int i =0;i < (int)m_D3DObjectsArray.size();i++)
	{
		m_D3DObjectsArray[i]->drawMesh();
	}

	//Caculate FPS
	m_dFrameCount++;
	m_fTimeElapsed += timeDelta;

	if(m_fTimeElapsed >= 1.0f)
	{
		m_fFPS = (float)m_dFrameCount / m_fTimeElapsed;

		sprintf(m_cFPSString, "%f", m_fFPS);
		m_cFPSString[49] = '\0'; // mark end of string

		m_fTimeElapsed = 0.0f;
		m_dFrameCount = 0;
	}

	// Display Debug of currently selected UAV
	m_D3DObjectsArray[m_UAVIndexArray[m_iCurrentUAVCameraIndex]]->getDebug(m_cDebugString);
	m_cDebugString[249] = '\0'; // mark end of string
	
	UpdateCamera();

	// Draw Text Last
	if(m_pFont)
	{
		m_pFont->DrawText(20, 50, 0xffffffff, m_cDebugString);
	}

	m_pDevice->EndScene();
	m_pDevice->Present(0, 0, 0, 0);

}

void CD3DDisplay::UpdateCamera()
{
	switch(m_iCurrentCameraIndex)
	{
		case CHASE: m_pCamera->UpdateChaseCameraMatrix(m_D3DObjectsArray[m_UAVIndexArray[m_iCurrentUAVCameraIndex]],2000.0f,700);
		break;
		case AERIAL: m_pCamera->UpdateAerialCameraMatrix(m_D3DObjectsArray[m_UAVIndexArray[m_iCurrentUAVCameraIndex]],m_fAerialCameraZoom);
		break;
		default: m_pCamera->UpdateChaseCameraMatrix(m_D3DObjectsArray[m_UAVIndexArray[m_iCurrentUAVCameraIndex]],2000.0f,700);
	}
}

void CD3DDisplay::DisplayMenu()
{
	
}