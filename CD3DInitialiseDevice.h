//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: capabilities.h
// Author: Tim Hobbs / Frank Luna (some initialisation code)
// Description: Starts up the 3d hardware, according to some capibilities
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_CD3DInitialiseDevice_H
#define INC_CD3DInitialiseDevice_H

#include <d3dx9.h>
#include<windows.h>

#include <commctrl.h>
#include <d3d9.h>

#include "Enumerated.h"

class CD3DInitialiseDevice
{
public:
	CD3DInitialiseDevice();
	~CD3DInitialiseDevice();

	bool getCapabilities(SETTINGS gameSettings,
					HINSTANCE hInstance,
					HWND* hwnd,
					IDirect3DDevice9** device);
private:
	bool initDevice(IDirect3D9* d3d9,
					HWND* hwnd,
					SETTINGS availableSettings,
					D3DFORMAT depth,
					int vp,
					D3DDEVTYPE deviceType,
					IDirect3DDevice9** device);
};

#endif //#ifndef INC_CD3DInitialiseDevice_H
