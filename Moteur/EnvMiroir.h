
// Classe pour la gestion de la rélection

#pragma once
#include "VariableGlobale.h"
#include "Scene.h"

class CScene;

class CEnvMiroir
{
public:
	CScene* m_pScene;					// pointeur sur la scène de la camera
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// pointeur sur l'objet Direct3DDevice
	LPDIRECT3DTEXTURE9 m_pMiroir;		// pointeur sur la texture de rendu du miroir
	LPDIRECT3DSURFACE9 m_DepthMiroir;	// pointeur sur le z-buffer pour le rendu du miroir
	D3DXMATRIXA16 m_MatriceReflection;	// matrice de réflection par rapport au miroir

	CEnvMiroir(CScene* pScene);
	~CEnvMiroir();

	bool Creation(D3DXPLANE* Plan);
	void Release();
	void RenduMiroir();
	LPDIRECT3DTEXTURE9 GetMiroir(){return m_pMiroir;}
};