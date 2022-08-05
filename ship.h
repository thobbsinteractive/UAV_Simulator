//////////////////////////////////////////////////////////////////////////////////////////////////
// ship.h
// Author: Tim Hobbs
// Description: Models the ships position and gunfire
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_SHIP_H
#define INC_SHIP_H

#include "objectTransform.h"
#include "fire.h"
#include <math.h>

class Ship : public objectTransform
{
public:
	Ship();
	Ship(float _health,D3DXVECTOR3 _pos, 
			D3DXVECTOR3 _rotation, 
			D3DXVECTOR3 _look);
	~Ship();

	void setHealth(float _health);
	float getHealth();
	void decreaseHealth(float amountToDecrease);
	void getFireMatrix(int i,D3DXMATRIX* V);
	void getFirePosition(int index,D3DXVECTOR3* firePos);
	void setFireHealth(int index, float _health);
	void getFireObject(int index, objectTransform* fireball);
	int getNumberofFireBalls();
	void getScale(D3DXVECTOR3* _scale);
	void walk(float units,bool keypress);
	float getCurrentSpeedSetting();
	float getCurrentSpeed(); 
	float getMaxSpeed();
	void shoot();
	void update(float timeDelta);
	void setSpeed(float _speed);
	void setTargeted();
	bool getTargeted();
	void getAngleToRotate(float* x,float* y);
	void getAngleToAvoid(float* x,float* y,D3DXVECTOR3 avoidTargetPos);
	float getAngleOfCurrentObjective();
	void setTargetPosition(D3DXVECTOR3 _targetpos);
	void getTargetPosition(D3DXVECTOR3* _targetPos);
	void getTargetMatrix(D3DXVECTOR3 at,float size,D3DXMATRIX* V);
	void getRadarMatrix(D3DXVECTOR3 at,float size,D3DXMATRIX* V);
	float distanceToTarget();
	float distanceToTarget(D3DXVECTOR3 position);
	D3DXVECTOR3 getWhereShipIsGoingToBe();

	void setJumping(bool value);
	bool isJumping();

	void setMissionCritical(bool value);
	bool getMissionCritical();
	void setComment(std::string newComments);
	void playSound(int soundNumber,D3DXVECTOR3 listenerPosition);
	std::string getComments();

protected:
	void updateFire(float timeDelta);

	float health;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR3 whereShipIsGoingToBe;
	D3DXVECTOR3 gunPosition1;
	D3DXVECTOR3 gunPosition2;

	std::vector<fire> fireBalls;
	bool beingTargeted;
	bool missionCritical;
	float speed;
	float currentSpeedSetting;
	float speedMax;
	float accelerationRate;
	float fireRate;
	float timeTillFire;
	float angleToRotateX;
	float angleToRotateY;
	float angleOfCurrentObjective;
	std::string comments;

	bool jumping;

};

#endif //INC_SHIP_H