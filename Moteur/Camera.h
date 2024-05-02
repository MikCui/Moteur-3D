
// Classe pour la gestion d'une caméra

#pragma once
#include "VariableGlobale.h"
#include "Scene.h"

class CScene;

class CCamera
{
public:
	CScene* m_pScene;						// pointeur sur la scène de la camera
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// pointeur sur l'objet Direct3DDevice

	D3DXMATRIXA16  m_MatriceTransProj;		// matrice de projection
	D3DXMATRIXA16  m_MatriceTransVision;	// matrice de vision

	D3DXVECTOR3 m_VecPosition;				// position de la caméra
	D3DXVECTOR3 m_VecCible;					// cible de la caméra
	D3DXVECTOR3 m_VecOrientation;			// orientation de la caméra

	float m_AngleX;							// angle en X de la caméra
	float m_AngleY;							// angle en Y de la caméra
	float m_Ratio;							// ratio largeur/hauteur

	D3DXPLANE m_PlanProjection[4];			// plans de la pyramide de projection

	CCamera();
	CCamera(CScene* pScene);
	~CCamera();
	void Creation();
	void Creation(D3DXVECTOR3* VecPosition, D3DXVECTOR3* VecCible, D3DXVECTOR3* VecOrientation = &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	void Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris, float DeltaTemps);
	void SetPosition(D3DXVECTOR3* VecPosition){m_VecPosition = *VecPosition;}
	void SetCible(D3DXVECTOR3* VecCible){m_VecCible = *VecCible;}
	void SetOrientation(D3DXVECTOR3* VecOrientation){m_VecOrientation = *VecOrientation;}
	D3DXVECTOR3 GetPosition(){return m_VecPosition;}
	D3DXVECTOR3 GetCible(){return m_VecCible;}
	D3DXVECTOR3 GetOrientation(){return m_VecOrientation;}
	D3DXMATRIXA16 GetMatriceVision(){return m_MatriceTransVision;}
	D3DXMATRIXA16 GetMatriceProjection(){return m_MatriceTransProj;}
	D3DXMATRIXA16 GetMatriceProjectionTexture();
	D3DXMATRIXA16 GetMatriceTextureCube();
	void SetMatrice();
	void SetMatriceCubeMap(DWORD nFace);
	void SetMatriceSphereMap();
	void SetMatriceMiroir(D3DXMATRIXA16* MatriceReflection);
	void SetPlanProjection();
	bool IsVisible(D3DXVECTOR3* Position, float Rayon);
};
