#include "CD3DInitialiseDevice.h"

CD3DInitialiseDevice::CD3DInitialiseDevice(){
}

CD3DInitialiseDevice::~CD3DInitialiseDevice(){
}


bool CD3DInitialiseDevice::getCapabilities(SETTINGS gameSettings,
										   HINSTANCE hInstance,
										   HWND* hwnd,
										   IDirect3DDevice9** device)	// [out] A pointer to a pointer of the created device)
{
	IDirect3D9* d3d9; // COM interface to create device

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	// Initialize D3DCAPS9 with actual Hardware Capabilities
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
			D3DADAPTER_DEFAULT, // [in] Denotes primary display adapter.
			D3DDEVTYPE_HAL,		// [in] Specifies the device type
			&caps);				// Return out cap pointer with a filled D3DCAPS9 structure (called "caps")
								// that contains the capabilities of primary display adapter.


	// Can we use hardware vertex processing?
	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// Yes, save in 'vp' that Hardware transform and lighting 
		// is supported.
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		// no, save in 'vp' the fact we must use software vertex 
		// processing.
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if(!(caps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3))
	{
		
	}
	
	
	//CheckDeviceCaps
	if(gameSettings.Resolution.Height > (int)caps.MaxTextureHeight)
	{
		gameSettings.Resolution.Height = (int)caps.MaxTextureHeight;
	}

	if(gameSettings.Resolution.Width > (int)caps.MaxTextureWidth)
	{
		gameSettings.Resolution.Width = (int)caps.MaxTextureWidth;
	}


	//temp

	D3DFORMAT depth = D3DFMT_D24S8;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

	return initDevice(d3d9,
		              hwnd,
					  gameSettings,
					  depth,
					  vp,
					  deviceType,device);
};

bool CD3DInitialiseDevice::initDevice(IDirect3D9* d3d9,
							  HWND* hwnd,
							  SETTINGS availableSettings,
							  D3DFORMAT depth,
							  int vp,
							  D3DDEVTYPE deviceType,
							  IDirect3DDevice9** device)
{

	HRESULT hr = 0;

	// Create and fill out an instance of the D3DPRESENT_PARAMETERS 
	// structure.
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth				= availableSettings.Resolution.Width;
	d3dpp.BackBufferHeight              = availableSettings.Resolution.Height;
	d3dpp.BackBufferFormat              = D3DFMT_A8R8G8B8; // pixel format
	d3dpp.BackBufferCount				= 1;
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality            = 0;
	d3dpp.SwapEffect                    = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow					= *hwnd;
	d3dpp.Windowed						= availableSettings.Windowed; // Full Screen
	d3dpp.EnableAutoDepthStencil		= true;
	d3dpp.AutoDepthStencilFormat        = depth; // 24 bit depth buffer
	d3dpp.Flags							= 0;
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval          = D3DPRESENT_INTERVAL_IMMEDIATE;

	// With the D3DPresent parameters filled out we can create the 
	// IDirect3DDevice9 object and point to it using the 'device' pointer.
 
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,	// primary adapter
							deviceType,         // our device type
							*hwnd,				// window associated with device
							vp,					// vertex processing type
					     	&d3dpp,				// our present parameters
						 	device);            // returned created device

	if( FAILED(hr) )
	{
		d3d9->Release(); // done with d3d9 object
		::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
		return false;
	}

	d3d9->Release(); // done with d3d9 object
	return true;
};