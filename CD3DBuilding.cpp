#include "CD3DBuilding.h"

CD3DBuilding::CD3DBuilding(D3DXVECTOR3 position,CD3DViewObject* _pMesh,IDirect3DDevice9* _pDevice)
{
	pos = position;
	rotation = D3DXVECTOR3(0.0f,0.0f,0.0f);

	pDevice = _pDevice;
	
	pMeshObjects.push_back(_pMesh);

	WHITE = ( D3DCOLOR_XRGB(255, 255, 255) );
	BLACK = ( D3DCOLOR_XRGB(  0,   0,   0) );
	RED = ( D3DCOLOR_XRGB(255,   0,   0) );
	GREEN = ( D3DCOLOR_XRGB(  0, 255,   0) );
	BLUE = ( D3DCOLOR_XRGB( 200,  200, 255) );
	YELLOW = ( D3DCOLOR_XRGB(255, 255,   0) );
	CYAN = ( D3DCOLOR_XRGB(  0, 255, 255) );
	MAGENTA = ( D3DCOLOR_XRGB(255,   0, 255) );

	WHITE_MTRL  = initMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	RED_MTRL    = initMtrl(RED, RED, RED, BLACK, 2.0f);

	CURRENT_BUILDING_MTRL = WHITE_MTRL;

	height =  _pMesh->getBoundingRadius()*2; // Radius of sphere is the height (unless building is wider than taller)
}


CD3DBuilding::~CD3DBuilding()
{

}

void CD3DBuilding::highLight()
{
	CURRENT_BUILDING_MTRL = RED_MTRL;
}

D3DMATERIAL9 CD3DBuilding::initMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

void CD3DBuilding::drawMesh()
{	
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);
	
	matWorld = Scaling * RotationMatrix * Translation;

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// If we use the usual way of drawing the mesh (the wrapped way) we cannot
	// set its materials, so we get a pointer and draw it our own way;
	ID3DXMesh* pMesh = pMeshObjects[0]->getMesh();

	// Draw all parts of the mesh with our custom Material set
	pDevice->SetMaterial(&CURRENT_BUILDING_MTRL);
	pMesh->DrawSubset(0);

	// Ensure material is set back to white.
	CURRENT_BUILDING_MTRL = WHITE_MTRL;
}

void CD3DBuilding::getType(char* pType)
{
	strcpy(pType,"CD3DBuilding");
}

float CD3DBuilding::getHeight()
{
	return height;
}