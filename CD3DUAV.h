#ifndef INC_CD3DUAV_H
#define INC_CD3DUAV_H

#include "CD3DObject.h"
#include "CD3DBuilding.h"
#include "SUavAttributes.h"
#include "SSimResults.h"

class CD3DUAV : public CD3DObject
{
public:
	CD3DUAV(UavAttributes attributes, CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice);
	~CD3DUAV();
	void buildRadarFrustum();
	bool sphereInRadar(CD3DObject *object);
	void walk(float units, float timeDelta); // forward/backward
	void pitch(float angle, float timeDelta); // rotate on left vector
	void yaw(float angle, float timeDelta); // rotate on up vector
	void roll(float angle ,float timeDelta); // rotate on look vector
	void behavior(std::vector<CD3DObject*> m_D3DObjectsArray,float timeDelta);
	void getDebug(char* pType);

	void virtual getType(char* ptype); // For type checking this class polymorphically

	bool simulationComplete(); // Tells us if this UAV has crashed or not
	SimResults getSimResults(); // get the metrics of the UAV peformance

protected:
	void lean(float timeDelta);

	void getAngleToRotate(float* x,float* y,D3DXVECTOR3 targetPos);

	void turnTowardsObjective(D3DXVECTOR3 posToHeadFor,float timeDelta);
	void avoid(D3DXVECTOR3 posToAvoid,float timeDelta);
	void flight(float timeDelta);

	frustumPlane m_radarfrustumPlanes[6]; // frustum from camera, help to cull objects
	float learnAngle; // decides the roll when the UAV banks

	void drawMesh(); // Yes we have to specialise the draw method (due to the fact we also draw the objective

private:
	UavAttributes m_sAttributes;
	SimResults m_sSimResults;

	// Addtional None adjustable attributes (physical, see fix these because for the 
	// Test to be fair all UAV's basic physical properties are the same
	float m_fWeightKG; // Weight, used to caculate min air speed
	float m_fMinSpeed; // Min air speed for UAV, it will start to drop in proportion to
		           // this if the speed is under
    float m_fMaxSpeed; // Effected by weight will be needed to climb
	float m_fRateOfTurn;
	float m_fAltitude;
	float m_fCurrentFuel;
	float m_fSetSpeed; // Speed the UAV is to run at
	float m_fActualSpeed; // Current speed of UAV.
	float m_fAccelerationRate;
	float m_fDecelerationRate;

	D3DXVECTOR3 m_fObjectivePos; // The position of the UAVs objective 

	float tarX;
	float tarY;
};

#endif //INC_CD3DUAV_H