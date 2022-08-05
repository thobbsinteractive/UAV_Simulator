//////////////////////////////////////////////////////////////////////////////////////////////////
// light.h
// Author: Tim Hobbs / Frank Luna
// Description: I really just put Franks code in a class for this one.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_LIGHT_H
#define INC_LIGHT_H

#include <d3dx9.h>

class CD3DLight
{
	public:
		CD3DLight(IDirect3DDevice9* pDevice);
		~CD3DLight();
		void InitDirectionalLight(D3DXVECTOR3 direction, D3DXCOLOR color);
		void InitPointLight(D3DXVECTOR3 position, D3DXCOLOR color);
		void InitSpotLight(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXCOLOR color);

	private:
		IDirect3DDevice9* m_pDevice;
		static int numberOfLightsInScene;

};
#endif //INC_LIGHT_H
