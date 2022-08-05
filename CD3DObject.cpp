/* Name: CD3DObject.cpp
   Description: The methods for the CD3DObject class
   
   This class holds an objects current positional coordinates and can be used to
   rotate an object on its own axis according to its own positiontion information.
   This is very useful for User and AI controlled objects.

   THIS IS NOW THE BEST VERSION: 1.0
*/
#include "CD3DObject.h"

CD3DObject::CD3DObject()
{
	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // In radians
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	objectBoundaries = Boundary(D3DXVECTOR3(FLT_MAX, FLT_MAX,FLT_MAX),D3DXVECTOR3(-FLT_MAX,-FLT_MAX, -FLT_MAX));

	distanceZ=0;
	distanceY=0;
	vertexCount = 0;
};

CD3DObject::CD3DObject(D3DXVECTOR3 _pos, CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice)
{
	pos   = _pos;
	pDevice = _pDevice;

	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	objectBoundaries = Boundary(D3DXVECTOR3(FLT_MAX, FLT_MAX,FLT_MAX),D3DXVECTOR3(-FLT_MAX,-FLT_MAX, -FLT_MAX));
	
	pMeshObjects.push_back(_pMesh);

	distanceZ=0;
	distanceY=0;
	vertexCount = 0;

};

CD3DObject::CD3DObject(Boundary _objectBoundaries, CD3DViewObject* _pMesh)
{
	objectBoundaries = _objectBoundaries;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pMeshObjects.push_back(_pMesh);

	distanceZ=0;
	distanceY=0;
	vertexCount = 0;
}

CD3DObject::CD3DObject(D3DXVECTOR3 _pos, D3DXVECTOR3 _rotation, D3DXVECTOR3 _look, CD3DViewObject* _pMesh)
{
	D3DXVECTOR3 pos = _pos;
	D3DXVECTOR3 rotation = _rotation;
	D3DXVECTOR3 look = _look;
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	objectBoundaries = Boundary(D3DXVECTOR3(10000.0f, 1000.0f,10000.0f),D3DXVECTOR3(-10000.0f, 100.0f, -10000.0f));
	pMeshObjects.push_back(_pMesh);

	distanceZ=0;
	distanceY=0;
	vertexCount = 0;
};

CD3DObject::~CD3DObject()
{
	for(int i =0;i < (int)sound.size();i++)
	{
		//erase all sounds
		sound.erase(sound.begin() + i);
	}

	for(int i =0;i < (int)pMeshObjects.size();i++)
	{
		//erase all meshes
		pMeshObjects.erase(pMeshObjects.begin() + i);
	}
	printf("CD3DObject deconstructor run");
	
};


void CD3DObject::getDebug(char* pType)
{
	char m_cDebugString[250];
	sprintf(m_cDebugString, "Object: x:%f y:%f z:%f",pos.x,pos.y,pos.z);
	m_cDebugString[249] = '\0'; // mark end of string

	strcpy(pType,m_cDebugString);
};

void CD3DObject::getType(char* pType)
{
	strcpy(pType,"CD3DObject");
};

D3DXVECTOR3 CD3DObject::getPosition()
{
	return pos;
};

void CD3DObject::setPosition(D3DXVECTOR3 _pos)
{
	pos = _pos;
};

D3DXVECTOR3 CD3DObject::getRotation()
{
	return rotation;
};

void CD3DObject::setRotation(D3DXVECTOR3 _rotation)
{
	rotation = _rotation;
};
D3DXVECTOR3 CD3DObject::getLook()
{
	return look;
};

void CD3DObject::setLook(D3DXVECTOR3 _look)
{
	look = _look;
};

void CD3DObject::setScale(D3DXVECTOR3 _pScale)
{
	scale = _pScale;
};

void CD3DObject::setDevice(IDirect3DDevice9* _ppDevice)
{
	pDevice = _ppDevice;
}

void CD3DObject::addObjectSound(LPTSTR wavFileName, HWND handle,CSoundManager* sound3Dmanager)
{
	//if(_sound)
	//{
		CD3DSound temp;
		temp.initialiseSound(pDevice,wavFileName,handle,sound3Dmanager);
		sound.push_back(temp);
	//}
};

void CD3DObject::playSound(int soundNumber,D3DXVECTOR3 listenerPosition)
{
	if((int)sound.size() > 0)
	{
		sound[soundNumber].setSoundPos(pos,0.0f);
		sound[soundNumber].playSound3D();
	}
}

float CD3DObject::getPosX()
{
	return pos.x;
};

float CD3DObject::getPosY()
{
	return pos.y;
};

float CD3DObject::getPosZ()
{
	return pos.z;
};

bool CD3DObject::checkIfVisibleToThisObject(CD3DObject* objectToCheck)
{
	return SphereInFrustum(objectToCheck);
}

float CD3DObject::getBoundingRadius()
{
	return pMeshObjects[0]->getBoundingRadius();
}

D3DXVECTOR3 CD3DObject::getCurrentDirectionalVector()
{
	return currentVector;
}

void CD3DObject::walk(float units, float timeDelta)
{
	units = units * timeDelta;
	// Mainly nicked from "thing3d", Thanks Nigel.

	D3DXVECTOR3 newLook;
	D3DXVECTOR3 posTemp;
	D3DXMATRIX RotationMatrix;

	posTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);

    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize( &currentVector, &newLook );
	
	posTemp.x = pos.x;
	posTemp.y = pos.y;
	posTemp.z = pos.z;

	posTemp += currentVector * units;

	//Test the new position first, if its okay update
	if((posTemp.x < objectBoundaries.Max.x) && (posTemp.x > objectBoundaries.Min.x))
	{
		pos.x = posTemp.x;
	}
		
	if((posTemp.y < objectBoundaries.Max.y) && (posTemp.y > objectBoundaries.Min.y))
	{
		pos.y = posTemp.y;
	}
		
	if((posTemp.z < objectBoundaries.Max.z) && (posTemp.z > objectBoundaries.Min.z))
	{
		pos.z = posTemp.z;
	}

	// update the sound position every time the object moves
	for(int i = 0; i < (int)sound.size(); i++)
	{
		sound[i].setSoundPos(pos,0);
	}
}

float CD3DObject::addAngle(float angleToAdd,float existingAngle)
{
	float angleInDegrees = angleToAdd*(180/D3DX_PI);
	float rotationInDegrees = existingAngle*(180/D3DX_PI);
	float answer = 0.0f;
	bool finished = false;

	// Ensures all angles stay with limits

	// 0 to 360
	if(((rotationInDegrees + angleInDegrees) <= 360)&&
		((rotationInDegrees + angleInDegrees) >= 0)&&
		(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees;
		finished = true;
	}

	// 360 to 420
	if(((rotationInDegrees + angleInDegrees) > 360)&&(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees -360;
		finished = true;
	}

	// 0 to -360
	if(((rotationInDegrees + angleInDegrees) < 0)&&
		((rotationInDegrees + angleInDegrees) >= -360)&&
		(finished == false))
	{
		answer = 360 + (rotationInDegrees + angleInDegrees);
		finished = true;
	}

	return answer * (D3DX_PI/180); // In radians
}

void CD3DObject::pitch(float angle,float timeDelta)
{
	angle = angle * timeDelta;

	float angleInDegrees = angle*(180/D3DX_PI);
	float rotationInDegrees = rotation.y*(180/D3DX_PI);
	float answer = 0.0f;
	bool finished = false;

	// Ensures all angles stay with limits

	// 0 to 360
	if(((rotationInDegrees + angleInDegrees) <= 360)&&
		((rotationInDegrees + angleInDegrees) >= 0)&&
		(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees;
		finished = true;
	}

	// 360 to 420
	if(((rotationInDegrees + angleInDegrees) > 360)&&(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees -360;
		finished = true;
	}

	// 0 to -360
	if(((rotationInDegrees + angleInDegrees) < 0)&&
		((rotationInDegrees + angleInDegrees) >= -360)&&
		(finished == false))
	{
		answer = 360 + (rotationInDegrees + angleInDegrees);
		finished = true;
	}
	
	rotation.y = answer * (D3DX_PI/180); // In radians
}

void CD3DObject::yaw(float angle,float timeDelta)
{
	angle = angle * timeDelta;
	float angleInDegrees = angle*(180/D3DX_PI);
	float rotationInDegrees = rotation.x*(180/D3DX_PI);
	float answer = 0.0f;
	bool finished = false;

	// Ensures all angles stay with limits

	// 0 to 360
	if(((rotationInDegrees + angleInDegrees) <= 360)&&
		((rotationInDegrees + angleInDegrees) >= 0)&&
		(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees;
		finished = true;
	}

	// 360 to 420
	if(((rotationInDegrees + angleInDegrees) > 360)&&(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees -360;
		finished = true;
	}

	// 0 to -360
	if(((rotationInDegrees + angleInDegrees) < 0)&&
		((rotationInDegrees + angleInDegrees) >= -360)&&
		(finished == false))
	{
		answer = 360 + (rotationInDegrees + angleInDegrees);
		finished = true;
	}
	
	rotation.x = answer * (D3DX_PI/180); // In radians
}

void CD3DObject::roll(float angle,float timeDelta)
{
	angle = angle * timeDelta;
	float angleInDegrees = angle*(180/D3DX_PI);
	float rotationInDegrees = rotation.z*(180/D3DX_PI);
	float answer = 0.0f;
	bool finished = false;

	// Ensures all angles stay with limits

	// 0 to 360
	if(((rotationInDegrees + angleInDegrees) <= 360)&&
		((rotationInDegrees + angleInDegrees) >= 0)&&
		(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees;
		finished = true;
	}

	// 360 to 420
	if(((rotationInDegrees + angleInDegrees) > 360)&&(finished == false))
	{
		answer = rotationInDegrees + angleInDegrees -360;
		finished = true;
	}

	// 0 to -360
	if(((rotationInDegrees + angleInDegrees) < 0)&&
		((rotationInDegrees + angleInDegrees) >= -360)&&
		(finished == false))
	{
		answer = 360 + (rotationInDegrees + angleInDegrees);
		finished = true;
	}
	
	rotation.z = answer * (D3DX_PI/180); // In radians
}

void CD3DObject::updatePositionInRelationTo(D3DXVECTOR3 offsetPostion,D3DXVECTOR3 rotationOfObject,D3DXVECTOR3* newPostion)
{

	D3DXVECTOR3 look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 newLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	*newPostion = pos; // equals the postion of this object

	float radins = 270 * (D3DX_PI/180);

	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Calculate new coordinates 
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotationOfObject.x+radins,rotationOfObject.y,rotationOfObject.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	*newPostion += normedVector * offsetPostion.x;

	look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	newLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normedVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotationOfObject.x,rotationOfObject.y+radins,rotationOfObject.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	*newPostion += normedVector * offsetPostion.y;

	look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	newLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normedVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotationOfObject.x,rotationOfObject.y,rotationOfObject.z+radins);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	*newPostion += normedVector * offsetPostion.z;
}

D3DXMATRIX CD3DObject::getMatrix()
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	matWorld = Scaling * RotationMatrix * Translation;
	return matWorld;
}


void CD3DObject::drawMesh()
{	
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);
	
	matWorld = Scaling * RotationMatrix * Translation;

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	pMeshObjects[0]->drawObject();
}

void CD3DObject::drawMesh(int index)
{
	if((int)pMeshObjects.size() > 0)
	{
		D3DXMATRIX RotationMatrix;
		D3DXMATRIX Translation;
		D3DXMATRIX Scaling;

		// Set meshes scale
		D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);

		// Set the meshes rotation (in the world)
		D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);

		// Set the meshes location in the world
		D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);
	
		// Build a combined matrix of all three
		matWorld = Scaling * RotationMatrix * Translation;

		// Send that combined matrix to device
		pDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// It will now draw the object at the location/rotation/scale provided
		pMeshObjects[index]->drawObject();
	}
}

// Gets a pointer to the objects mesh
void CD3DObject::getMesh(CD3DViewObject** _pMesh,int index)
{
	///*_pMesh = 0;

	if(index < pMeshObjects.size())
	{
		*_pMesh = pMeshObjects[index];
	}
}

void CD3DObject::addMesh(CD3DViewObject* _pMesh)
{
	if(_pMesh)
	{
		pMeshObjects.push_back(_pMesh);
	}
}
	
void CD3DObject::setMesh(CD3DViewObject* _pMesh,int index)
{
	if((_pMesh)&&(index < pMeshObjects.size()))
	{
		pMeshObjects[index] = _pMesh;
	}
}

int CD3DObject::getNumberOfMeshs()
{
	return pMeshObjects.size();
}


bool CD3DObject::highlevelCollisionDetection(CD3DObject* objectToCheckAgainst){
	
	CD3DViewObject* objectsMesh;
	D3DXVECTOR3 objectsPosition;

	objectToCheckAgainst->getMesh(&objectsMesh,0);
	objectsPosition = objectToCheckAgainst->getPosition();

	// We have to allow for the spheres center position a relation of the pos of the model (at the base of the building)
	D3DXVECTOR3 localSpherePosition = objectsMesh->getBoundingLocalPosition();
	objectsPosition = objectsPosition + localSpherePosition;

	return objectsMesh->isPointInsideBoundingBox(&objectsPosition,&pos);
}

bool CD3DObject::highlevelCollisionDetection(D3DXVECTOR3 objectsPosition)
{
	return pMeshObjects[0]->isPointInsideBoundingBox(&objectsPosition,&pos);
}

bool CD3DObject::lowlevelCollisionDetection(CD3DObject* objectToCheckAgainst)
{
	D3DXVECTOR3 objectsPosition;

	objectsPosition = objectToCheckAgainst->getPosition();

	// Use inverse of matrix
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,NULL,&matWorld);

	// Transform ray origin and direction by inv matrix
	D3DXVECTOR3 rayObjOrigin,rayObjDirection;
	D3DXVECTOR3 rayOrigin,rayDirection;
	rayOrigin = objectToCheckAgainst->getPosition();
	rayDirection = objectToCheckAgainst->getRotation();

	//D3DXVec3TransformCoord(&rayObjOrigin,&rayOrigin,&matInverse);
	//D3DXVec3TransformNormal(&rayObjDirection,&rayDirection,&matInverse);
	D3DXVec3TransformCoord(&rayObjOrigin,&pos,&matInverse);
	D3DXVec3TransformNormal(&rayObjDirection,&rotation,&matInverse);
	D3DXVec3Normalize(&rayObjDirection,&rayObjDirection);

	//We can now call the intersect function on our untransformed graphic mesh data:
	return pMeshObjects[0]->isPointIntersectingWithMesh(rayObjOrigin,rayObjDirection);
}

bool CD3DObject::lowlevelMeshCollisionDetection(CD3DObject* objectToCheckAgainst)
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXVECTOR3 newLook;

	IDirect3DVertexBuffer9* _vb;
	MESHVERTEX* pVertices;

	bool hasHit = false;

	// Get the object to check againsts mesh
	CD3DViewObject* objectsViewMesh;
	objectToCheckAgainst->getMesh(&objectsViewMesh,0);
	// Gets the UAV mesh object
	ID3DXMesh* pMesh = pMeshObjects[0]->getMesh();

	// Get the World Matrix for this UAV
	D3DXMATRIX UAVMeshWorld = getMatrix();
	// Get the World Matrix for the D3DObject we are checking against
	D3DXMATRIX objectsMatrix = objectToCheckAgainst->getMatrix();

	// Get inverse of the object we are checking againts world matrix 
	// (to transform any vectors we create to the mesh model space)
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,NULL,&objectsMatrix);

	// We will populate these to create a Ray from vectors created from each vertex of our UAV Model
	D3DXVECTOR3 rayOrigin,rayDirection;


	// Creates a directional vector for the direction the UAV is flying in
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize( &rayDirection, &newLook );

	// locks the memory of the cards where the mesh is held, so we can extract vertex data
	pMesh->GetVertexBuffer(&_vb);
	_vb->Lock(0, 0, (void**)&pVertices, 0);
	int numberOfVertexs = pMesh->GetNumVertices();

	// Nose
	// Test vetrex 258
	rayOrigin = pVertices[258].p;

	// Transform vertex's position to world space
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&UAVMeshWorld);

	// Transform vertex's postion into model (local) space of the object we are checking against
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&matInverse);
	D3DXVec3TransformNormal(&rayDirection,&rayDirection,&matInverse);
	D3DXVec3Normalize(&rayDirection,&rayDirection);

	//We can now call the intersect function on our untransformed graphic mesh data:
	if (objectsViewMesh->isPointIntersectingWithMesh(rayOrigin,rayDirection))
	{
		hasHit = true;
	}

	// Wings
	// Test vetrex 1807
	rayOrigin = pVertices[1807].p;

	// Transform vertex's position to world space
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&UAVMeshWorld);

	// Transform vertex's postion into model (local) space of the object we are checking against
	// This time we do not need to normalize as rayDirection is the same
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&matInverse);

	//We can now call the intersect function on our untransformed graphic mesh data:
	if (objectsViewMesh->isPointIntersectingWithMesh(rayOrigin,rayDirection))
	{
		hasHit = true;
	}

	// Test vetrex 80
	rayOrigin = pVertices[80].p;

	// Transform vertex's position to world space
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&UAVMeshWorld);

	// Transform vertex's postion into model (local) space of the object we are checking against
	// This time we do not need to normalize as rayDirection is the same
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&matInverse);

	//We can now call the intersect function on our untransformed graphic mesh data:
	if (objectsViewMesh->isPointIntersectingWithMesh(rayOrigin,rayDirection))
	{
		hasHit = true;
	}

	// Tail
	// Test vetrex 1713
	rayOrigin = pVertices[1713].p;

	// Transform vertex's position to world space
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&UAVMeshWorld);

	// Transform vertex's postion into model (local) space of the object we are checking against
	// This time we do not need to normalize as rayDirection is the same
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&matInverse);

	//We can now call the intersect function on our untransformed graphic mesh data:
	if (objectsViewMesh->isPointIntersectingWithMesh(rayOrigin,rayDirection))
	{
		hasHit = true;
	}

	// Test vetrex 1625
	rayOrigin = pVertices[1625].p;

	// Transform vertex's position to world space
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&UAVMeshWorld);

	// Transform vertex's postion into model (local) space of the object we are checking against
	// This time we do not need to normalize as rayDirection is the same
	D3DXVec3TransformCoord(&rayOrigin,&rayOrigin,&matInverse);

	//We can now call the intersect function on our untransformed graphic mesh data:
	if (objectsViewMesh->isPointIntersectingWithMesh(rayOrigin,rayDirection))
	{
		hasHit = true;
	}

	pMesh->UnlockVertexBuffer();

	return hasHit;
}

void CD3DObject::drawMeshBounding()
{	
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX SphereTranslation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	matWorld = Scaling * RotationMatrix * Translation;

	// We need this to transform points in our mesh to world space
	D3DXVECTOR3 localSpherePosition = pMeshObjects[(int)pMeshObjects.size()-1]->getBoundingLocalPosition();

	// Transform our local coordinate to the world matrix of the object
	D3DXVec3TransformCoord(&localSpherePosition,&localSpherePosition,&matWorld);
	D3DXMatrixTranslation(&SphereTranslation,localSpherePosition.x,localSpherePosition.y,localSpherePosition.z);
	
	
	pDevice->SetTransform(D3DTS_WORLD, &SphereTranslation);
	pMeshObjects[(int)pMeshObjects.size()-1]->drawCD3DBoundingSphere();
}


// Based on http://www.c-unit.com/tutorials/mdirectx/?t=45 shoud be in the camrea code 
void CD3DObject::buildViewFrustum(D3DXMATRIX matView)
{

	// just to keep compiler happy
	D3DXMATRIX matProj;
	// Get combined matrix
	D3DXMATRIXA16 matComb;
	//D3DXMATRIXA16 matComb;
	D3DXMatrixMultiply(&matComb, &matView, &matProj);

	// Left clipping plane
	m_frustumPlanes[0].m_normal.x = matComb._14 + matComb._11; 
	m_frustumPlanes[0].m_normal.y = matComb._24 + matComb._21; 
	m_frustumPlanes[0].m_normal.z = matComb._34 + matComb._31; 
	m_frustumPlanes[0].m_distance = matComb._44 + matComb._41;

	// Right clipping plane 
	m_frustumPlanes[1].m_normal.x = matComb._14 - matComb._11; 
	m_frustumPlanes[1].m_normal.y = matComb._24 - matComb._21; 
	m_frustumPlanes[1].m_normal.z = matComb._34 - matComb._31; 
	m_frustumPlanes[1].m_distance = matComb._44 - matComb._41;

	// Top clipping plane 
	m_frustumPlanes[2].m_normal.x = matComb._14 - matComb._12; 
	m_frustumPlanes[2].m_normal.y = matComb._24 - matComb._22; 
	m_frustumPlanes[2].m_normal.z = matComb._34 - matComb._32; 
	m_frustumPlanes[2].m_distance = matComb._44 - matComb._42;

	// Bottom clipping plane 
	m_frustumPlanes[3].m_normal.x = matComb._14 + matComb._12; 
	m_frustumPlanes[3].m_normal.y = matComb._24 + matComb._22; 
	m_frustumPlanes[3].m_normal.z = matComb._34 + matComb._32; 
	m_frustumPlanes[3].m_distance = matComb._44 + matComb._42;

	// Near clipping plane 
	m_frustumPlanes[4].m_normal.x = matComb._13; 
	m_frustumPlanes[4].m_normal.y = matComb._23; 
	m_frustumPlanes[4].m_normal.z = matComb._33; 
	m_frustumPlanes[4].m_distance = matComb._43;

	// Far clipping plane 
	m_frustumPlanes[5].m_normal.x = matComb._14 - matComb._13; 
	m_frustumPlanes[5].m_normal.y = matComb._24 - matComb._23; 
	m_frustumPlanes[5].m_normal.z = matComb._34 - matComb._33; 
	m_frustumPlanes[5].m_distance = matComb._44 - matComb._43; 
}

bool CD3DObject::SphereInFrustum(CD3DObject *object)
{
	CD3DViewObject *tempMesh;
	object->getMesh(&tempMesh,0);
	float radius = tempMesh->getBoundingRadius();

	// We have to allow for the spheres center position a relation of the pos of the model

	// Get objects world matrix (where it is in the 3d world)
	D3DXMATRIX matWorld = object->getMatrix();

	// Get the local (mesh) coordinates of bounding sphere
	D3DXVECTOR3 localSpherePosition = tempMesh->getBoundingLocalPosition();

	// Transform our local coordinates to the world matrix of the object
	D3DXVec3TransformCoord(&localSpherePosition,&localSpherePosition,&matWorld);

	//D3DXVECTOR4 position4 = D3DXVECTOR4(object->getPosX(), object->getPosY(), object->getPosZ(), 1.0f );
	D3DXVECTOR4 position4 = D3DXVECTOR4(localSpherePosition.x,localSpherePosition.y,localSpherePosition.z, 1.0f );

	for ( int i = 0; i < 6; i++ ) 
	{ 
		D3DXVECTOR4 frustumPlanes = D3DXVECTOR4(m_frustumPlanes[i].m_normal.x, 
											m_frustumPlanes[i].m_normal.y, 
											m_frustumPlanes[i].m_normal.z, 
											m_frustumPlanes[i].m_distance);

		// *1.5 on the radius ensures objects do not just dissapear from the edge of the screen
		// change it back to just "radius" to see object culling in effect
		if ( D3DXVec4Dot(&frustumPlanes,&position4 ) + radius < 0 ) 
		{ 
			// Outside the frustum, reject it! 
			return false; 
		} 
	} 
	return true; 
}

// Gets the Euclidean distance to an objects position
float CD3DObject::relativeDistanceToObject(D3DXVECTOR3 position)
{
	float Dx,Dy,Dz,distance;

	//euclidean distance
	Dx = pos.x - position.x;
	Dy = pos.y - position.y;
	Dz = pos.z - position.z;

	distance = sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
	return distance;
}

// Gets the Euclidean distance to an object
float CD3DObject::relativeDistanceToObject(CD3DObject *object)
{
	float Dx,Dy,Dz,distance;

	//euclidean distance
	Dx = pos.x - object->getPosX();
	Dy = pos.y - object->getPosY();
	Dz = pos.z - object->getPosZ();

	distance = sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
	return distance;
}