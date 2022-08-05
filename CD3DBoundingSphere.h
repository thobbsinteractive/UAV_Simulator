#ifndef INC_CD3DBoundingSphere_H
#define INC_CD3DBoundingSphere_H

#include <d3dx9.h>

//
// Constants
//

class CD3DBoundingSphere
{
public:
	CD3DBoundingSphere();
	~CD3DBoundingSphere();
	bool isPointInside(D3DXVECTOR3& p);
	void setWorldPosition(D3DXVECTOR3 newPos);
	
	float radius;
	D3DXVECTOR3 worldPos;
	D3DXVECTOR3 localPos;
};
#endif // INC_CD3DBoundingSphere_H