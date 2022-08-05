#ifndef INC_CD3DMyCamera_H
#define INC_CD3DMyCamera_H

#include <d3dx9.h>
#include "CD3DObject.h"

class CD3DMyCamera
{
public:
	CD3DMyCamera(IDirect3DDevice9* device);
	~CD3DMyCamera();

	void SetUpCamera(int width,int height);

	void SetTarget(D3DXVECTOR3 pos);
	void SetPosition(D3DXVECTOR3 pos);
	void SetWorldUp(D3DXVECTOR3 pos);

	void UpdateCameraMatrix();
	void UpdateChaseCameraMatrix(CD3DObject *objectToChase, float chaseDistanceX,float chaseDistanceY);
	void UpdateAerialCameraMatrix(CD3DObject *objectToChase, float cameraElevation);

	D3DXVECTOR3 GetCameraPosition();
	D3DXVECTOR3 GetCameraTarget();
	D3DXVECTOR3 GetCameraWorldUp();

	D3DXMATRIX GetProjection();
	D3DXMATRIX GetView();
	D3DXMATRIX GetViewProjection();

private:
	D3DXVECTOR3 m_CameraPosition;
	D3DXVECTOR3 m_CameraTarget;
	D3DXVECTOR3 m_CameraWorldUp;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_View;

	IDirect3DDevice9* m_pDevice;
};

#endif // INC_CD3DMyCamera_H
