#include "CD3DBoundingSphere.h"

CD3DBoundingSphere::CD3DBoundingSphere()
{
	// infinite small 

	D3DXVECTOR3 worldPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	D3DXVECTOR3 localPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	radius = 0.0f;
}

CD3DBoundingSphere::~CD3DBoundingSphere()
{

}

void CD3DBoundingSphere::setWorldPosition(D3DXVECTOR3 newCentre)
{
	worldPos = newCentre;
}

bool CD3DBoundingSphere::isPointInside(D3DXVECTOR3& p)
{
	// modified by me to allow for movement through the snow
	if( (p.x >= (worldPos.x - radius)) && (p.y >= (worldPos.y - radius)) && (p.z >= (worldPos.z - radius))&&
		(p.x <= (worldPos.x + radius)) && (p.y <= (worldPos.y + radius)) && (p.z <= (worldPos.z + radius)))
	{
		return true;
	}
	else
	{
		return false;
	}
}