#ifndef INC_CD3DBUILDING_H
#define INC_CD3DBUILDING_H

#include "CD3DObject.h"

class CD3DBuilding : public CD3DObject
{
public:
	CD3DBuilding(D3DXVECTOR3 position, CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice);
	~CD3DBuilding();
	void highLight(); // Highlights building in red for 1 frame (TO show that the UAV has detected it
	void drawMesh();
	float getHeight();
	void virtual getType(char* ptype); // For type checking this class polymorphically

private:
	float height;
	D3DMATERIAL9 initMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	// Colors
	D3DXCOLOR      WHITE;
	D3DXCOLOR      BLACK;
	D3DXCOLOR        RED;
	D3DXCOLOR      GREEN;
	D3DXCOLOR       BLUE;
	D3DXCOLOR     YELLOW;
	D3DXCOLOR       CYAN;
	D3DXCOLOR    MAGENTA;

	//
	// Materials
	//
	D3DMATERIAL9 WHITE_MTRL;
	D3DMATERIAL9 RED_MTRL;
	D3DMATERIAL9 CURRENT_BUILDING_MTRL;
};

#endif //INC_CD3DBUILDING_H