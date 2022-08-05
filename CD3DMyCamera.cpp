#include "CD3DMyCamera.h"

CD3DMyCamera::CD3DMyCamera(IDirect3DDevice9* device)
{
	m_pDevice = device;
	m_CameraPosition = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	m_CameraTarget = D3DXVECTOR3(0.0f,0.0f,1000000.0f);
	m_CameraWorldUp = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

CD3DMyCamera::~CD3DMyCamera()
{

}

D3DXMATRIX CD3DMyCamera::GetViewProjection()
{
	return m_View * m_Proj;
}

D3DXMATRIX CD3DMyCamera::GetProjection()
{
	return m_View * m_Proj;
}

D3DXMATRIX CD3DMyCamera::GetView()
{
	return m_View;
}

void CD3DMyCamera::SetPosition(D3DXVECTOR3 pos)
{
	m_CameraPosition = pos;
}

void CD3DMyCamera::SetTarget(D3DXVECTOR3 pos)
{
	m_CameraTarget = pos;
}

void CD3DMyCamera::SetWorldUp(D3DXVECTOR3 pos)
{
	m_CameraWorldUp = pos;
}

D3DXVECTOR3 CD3DMyCamera::GetCameraPosition()
{
	return m_CameraPosition;
}

D3DXVECTOR3 CD3DMyCamera::GetCameraTarget()
{
	return m_CameraTarget;
}

D3DXVECTOR3 CD3DMyCamera::GetCameraWorldUp()
{
	return m_CameraWorldUp;
}

// it this one or the other NOT both
void CD3DMyCamera::UpdateChaseCameraMatrix(CD3DObject *objectToChase, float chaseDistanceX,float chaseDistanceY)
{

	D3DXVECTOR3 object_pos;
	D3DXVECTOR3 object_rotation;

	object_pos = objectToChase->getPosition();
	object_rotation = objectToChase->getRotation();

	m_CameraPosition = object_pos;
	m_CameraTarget = object_pos;

	D3DXVECTOR3 look;
	D3DXVECTOR3 newLook;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;

	float HalfPi = D3DX_PI/2;

	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Calculate Camera look at coordinates 
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,object_rotation.x,object_rotation.y,object_rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	m_CameraTarget += normedVector * chaseDistanceX;
	m_CameraPosition -= normedVector * chaseDistanceX;

	
	// Calculates Vertical Camera Position
	
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,object_rotation.x,object_rotation.y+HalfPi,object_rotation.z);
	D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
	D3DXVec3Normalize(&normedVector, &newLook );

	//m_CameraTarget -= normedVector * chaseDistanceY;
	m_CameraPosition -= normedVector * chaseDistanceY;

	m_CameraWorldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	
	// Stops the camera being flipped upside down when rotating up
	if(((object_rotation.y*(180/D3DX_PI)) > 90)&&((object_rotation.y*(180/D3DX_PI)) < 270))
	{
		m_CameraWorldUp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}else
	{
		m_CameraWorldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// When done update matrix
	UpdateCameraMatrix();
}

void CD3DMyCamera::UpdateAerialCameraMatrix(CD3DObject *objectToChase, float cameraElevation)
{

	D3DXVECTOR3 object_pos = objectToChase->getPosition();

	m_CameraPosition = D3DXVECTOR3(object_pos.x,object_pos.y + cameraElevation,object_pos.z);
	m_CameraTarget = object_pos;

	m_CameraWorldUp = D3DXVECTOR3(0.0f, 0.0f,1.0f);

	// When done update matrix
	UpdateCameraMatrix();
}

void CD3DMyCamera::UpdateCameraMatrix()
{
	// View Space Transformation, needs to be run every frame.

	// Computer View Space Matrix
	D3DXMatrixLookAtLH(&m_View, &m_CameraPosition, &m_CameraTarget, &m_CameraWorldUp);

	// View Space Transformation (transforms everything along the axis of the camrea to render)
	m_pDevice->SetTransform(D3DTS_VIEW,&m_View);
}

void CD3DMyCamera::SetUpCamera(int width,int height)
{
	D3DXMatrixPerspectiveFovLH(
		&m_Proj,
		D3DX_PI * 0.25f,
		(float)width / (float)height,
		1.0f,
		10000000000.0f);

	// Setup the projection params (so that the 3D image can be 
	// converted to 2D
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_Proj);
}

