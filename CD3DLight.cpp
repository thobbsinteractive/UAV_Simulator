/* Name: light.cpp
   Description: The methods for the light class
*/
#include "CD3DLight.h"
int CD3DLight::numberOfLightsInScene = 0;

CD3DLight::CD3DLight(IDirect3DDevice9* pDevice)
{
	m_pDevice = pDevice;
}

CD3DLight::~CD3DLight()
{
	numberOfLightsInScene--;
}

void CD3DLight::InitDirectionalLight(D3DXVECTOR3 direction, D3DXCOLOR color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = color * 0.4f;
	light.Diffuse   = color;
	light.Specular  = color * 0.6f;
	light.Direction = direction;

	// Add the light to the scene and enable it
	m_pDevice->SetLight(numberOfLightsInScene,&light); 
	m_pDevice->LightEnable(numberOfLightsInScene,true);
	// Track the number of lights in the scene
	numberOfLightsInScene++;
}

void CD3DLight::InitPointLight(D3DXVECTOR3 position, D3DXCOLOR color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_POINT;
	light.Ambient   = color * 0.6f;
	light.Diffuse   = color;
	light.Specular  = color * 0.6f;
	light.Position  = position;
	light.Range        = 100000000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

		// Add the light to the scene and enable it
	m_pDevice->SetLight(numberOfLightsInScene,&light); 
	m_pDevice->LightEnable(numberOfLightsInScene,true);
	// Track the number of lights in the scene
	numberOfLightsInScene++;
}

void CD3DLight::InitSpotLight(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXCOLOR color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_SPOT;
	light.Ambient   = color * 0.4f;
	light.Diffuse   = color;
	light.Specular  = color * 0.6f;
	light.Position  = position;
	light.Direction = direction;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta        = 0.5f;
	light.Phi          = 0.7f;

	// Add the light to the scene and enable it
	m_pDevice->SetLight(numberOfLightsInScene,&light); 
	m_pDevice->LightEnable(numberOfLightsInScene,true);
	// Track the number of lights in the scene
	numberOfLightsInScene++;
}