//////////////////////////////////////////////////////////////////////////////////////////////////
// CD3DViewObject.h
// Author: Tim Hobbs /some Frank Luna code
// Description: Holds all mesh data, for drawing the mesh and function to execute associate FX 
// files.
//      
// Version 1.2 -- Adding getBoundingLocalPosition, add this to your objects position to
// place the bounding sphere correctly
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_CD3DViewObject_H
#define INC_CD3DViewObject_H

#include <d3dx9.h>
#include <vector>
#include "CD3DBoundingSphere.h"
using std::vector;

class CD3DViewObject
{
public:
	CD3DViewObject();
	~CD3DViewObject();

	bool loadMeshIntoBuffer(char sysPath[], IDirect3DDevice9* localDevice);
	bool computeCD3DBoundingSphere();
	void drawObject();
	void drawCD3DBoundingSphere();
	void optmizeMesh();
	bool isPointInsideBoundingBox(D3DXVECTOR3* point,D3DXVECTOR3* currentWorldPosition);
	bool isPointIntersectingWithMesh(D3DXVECTOR3 rayObjOrigin,D3DXVECTOR3 rayObjDirection);
	ID3DXMesh* getMesh();
	float getBoundingRadius();
	D3DXVECTOR3 getBoundingLocalPosition();
	void cleanUP();
	DWORD VectorToRGB(D3DXVECTOR3* NormalVector);

	void setEffect(ID3DXEffect *_pEffect);
	void setEffect(LPCSTR effectName);

    template<class T> void Release(T t)
	{
		if(t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if(t)
		{
			delete t;
			t = 0;
		}
	}
	
private:
	DWORD  F2DW( FLOAT f );

	CD3DBoundingSphere sphere;

	IDirect3DDevice9* localDevice;
	ID3DXMesh* pMesh;
	ID3DXMesh* sphereMesh;
	
	std::vector<D3DMATERIAL9> Mtrls;              // Materials of mesh (and sub meshes)
	std::vector<IDirect3DTexture9*> Textures;     // Textures of mesh (and sub meshes)
	std::vector<ID3DXEffect*> Effects;            // Effect instances for mesh (and sub meshes)

	ID3DXBuffer* adjBuffer;
	ID3DXEffect* pEffect; // Compiled Effect to use on mesh object
	D3DXMATRIX* pWorld;

	DWORD dwTFactor;

	IDirect3DCubeTexture9 *m_pCubeTex;
	IDirect3DTexture9 *m_pNoiseTex;

};

#endif // INC_CD3DViewObject_H