#include "Ship.h"

Ship::Ship()
{
	health = 100.0f;
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	speed = 0.0f;
	speedMax = 250.0f;
	currentSpeedSetting = 0.0f;
	fireRate = 0.25f;
	timeTillFire = 0.0f;
	beingTargeted = false;

	angleOfCurrentObjective = 0.0f;

	gunPosition1 = D3DXVECTOR3(-90.0f, -20.0f, -70.0f);
	gunPosition2 = D3DXVECTOR3(90.0f, -20.0f, -70.0f);

	comments="";
	jumping = false;
}

Ship::Ship(float _health,D3DXVECTOR3 _pos, 
				D3DXVECTOR3 _rotation, 
				D3DXVECTOR3 _look)
{
	pos = _pos;
	health = _health;
	rotation = _rotation;
	look = _look;

	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	speed = 0.0f;
	speedMax = 250.0f;
	currentSpeedSetting = 0.0f;
	accelerationRate = 10.0f; // The Lower the faster
	fireRate = 0.25f;
	timeTillFire = 0.0f;
	beingTargeted = false;

	angleOfCurrentObjective = 0.0f;

	gunPosition1 = D3DXVECTOR3(-90.0f, -20.0f, -70.0f);
	gunPosition2 = D3DXVECTOR3(90.0f, -20.0f, -70.0f);

	comments="";

	jumping = false;
};

Ship::~Ship()
{
	for(int i =0;i < (int)fireBalls.size();i++)
	{
		//erase all sounds
		fireBalls.erase(fireBalls.begin() + i);
	}
};

void Ship::setComment(std::string newComments)
{
	comments = newComments;
}

std::string Ship::getComments()
{
	return comments;
}

void Ship::setJumping(bool value)
{
	jumping = value;
}

bool Ship::isJumping()
{
	return jumping;
}

float Ship::getMaxSpeed()
{
	return speedMax;
}

void Ship::setMissionCritical(bool value)
{
	missionCritical=value;
}

bool Ship::getMissionCritical()
{
	return missionCritical;
}

void Ship::getTargetMatrix(D3DXVECTOR3 at,float size,D3DXMATRIX* V)
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,size,size,size);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, at.x,at.y,at.z);
	
	*V = RotationMatrix * Scaling * Translation;
}

void Ship::getRadarMatrix(D3DXVECTOR3 at,float size,D3DXMATRIX* V)
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX LocalTranslation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,1.0,1.0,1.0);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);
	D3DXMatrixTranslation(&LocalTranslation, at.x,at.y,at.z);
	Translation = Translation * LocalTranslation;

	*V = RotationMatrix * Scaling * Translation;
}

void Ship::setSpeed(float _speed)
{
	speed = _speed;
}

float Ship::distanceToTarget()
{
	float Dx,Dy,Dz,distance;

	//euclidean distance
	Dx = pos.x - targetPos.x;
	Dy = pos.y - targetPos.y;
	Dz = pos.z - targetPos.z;

	distance = sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
	return distance;
}

float Ship::distanceToTarget(D3DXVECTOR3 position)
{
	float Dx,Dy,Dz,distance;

	//euclidean distance
	Dx = pos.x - position.x;
	Dy = pos.y - position.y;
	Dz = pos.z - position.z;

	distance = sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
	return distance;
}

void Ship::getAngleToRotate(float* x,float* y)
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

void Ship::getAngleToAvoid(float* x,float* y,D3DXVECTOR3 avoidTargetPos)
{
	float Dx,Dy,Dz;
	float bearingX;
	float bearingY;


	// Work out how much to yaw
	Dz = pos.z - avoidTargetPos.z;
	Dx = pos.x - avoidTargetPos.x;


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
	Dz = pos.z - avoidTargetPos.z;
	Dx = pos.y - avoidTargetPos.y;

	
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

float Ship::getAngleOfCurrentObjective()
{
	return angleOfCurrentObjective;
}

void Ship::setHealth(float _health)
{
	health = _health;
};

void Ship::decreaseHealth(float amountToDecrease)
{
	health = health - amountToDecrease;	
};

void Ship::shoot()
{
	D3DXVECTOR3 firePosition;

	if (timeTillFire <= 0.0f)
	{
		if (fireBalls.size() < 200)
		{
			updatePositionInRelationTo(gunPosition1,rotation,&firePosition);
			fire temp(30.0f,&firePosition,&rotation,&look);
			fireBalls.push_back(temp);

			updatePositionInRelationTo(gunPosition2,rotation,&firePosition);
			fire temp2(30.0f,&firePosition,&rotation,&look);
			fireBalls.push_back(temp2);
			
			sound[1].stop();
			sound[1].reset();
			playSound(1,D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
		timeTillFire = fireRate;
	}
}

void Ship::updateFire(float timeDelta)
{

	for(int i = 0; i < (int)fireBalls.size(); i++)
	{
		fireBalls[i].walk(timeDelta);
		fireBalls[i].updateHealth(timeDelta);

		if (fireBalls[i].getHealth() <= 0.0f)
		{
			//Delete fire ball if its dead
			fireBalls.erase(fireBalls.begin() + i);
		}
	}
	timeTillFire = timeTillFire - timeDelta;
}

void Ship::setFireHealth(int index, float _health)
{
	fireBalls[index].setHealth(_health);
}

void Ship::getFirePosition(int index, D3DXVECTOR3* firePos)
{
	fireBalls[index].getPosition(&*firePos);
}

void Ship::getFireObject(int index, objectTransform* fireball)
{
	*fireball = (objectTransform)fireBalls[index];
}

void Ship::update(float timeDelta)
{
	// Maintain objects speed
	walk(speed * timeDelta,false);
	updateFire(timeDelta);
}

float Ship::getHealth()
{
	return health;
}

void Ship::getFireMatrix(int i,D3DXMATRIX* V)
{
	fireBalls[i].getMatrix(&*V,rotation);
};

int Ship::getNumberofFireBalls()
{
	return (int)fireBalls.size();
}

float Ship::getCurrentSpeedSetting()
{
	return currentSpeedSetting;
}
float Ship::getCurrentSpeed()
{
	return speed;
}

bool Ship::getTargeted()
{
	return beingTargeted;
}

void Ship::setTargeted()
{
	if(beingTargeted == false)
	{
		beingTargeted = true;
	}else
	{
		beingTargeted = false;
	}

}

void Ship::walk(float units,bool keypress)
{
    // Addition of speed controls means overloading the inherted function.
	D3DXVECTOR3 newLook;
	D3DXVECTOR3 posTemp;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;
	
	// If a key is press increase or reduce the currentSpeedSetting setting accordingly
	if (keypress == true)
	{
		if((units > 0.0f) && (currentSpeedSetting < speedMax))
		{
			// Ensure that the setting does not go above speedMax
			if(currentSpeedSetting + units < speedMax)
			{
				currentSpeedSetting = currentSpeedSetting + units/2;
			}else
			{
				currentSpeedSetting = speedMax;
			}
		}
		
		if((units <= 0.0f) && (currentSpeedSetting > 0.0f))
		{
			// Ensure that the setting does not go below 0.0f
			if(currentSpeedSetting + units > 0.0f)
			{
				currentSpeedSetting = currentSpeedSetting + units/2;
			}else
			{
				currentSpeedSetting = 0.0f;
			}
		}
	}
	

	if(speed != currentSpeedSetting)
	{
		// Speed up to current speed setting if a positive value is used
		if(speed < currentSpeedSetting)
		{
			// Ensure that the setting does not go above currentSpeedSetting
			if(units > 0.0f)
			{
				if(speed + units/2 < currentSpeedSetting)
				{
					speed = speed + units/2;
				}else
				{
					speed = currentSpeedSetting;
				}
			}else
			{
				if(speed - units/2 < currentSpeedSetting)
				{
					speed = speed - units/2;
				}else
				{
					speed = currentSpeedSetting;
				}
			}
		}
		
		// Slow down to current speed setting if a negative value is used
		if(speed > currentSpeedSetting)
		{
			// Ensure that the setting does not go below 0.0f
			if(units < 0.0f)
			{
				if(speed + units/2 > 0.0f)
				{
					speed = speed + units/2;
				}else
				{
					speed = 0.0f;
				}
			}else
			{
				if(speed - units/2 > 0.0f)
				{
					speed = speed - units/2;
				}else
				{
					speed = 0.0f;
				}
			}
		}
	}


	// The "look" vector isn't even updated, if I assign the value of "normedVector" to "look"
	// The objects trajectory is messed up? normedVector seems to hold the amount of change to
	// make, not the actual vector.

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&currentVector, &newLook );

	posTemp.x = pos.x;
	posTemp.y = pos.y;
	posTemp.z = pos.z;

	posTemp += currentVector * speed;

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

	if(chaseCamera == true)
	{
		updateCamera();
	}

	whereShipIsGoingToBe = pos;
	whereShipIsGoingToBe += currentVector * speed;

	// update the sound position every time the object moves
	for(int i = 0; i < sound.size(); i++)
	{
		sound[i].setSoundPos(pos,speed);
	}
}

void Ship::getTargetPosition(D3DXVECTOR3* _targetPos)
{
	*_targetPos = targetPos;
}

D3DXVECTOR3 Ship::getWhereShipIsGoingToBe()
{
	return whereShipIsGoingToBe;
}

// Includes speed

void Ship::playSound(int soundNumber,D3DXVECTOR3 listenerPosition)
{
	if((int)sound.size() > 0)
	{
		sound[soundNumber].setSoundPos(pos,0.0f);
		sound[soundNumber].playSound3D();
	}
}