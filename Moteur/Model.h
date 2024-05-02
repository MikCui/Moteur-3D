
// Classe pour la gestion d'un model 3D

#pragma once
#include "VariableGlobale.h"
#include "Scene.h"

class CScene;

class CModel
{
public:
	CScene* m_pScene;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	LPD3DXMESH              m_pMesh;			// Pointeur sur le Mesh
	D3DMATERIAL9*           m_pMeshMaterielles;	// Pointeur sur les matérielles
	LPDIRECT3DTEXTURE9*     m_pMeshTextures;	// Pointeur sur les textures
	DWORD                   m_NumMaterielles;	// Nombre de matérielle

	CModel();
	CModel(CScene* pScene);
	~CModel();

	virtual bool ChargerModel(char* NomFichier);
	virtual void Release();
	virtual void RenduModel();
};
