//////////////////////////////////////////////////////////////////////////////////////////////////
// CD3DObject.h
// Author: Tim Hobbs
// Description: A basic super class for all objects in the world, holding their positions and 
// sound and now also a pointer to their mesh data.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_CD3DObject_H
#define INC_CD3DObject_H

#include "boundary.h"
#include "CD3DViewObject.h"
#include "frustumPlane.h"
#include "vertex.h"
#include "CD3DSound.h"

class CD3DObject
{
public:
	CD3DObject();
	CD3DObject(D3DXVECTOR3 _pos, CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice);
	CD3DObject(D3DXVECTOR3 _pos, D3DXVECTOR3 _rotation, D3DXVECTOR3 _look, CD3DViewObject* _pMesh);
	CD3DObject(Boundary _objectBoundaries, CD3DViewObject* _pMesh);
	virtual ~CD3DObject();

	D3DXVECTOR3 getCurrentDirectionalVector();
	bool checkIfVisibleToThisObject(CD3DObject* objectToCheck);
	D3DXVECTOR3 getPosition();

	void setPosition(D3DXVECTOR3 _pos);
	D3DXVECTOR3 getCameraPosition();

	D3DXVECTOR3 getRotation();
	void setRotation(D3DXVECTOR3 _rotation);

	D3DXVECTOR3 getLook(); //
	void setLook(D3DXVECTOR3 _look);

	void setScale(D3DXVECTOR3 _pScale);

	void addMesh(CD3DViewObject* _pMesh);
	void setMesh(CD3DViewObject* _pMesh,int index);
	int  getNumberOfMeshs();
	void getMesh(CD3DViewObject** _pMesh,int index);

	void setDevice(IDirect3DDevice9* _pDevice);
	
	// collison detection
	bool highlevelCollisionDetection(D3DXVECTOR3 objectsPosition);
	bool highlevelCollisionDetection(CD3DObject* objectToCheckAgainst);
	bool lowlevelCollisionDetection(CD3DObject* objectToCheckAgainst);
	bool lowlevelMeshCollisionDetection(CD3DObject* objectToCheckAgainst);

	// Distance calcuation
	float relativeDistanceToObject(D3DXVECTOR3 position);
	float relativeDistanceToObject(CD3DObject *object);

	void drawWithoutMatrix(int index);
	float addAngle(float angleToAdd,float existingAngle);

	void updatePositionInRelationTo(D3DXVECTOR3 offsetPostion,D3DXVECTOR3 rotationOfObject,D3DXVECTOR3* newPostion);

	void virtual walk(float units, float timeDelta); // forward/backward
	void virtual pitch(float angle, float timeDelta); // rotate on left vector
	void virtual yaw(float angle, float timeDelta); // rotate on up vector
	void virtual roll(float angle ,float timeDelta); // rotate on look vector
	D3DXMATRIX getMatrix();
	void addObjectSound(LPTSTR wavFileName, HWND handle,CSoundManager* sound3D);
	void playSound(int soundNumber,D3DXVECTOR3 listenerPosition);
	void virtual drawMesh();
	void virtual drawMesh(int index);
	void drawMeshBounding();
	float getPosX();
	float getPosY();
	float getPosZ();


	// Based on http://www.c-unit.com/tutorials/mdirectx/?t=45
	// and 
	// http://www.toymaker.info/Games/html/direct3d_faq.html#D3D5

	void buildViewFrustum(D3DXMATRIX _matView);
	bool SphereInFrustum(CD3DObject *object);
	void buildFrustum(float aspectWidth = 800.0f, float aspectHeight = 600.0f, 
					  float viewingAngle = 0.25, float radarRange = 1000000.0f);
	float getBoundingRadius();

	void setEffect(ID3DXEffect *_pEffect);

	// Virtual Should be implemented in every derived class
	void virtual getType(char* ptype);
	void virtual getDebug(char* pType);

protected:

	float distanceZ;
	float distanceY;

	D3DXMATRIX matWorld;
	frustumPlane m_frustumPlanes[6]; // frustum from camera, help to cull objects

	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 look;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 currentVector; // Current directional vector. Used in object culling

	D3DXVECTOR3 angularVelocity;

	Boundary objectBoundaries;
	std::vector<CD3DSound> sound;  //can have a number of associated sounds
	std::vector<CD3DViewObject*> pMeshObjects; //can have a number of meshes
	IDirect3DDevice9* pDevice;
	float fOrientation;
	int vertexCount;

	struct MESHVERTEX 
	{ 
		D3DXVECTOR3 p; 
		D3DXVECTOR3	n; 
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;;
	};
	//const DWORD MESHVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

};
#endif // INC_CD3DObject_H