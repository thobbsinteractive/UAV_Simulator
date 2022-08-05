//////////////////////////////////////////////////////////////////////////////////////////////////
// CD3DFreeCamera.h
// Author: Tim Hobbs
// Description: A basic free camera class to control a scenes camera
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_CD3DFREECAMERA_H
#define INC_CD3DFREECAMERA_H

#include "boundary.h"
#include "viewObject.h"
#include "frustumPlane.h"
#include "vertex.h"
#include "DCD3DSound.h"

class CD3DFreeCamera
{
public:
	CD3DFreeCamera();
	CD3DFreeCamera(D3DXVECTOR3 _pos, ViewObject* _pMesh);
	CD3DFreeCamera(D3DXVECTOR3 _pos, D3DXVECTOR3 _rotation, D3DXVECTOR3 _look, ViewObject* _pMesh);
	CD3DFreeCamera(Boundary _objectBoundaries, ViewObject* _pMesh);
	virtual ~CD3DFreeCamera();

	D3DXVECTOR3 getCurrentDirectionalVector();

	bool checkIfVisibleToThisObject(CD3DFreeCamera* objectToCheck);
	
	void getPosition(D3DXVECTOR3* _pos);
	void setPosition(D3DXVECTOR3* _pos);
	
	void getCameraPosition(D3DXVECTOR3* _pos);

	void getRotation(D3DXVECTOR3* _rotation);
	void setRotation(D3DXVECTOR3* _rotation);
	void getLook(D3DXVECTOR3* _look); //
	void setLook(D3DXVECTOR3* _look);

	void setDevice(IDirect3DDevice9* _pDevice);

	void setProjectionForCamera(float _distanceZ, float _distanceY,float width, float height); // only needs to run once to setup camera projection
	void setFreeCameraMatrix();

protected:

	void updateCamera();

	D3DXMATRIX matProj; // Projection matrix for chase camera
	frustumPlane m_frustumPlanes[6]; // frustum from camera, help to cull objects

	D3DXVECTOR3 pos;
	D3DXVECTOR3 look;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 currentVector; // Current directional vector. Used in object culling

	D3DXVECTOR3 cameraPosition; // camera Position
	D3DXVECTOR3 cameraLookAt; // camera lookAt;
	D3DXVECTOR3 cameraUp; // camera up
	D3DXVECTOR3 angularVelocity;

	Boundary objectBoundaries;
	IDirect3DDevice9* pDevice;

};
#endif // INC_CD3DFREECAMERA_H