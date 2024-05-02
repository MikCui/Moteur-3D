
// Classe pour la gestion de l'environnement mapping

#pragma once
#include "VariableGlobale.h"
#include "Scene.h"

// Résolution des textures de réflection
const int  CUBEMAP_RESOLUTION = 256;
const int  SPHEREMAP_RESOLUTION = 512;

class CScene;
class CCamera;

class CEnvMapping
{
public:
	CScene* m_pScene;					// pointeur sur la scène de la camera
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// pointeur sur l'objet Direct3DDevice
	LPDIRECT3DCUBETEXTURE9 m_pCubeMap;	// pointeur sur la texture de rendu pour le cube mapping
	LPDIRECT3DSURFACE9 m_DepthCube;		// pointeur sur le z-buffer pour le rendu du cube mapping
	LPDIRECT3DTEXTURE9 m_pSphereMap;	// pointeur sur la texture de rendu pour le sphère mapping
	LPDIRECT3DSURFACE9 m_DepthSphere;	// pointeur sur le z-buffer pour le rendu du sphère mapping
	LPD3DXMESH m_pMesh;					// pointeur sur une sphère pour l erendu du sphère mapping
	D3DXMATRIXA16 m_MatriceIdentite;	// matrice indentité
	D3DXVECTOR3 m_Position;				// centre du rendu de l'environnement dans la scène 

	CCamera* m_pCameraCubeMapping;		// pointeur sur la caméra pour le rendu du cube mapping
	CCamera* m_pCameraSphereMapping;	// pointeur sur la caméra pour le rendu du sphère mapping

	CEnvMapping(CScene* pScene);
	~CEnvMapping();

	bool Creation();
	void Release();
	void RenduCubeMapping();
	void RenduSphereMapping();
	LPDIRECT3DCUBETEXTURE9 GetCubeMap(){return m_pCubeMap;}
	LPDIRECT3DTEXTURE9 GetSphereMap(){return m_pSphereMap;}
	void SetPostion(D3DXVECTOR3* Position);
};