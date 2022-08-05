/* Name: CD3DViewObject.cpp
   Description: The methods for the CD3DViewObject class
   This class loads a requested mesh from a file into the vertex buffer.
   The mesh is then optimized and when requested drawn. 
   
   This class is seperate from the object transform class because you may 
   have 20 ojects with the same mesh and in doing so, you do not want to 
   needlessly fill the vertex buffer with the same mesh data.
*/
#include "CD3DViewObject.h"

CD3DViewObject::CD3DViewObject()
{
	adjBuffer = 0;
}

CD3DViewObject::~CD3DViewObject()
{
	cleanUP();
}


bool CD3DViewObject::loadMeshIntoBuffer(char sysPath[],IDirect3DDevice9* Device)
{
	// save a ptr to the device in this object
	localDevice = Device;
	ID3DXBuffer* mtrlBuffer = 0;
	ID3DXEffect* pEffectBuffer = 0;
	DWORD numMtrls = 0;
	
	bool result = true;				// Initialise return result

	//MessageBox(NULL, sysPath, "Meshes.exe", MB_OK);

    // D3DXLoadMeshFromX
    // Load the mesh from the specified file
	if( FAILED( D3DXLoadMeshFromXA( sysPath, 
									D3DXMESH_MANAGED, 
                                    localDevice,
									&adjBuffer,
                                    &mtrlBuffer, 
									0,               //effect instance Modify to retrieve
									&numMtrls, 
                                    &pMesh ) ) )
    {
		MessageBox(NULL, L"Could not find Mesh", L"Meshes.exe", MB_OK);
        result = false;
    }

	if (result)
	{
	//
	// Extract the materials, and load textures for mesh.
	//

	if( mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < (int)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFileA(
					localDevice,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}

	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer
	}
	
	if(pMesh)
	{
		optmizeMesh();
		computeCD3DBoundingSphere();
	}

    return result;
};

void CD3DViewObject::optmizeMesh()
{
	//
	// Optimize the mesh.
	//
	HRESULT hr = NULL;

	hr = pMesh->OptimizeInplace(		
	D3DXMESHOPT_ATTRSORT |
	D3DXMESHOPT_COMPACT  |
	D3DXMESHOPT_VERTEXCACHE,
	(DWORD*)adjBuffer->GetBufferPointer(),
	0, 0, 0);

	if(FAILED(hr))
	{
		::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
	}
};

D3DXVECTOR3 CD3DViewObject::getBoundingLocalPosition()
{
	return sphere.localPos;
}

bool CD3DViewObject::computeCD3DBoundingSphere()
{
	HRESULT hr = 0;

	BYTE* v = 0;
	pMesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)v,
			pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(pMesh->GetFVF()),
			&sphere.localPos,
			&sphere.radius);
	
	//sphere.radius = sphere.radius/2;

	pMesh->UnlockVertexBuffer();

	D3DXCreateSphere(
		localDevice,
		sphere.radius,
		20,
		20,
		&sphereMesh,
		0);

	if( FAILED(hr) )
		return false;

	return true;
}

bool CD3DViewObject::isPointInsideBoundingBox(D3DXVECTOR3* point,D3DXVECTOR3* currentWorldPosition)
{
	sphere.setWorldPosition(*currentWorldPosition);
	return sphere.isPointInside(*point);
}

bool CD3DViewObject::isPointIntersectingWithMesh(D3DXVECTOR3 rayObjOrigin,D3DXVECTOR3 rayObjDirection)
{
	BOOL hasHit = false;
	float distanceToCollision = 0;

	D3DXIntersect(pMesh, &rayObjOrigin, &rayObjDirection, &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL);

	if((distanceToCollision != 0.0f)&&(distanceToCollision < 1000.0f)&&(hasHit == true))
	{
		return true;	
	}
	return false;
}

ID3DXMesh* CD3DViewObject::getMesh()
{
	return pMesh;
}

float  CD3DViewObject::getBoundingRadius()
{
	return sphere.radius;
}

// Draws the mesh (with all submeshes and their associated materials and textures)
void CD3DViewObject::drawObject()
{	
	
	for(int i = 0; i < (int)Mtrls.size(); i++)
	{
		localDevice->SetMaterial(&Mtrls[i]);
		localDevice->SetTexture( 0,Textures[i]);
		pMesh->DrawSubset(i);
	}
	localDevice->SetTexture(0, NULL);
};

DWORD  CD3DViewObject::F2DW( FLOAT f ) { 
	return *((DWORD*)&f); 
}

// Just draws the meshs bounding sphere
void CD3DViewObject::drawCD3DBoundingSphere()
{
	sphereMesh->DrawSubset(0);
};

void CD3DViewObject::cleanUP()
{
	// release meshes
	Release<ID3DXMesh*>(sphereMesh);
	Release<ID3DXMesh*>(pMesh);

//	sphereMesh->Release();
	//if(pMesh)
	//{
	//	pMesh->Release();
	//}

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	for(int i = 0; i < (int)Textures.size(); i++)
		Release<IDirect3DTexture9*>( Textures[i] );

	Mtrls.clear();
};