#include "CD3DUAV.h"

CD3DUAV::CD3DUAV(UavAttributes attributes, CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice)
{
	m_sAttributes = attributes;
	
	m_fObjectivePos = D3DXVECTOR3(m_sAttributes.rescuePosX,m_sAttributes.rescueAltitude,m_sAttributes.rescuePosZ);

	// Initialise sim results
	m_sSimResults.distanceToTarget = relativeDistanceToObject(m_fObjectivePos);
	m_sSimResults.missionComplete = false;
	m_sSimResults.crashed = false;
	m_sSimResults.timeToReachTarget = 0.0f;


	// UAV starts at flightSpeed (as its a plane and we don't simulate takeoff)
	m_fSetSpeed = m_sAttributes.flightSpeed;
	m_fActualSpeed = m_sAttributes.flightSpeed;
	m_fWeightKG = 10.0 + (m_sAttributes.startFuel/100);
	m_fAccelerationRate = 1000.0f;
	m_fDecelerationRate = 2000.0f;
	m_fRateOfTurn = 1.0f;
	m_fCurrentFuel = m_sAttributes.startFuel;
	m_fMinSpeed = 10000.0f; // 100 kph
	m_fMaxSpeed = 35000.0f; // 350 Kph (175 mph)



	pos = D3DXVECTOR3(m_sAttributes.startPosX, m_sAttributes.startAltitude, m_sAttributes.startPosZ);
	rotation = D3DXVECTOR3(m_sAttributes.startRotX,m_sAttributes.startRotY,m_sAttributes.startRotZ);

	pDevice = _pDevice;

	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	pMeshObjects.push_back(_pMesh);

	buildRadarFrustum();
}

CD3DUAV::~CD3DUAV()
{

}

void CD3DUAV::getType(char* pType)
{
	strcpy(pType,"CD3DUAV");
}

void CD3DUAV::getDebug(char* pType)
{
	char m_cDebugString[350];

	float distanceOfObjective = relativeDistanceToObject(m_fObjectivePos);
	sprintf(m_cDebugString, "DistanceOfObjective: %8.2f\nSetSpeed: %8.2f\nActualSpeed: %8.2f\nCurrentFuel: %8.2f\nCurrentWeight: %8.0f\nAltitude: %8.2f \nHasReachedObjective: %d HasCrashed: %d",distanceOfObjective,m_fSetSpeed,m_fActualSpeed,m_fCurrentFuel,m_fWeightKG,pos.y,m_sSimResults.missionComplete,m_sSimResults.crashed);
	m_cDebugString[249] = '\0'; // mark end of string
	strcpy(pType,m_cDebugString);
}

void CD3DUAV::walk(float units, float timeDelta)
{
	CD3DObject::walk(units,timeDelta);
	buildRadarFrustum();
}

void CD3DUAV::pitch(float angle, float timeDelta)
{
	CD3DObject::pitch(angle,timeDelta);
	buildRadarFrustum();
}

void CD3DUAV::yaw(float angle, float timeDelta)
{
	CD3DObject::yaw(angle,timeDelta);
	buildRadarFrustum();
}

void CD3DUAV::lean(float timeDelta)
{

}

void CD3DUAV::roll(float angle ,float timeDelta)
{
	CD3DObject::roll(angle,timeDelta);
	buildRadarFrustum();
}

// Builds Decides the radar frustum
void CD3DUAV::buildRadarFrustum()
{
	D3DXVECTOR3 radarTarget = pos; // Need current position
	D3DXVECTOR3 look;
	D3DXVECTOR3 newLook;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;

	D3DXMATRIX m_Proj; // Projection matrix for radar
	D3DXMATRIX m_View;

	// Just like building the camera project matrix, we do the same here for the radar
	D3DXMatrixPerspectiveFovLH(
		&m_Proj,
		D3DX_PI * m_sAttributes.radarViewAngle,
		m_sAttributes.radarAspectWidth / m_sAttributes.radarAspectHeight,
		1.0f,
		m_sAttributes.radarRange);


	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Calculate radar look at coordinates 
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	radarTarget += normedVector * m_sAttributes.radarRange;

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// Stops the radar being flipped upside down when rotating up
	if(((rotation.y*(180/D3DX_PI)) > 90)&&((rotation.y*(180/D3DX_PI)) < 270))
	{
		up = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}else
	{
		up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// Computer Radar View Space Matrix
	D3DXMatrixLookAtLH(&m_View, &pos, &radarTarget, &up);

	// Get combined matrix
	D3DXMATRIXA16 matComb;

	//D3DXMATRIXA16 matComb;
	D3DXMatrixMultiply(&matComb, &m_View, &m_Proj);

	// Left clipping plane
	m_radarfrustumPlanes[0].m_normal.x = matComb._14 + matComb._11; 
	m_radarfrustumPlanes[0].m_normal.y = matComb._24 + matComb._21; 
	m_radarfrustumPlanes[0].m_normal.z = matComb._34 + matComb._31; 
	m_radarfrustumPlanes[0].m_distance = matComb._44 + matComb._41;

	// Right clipping plane 
	m_radarfrustumPlanes[1].m_normal.x = matComb._14 - matComb._11; 
	m_radarfrustumPlanes[1].m_normal.y = matComb._24 - matComb._21; 
	m_radarfrustumPlanes[1].m_normal.z = matComb._34 - matComb._31; 
	m_radarfrustumPlanes[1].m_distance = matComb._44 - matComb._41;

	// Top clipping plane 
	m_radarfrustumPlanes[2].m_normal.x = matComb._14 - matComb._12; 
	m_radarfrustumPlanes[2].m_normal.y = matComb._24 - matComb._22; 
	m_radarfrustumPlanes[2].m_normal.z = matComb._34 - matComb._32; 
	m_radarfrustumPlanes[2].m_distance = matComb._44 - matComb._42;

	// Bottom clipping plane 
	m_radarfrustumPlanes[3].m_normal.x = matComb._14 + matComb._12; 
	m_radarfrustumPlanes[3].m_normal.y = matComb._24 + matComb._22; 
	m_radarfrustumPlanes[3].m_normal.z = matComb._34 + matComb._32; 
	m_radarfrustumPlanes[3].m_distance = matComb._44 + matComb._42;

	// Near clipping plane 
	m_radarfrustumPlanes[4].m_normal.x = matComb._13; 
	m_radarfrustumPlanes[4].m_normal.y = matComb._23; 
	m_radarfrustumPlanes[4].m_normal.z = matComb._33; 
	m_radarfrustumPlanes[4].m_distance = matComb._43;

	// Far clipping plane 
	m_radarfrustumPlanes[5].m_normal.x = matComb._14 - matComb._13; 
	m_radarfrustumPlanes[5].m_normal.y = matComb._24 - matComb._23; 
	m_radarfrustumPlanes[5].m_normal.z = matComb._34 - matComb._33; 
	m_radarfrustumPlanes[5].m_distance = matComb._44 - matComb._43; 
}

// Requires the passed object to have a bounding sphere setup
bool CD3DUAV::sphereInRadar(CD3DObject *object)
{

	float objectAltitude = 0;
	// Because there is only one position coordinate in an object (at the bottom)
	// And we don't want to fly through the tops of some of the taller buildings
	// We will for the case of radar and collision avoidance use either the m_fAltitude
	// of the UAV or (if it it flying above the buildings) the buildings m_fAltitude.

	// Check type
	char type[250];
	object->getType(type);

	if(strcmp(type,"CD3DBuilding") == 0)
	{	
		// If UAV is lower than the building...
		if((static_cast<CD3DBuilding *>(object)->getHeight()) > pos.y)
		{
			// best coordinates to use is the UAV's m_fAltitude
			objectAltitude = pos.y;
			
		}else
		{
			// Use the buildings height (so we know we are above it)
			objectAltitude = static_cast<CD3DBuilding *>(object)->getHeight();
		}
	}

	D3DXVECTOR3 buildingPosition = D3DXVECTOR3(object->getPosX(),objectAltitude, object->getPosZ());

	D3DXVECTOR4 position4 = D3DXVECTOR4(buildingPosition.x,buildingPosition.y,buildingPosition.z, 1.0f );

	CD3DViewObject *tempMesh;
	object->getMesh(&tempMesh,0);
	float radius = tempMesh->getBoundingRadius();

	// Get objects distance from our UAV (using the new building coordinates
	float distance = relativeDistanceToObject(buildingPosition);


	for ( int i = 0; i < 6; i++ ) 
	{ 
		D3DXVECTOR4 frustumPlanes = D3DXVECTOR4(m_radarfrustumPlanes[i].m_normal.x, 
											m_radarfrustumPlanes[i].m_normal.y, 
											m_radarfrustumPlanes[i].m_normal.z, 
											m_radarfrustumPlanes[i].m_distance);


		// *1.5 on the radius ensures objects do not just dissapear from the edge of the screen
		// change it back to just "radius" to see object culling in effect
		if (( D3DXVec4Dot(&frustumPlanes,&position4 ) + radius < 0 )||(distance > m_sAttributes.radarRange))
		{ 
			// Outside the frustum, reject it! 
			return false; 
		} 
	} 
	return true; 
}

bool CD3DUAV::simulationComplete()
{
	if((m_sSimResults.crashed == true)||(m_sSimResults.missionComplete == true))
	{
		return true;
	}else
	{
		return false;
	}
}

SimResults CD3DUAV::getSimResults()
{
	return m_sSimResults;
}

void CD3DUAV::behavior(std::vector<CD3DObject*> m_D3DObjectsArray,float timeDelta)
{

	// Check sim hasn't finished
	if((m_sSimResults.crashed == false)&&(m_sSimResults.missionComplete == false))
	{
		int indexOfClosestObject = -1;
		float distanceOfClosestObject = -1.0f;
		float distanceOfObject = 0.0f;

		// Need to check radar to see if we are going to hit anything
		for(int i = 0;i < (int)m_D3DObjectsArray.size();i++)
		{
			// Check type
			char type[250];
			m_D3DObjectsArray[i]->getType(type);

			// If NOT a UAV
			if(strcmp(type,"CD3DBuilding") == 0)
			{
				// Is it in the radar?
				if(sphereInRadar(m_D3DObjectsArray[i]))
				{
					// If it is and is a building paint it red
					if(strcmp(type,"CD3DBuilding") == 0)
					{	
						static_cast<CD3DBuilding *>(m_D3DObjectsArray[i])->highLight();
						
						// Get objects distance from UAV
						distanceOfObject = relativeDistanceToObject(m_D3DObjectsArray[i]);

						// check its distance from the last object
						if(distanceOfClosestObject == -1.0f)
						{
							distanceOfClosestObject = distanceOfObject;
							indexOfClosestObject = i;
						}
						else if(distanceOfObject < distanceOfClosestObject)
						{
							// If it is the closest get its index (so see can get positional 
							// data from it)
							distanceOfClosestObject = distanceOfObject;
							indexOfClosestObject = i;
						}
					}
				}

				// Check if the UAV has crashed in building (lowlevel collision detection required)
				if(pos.y <= 0.0f)
				{
					m_sSimResults.crashed = true;
				}
				
				// Are we in the buildings bounding sphere?
				if(highlevelCollisionDetection(m_D3DObjectsArray[i]))
				{
					// If yes check lowlevel collisions
					if(lowlevelMeshCollisionDetection(m_D3DObjectsArray[i]))
					{
						m_sSimResults.crashed = true;
					}
				}
			}
		}

		//indexOfClosestObject = -1;

		// If we have an object in range we need to take evasive action
		if(indexOfClosestObject != -1)
		{
			// Avoid
			D3DXVECTOR3 position = m_D3DObjectsArray[indexOfClosestObject]->getPosition();
			avoid(position,timeDelta);

		// There is nothing to avoid head towards our objective
		}else
		{
			turnTowardsObjective(m_fObjectivePos,timeDelta);
		}

		// Simulate general flight (speed weight fuel etc..)
		flight(timeDelta);

		// Increment the time the UAV has been flying
		m_sSimResults.timeToReachTarget += 1 * timeDelta;

		// Has the UAV completed its objective?
		if(m_sSimResults.distanceToTarget < 100000.0f)
		{
			m_sSimResults.missionComplete = true;
		}
	}
}

void CD3DUAV::flight(float timeDelta)
{
	// Adjust speed (if necessary)

	if(m_fCurrentFuel > 0.0f)
	{

		// UAV is traveling too fast for the thing to fly
		if(m_fActualSpeed > m_fMaxSpeed)
		{
			m_fActualSpeed = m_fMaxSpeed;
		}else
		{
			if(m_fSetSpeed > m_fActualSpeed)
			{
				m_fActualSpeed += m_fDecelerationRate* timeDelta;
			}
			else if(m_fSetSpeed <= m_fActualSpeed)
			{
				m_fActualSpeed -= m_fAccelerationRate* timeDelta;
			}
		}
		m_fCurrentFuel -= ((m_fWeightKG/100) + m_fActualSpeed/100) * timeDelta;

	}else
	{
		if(m_fActualSpeed >= 0.0f)
		{
			m_fActualSpeed -= ((m_fDecelerationRate/2)* timeDelta);
		}
		m_fCurrentFuel = 0.0f;
	}

	m_fSetSpeed = m_sAttributes.flightSpeed;

	// If the flight speed is too slow
	if(m_fActualSpeed < m_fMinSpeed)
	{
		pos.y = pos.y - (((m_fMinSpeed - m_fActualSpeed)*20)*timeDelta);
	}

	// Update weight per frame
	m_fWeightKG = 10.0 + m_fCurrentFuel/10000;

	// Weight effects speed
	walk(m_fActualSpeed - (m_fWeightKG*10),timeDelta);

	// Update the distance from our objective each frame
	m_sSimResults.distanceToTarget = relativeDistanceToObject(m_fObjectivePos);
}

void CD3DUAV::turnTowardsObjective(D3DXVECTOR3 posToHeadFor,float timeDelta)
{
	float x = 0.0f;
	float y = 0.0f;
	float rotX = rotation.x;
	float rotY = rotation.y;
	float yawAmount = 0.0f;
	float pitchAmount = 0.0f;
	float difference = 0.0f;

	getAngleToRotate(&x,&y,posToHeadFor);
	// Angles returned are in degrees 0 to 360 (no minus)

	float angleToRotateX;
	angleToRotateX = addAngle(rotX,-x);
	// Convert to degrees
	angleToRotateX = angleToRotateX * (180/D3DX_PI); 
	tarX = angleToRotateX;

	float angleToRotateY;
	angleToRotateY = addAngle(rotY,-y);
	// Convert to degrees
	angleToRotateY = angleToRotateY * (180/D3DX_PI); 
	tarY = angleToRotateY;

	// We now know how much to turn.

	if(angleToRotateX > 180)
	{
		yawAmount = m_fRateOfTurn;
	}else
	{
		yawAmount = -m_fRateOfTurn;
	}

	if(angleToRotateY > 180)
	{
		pitchAmount = m_fRateOfTurn;
	}else
	{
		pitchAmount = -m_fRateOfTurn;
	}

	m_fSetSpeed = m_sAttributes.flightSpeed;

	if(angleToRotateX > 1)
	{
		yaw(yawAmount,timeDelta);
		m_fSetSpeed = m_sAttributes.turnSpeed;
	}

	if(angleToRotateY > 1)
	{
		pitch(pitchAmount,timeDelta);
		m_fSetSpeed = m_sAttributes.turnSpeed;
	}
}


void CD3DUAV::avoid(D3DXVECTOR3 posToAvoid,float timeDelta)
{
	float x = 0.0f;
	float y = 0.0f;
	float rotX = rotation.x;
	float rotY = rotation.y;
	float yawAmount = 0.0f;
	float pitchAmount = 0.0f;
	float difference = 0.0f;

	getAngleToRotate(&x,&y,posToAvoid);
	// Angles returned are in degrees 0 to 360 (no minus)

	float angleToRotateX;
	angleToRotateX = addAngle(rotX,-x);
	// Convert to degrees
	angleToRotateX = angleToRotateX * (180/D3DX_PI); 

	float angleToRotateY;
	angleToRotateY = addAngle(rotY,-y);
	// Convert to degrees
	angleToRotateY = angleToRotateY * (180/D3DX_PI); 
	rotY = rotY * (180/D3DX_PI); 

	//tarX = angleToRotateX;
	//tarY = angleToRotateY;

	// We now know how much to turn.
	if(angleToRotateX > 180)
	{
		yawAmount = (-m_fRateOfTurn);
	}else
	{
		yawAmount = (m_fRateOfTurn);
	}

	// We now know how much to turn. WE DO NOT WANT TO DIVE TO AVOID!! 
	// We also do not want the UAV to loop the loop
	//if(rotY < 180)
	//{
	if(angleToRotateY > 180)
	{
		pitchAmount = -m_fRateOfTurn;
	}else
	{
		pitchAmount = m_fRateOfTurn;
	}
	//}


	if(angleToRotateY > 1)
	{
		pitch(pitchAmount,timeDelta);
		m_fSetSpeed = m_sAttributes.turnSpeed;
	}

	if(angleToRotateX > 1)
	{
		yaw(yawAmount,timeDelta);
		m_fSetSpeed = m_sAttributes.turnSpeed;
	}
}

void CD3DUAV::getAngleToRotate(float* x,float* y,D3DXVECTOR3 targetPos)
{
	float Dx,Dy,Dz;
	float bearingX;
	float bearingY;

	// Work out how much to yaw
	Dz = pos.z - targetPos.z;
	Dx = pos.x - targetPos.x;

	if ((Dz == 0) && (Dx >= 0))
	{
		bearingX = 0;
	}else if ((Dz == 0) && (Dx < 0))
	{
		bearingX = 180;
	}else if ((Dx == 0) && (Dz > 0))
	{
		bearingX = 90;	
	}else if ((Dx == 0) && (Dz < 0))
	{
		bearingX = 270;
	}else
	{
		bearingX = atan(abs(Dx / Dz)) * 57.296;  
	}
	
	if ((Dz > 0) && (Dx > 0))
	{
		bearingX = 180 + bearingX;
	}

	if ((Dz < 0) && (Dx > 0))
	{
		bearingX = 360 - bearingX;
	}

	if ((Dz > 0) && (Dx < 0)) 
	{
		bearingX = 180 - bearingX;
	}

	bearingX = bearingX * (D3DX_PI/180); // Convert to radians


	// Work out how much to pitch
	Dz = pos.z - targetPos.z;
	Dx = pos.y - targetPos.y;

	
	if ((Dz == 0) && (Dx >= 0))
	{
		bearingY = 0;
	}else if ((Dz == 0) && (Dx < 0))
	{
		bearingY = 180;
	}else if ((Dx == 0) && (Dz > 0))
	{
		bearingY = 90;	
	}else if ((Dx == 0) && (Dz < 0))
	{
		bearingY = 270;
	}else
	{
		bearingY = atan(abs(Dx / Dz)) * 57.296;  
	}

	if ((Dz < 0) && (Dx < 0))
	{
		bearingY = 360 - bearingY;
	}

	if ((Dz > 0) && (Dx < 0))
	{
		bearingY = 360 - bearingY;
	}

	if ((Dz > 0) && (Dx < 0)) 
	{
		//bearingY = 180 - bearingY;
	}

	bearingY = bearingY * (D3DX_PI/180); // Convert to radians

	*x = bearingX;
	*y = bearingY;
}

void CD3DUAV::drawMesh()
{	
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);
	
	matWorld = Scaling * RotationMatrix * Translation;

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Draw UAV
	pMeshObjects[0]->drawObject();

	// Draw Objective it is heading towards
	D3DXMatrixTranslation(&Translation,m_fObjectivePos.x,m_fObjectivePos.y,m_fObjectivePos.z);
	D3DXMatrixScaling(&Scaling,5.0f,5.0f,5.0f);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,0.0f,0.0f,0.0f);
	
	matWorld = Scaling * RotationMatrix * Translation;

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Draw Objective it is heading towards (with the same scale and rotation, but different position)
	pMeshObjects[1]->drawObject();
}