//////////////////////////////////////////////////////////////////////////////////////////////////
// objectTransform.h
// Author: Tim Hobbs
// Description: A container class for 3D sound objects, code has largly been nicked from Shootemup
// , Thanks Nigel
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INCCD3DSOUND_H
#define INCCD3DSOUND_H

#include <d3dx9.h>
#include "dsutil.h"  //Microsoft helper class.
#include "dxutil.h"

class CD3DSound
{
public:
	
	CD3DSound();
	~CD3DSound();
	void initialiseSound(IDirect3DDevice9* _pDevice, LPTSTR wavFileName, HWND handle,CSoundManager *_sound3Dmanager);
	void setSoundPos(D3DXVECTOR3 position,float speed);
	void setListenerSoundPos(D3DXVECTOR3 position,float speed);

	void playSound3D();
    void playSound3D(unsigned long int dwPriority, unsigned long int dwFlags);

	void Set3DParameters(float fDopplerFactor, float fRolloffFactor,
                         float fMinDistance,   float fMaxDistance,
						 D3DXVECTOR3 soundPosition);
	void stop();
	void reset();

private:
	D3DXVECTOR3 soundPos;					  // All these are properties of the 3d sound
	
	LPDIRECTSOUND3DBUFFER   pDS3DBuffer;      // 3D sound buffer
    LPDIRECTSOUND3DLISTENER pDSListener;      // 3D listener object
    DS3DBUFFER              dsBufferParams;   // 3D buffer properties
    DS3DLISTENER            dsListenerParams; // Listener properties
    BOOL                    bDeferSettings;
    CSound                  *pSound3D;
	
	IDirect3DDevice9* pLocalDevice;

	CSoundManager *sound3Dmanager;
};

#endif INCCD3DSOUND_H
