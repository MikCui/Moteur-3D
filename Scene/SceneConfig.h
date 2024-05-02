#pragma once
#include "..\Moteur\Scene.h"
#include "ModelRoche.h"
#include "ModelSol.h"
#include "ModelEau.h"
#include "ModelVegetation.h"
#include "SkyBox.h"

class CSceneConfig : public CScene
{
public:
	CModelRoche*		m_pModelRoche;		// roché de droite et gauche
	CModelSol*			m_pModelSol;		// sol
	CModelEau*			m_pModelEau;		// eau
	CModelVegetation*   m_pModelVegetation;	// arbres
	CSkyBox*			m_pSkyBox;			// ciel
	CLumiere     m_LumiereD1;				// lumière du soleil

	int m_QualiterFiltrage;					// type de filtrage des textures
	int m_QualiterModel;					// qualité des models
	float m_CoefLODSol[3];					// coeficient pour déterminer le niveau de LOD
	float m_CoefLODSolR[3];					// coeficient pour déterminer le niveau de LOD
	float m_CoefLODRoche[3];				// coeficient pour déterminer le niveau de LOD
	float m_CoefLODRocheR[3];				// coeficient pour déterminer le niveau de LOD

	CSceneConfig(CMoteur* pMoteur);
	~CSceneConfig();
	
	bool ConfirmeFonction3D(D3DCAPS9* pCaps);
	bool Initialisation();
	bool CreationObjet();
	void DestructionObjet();
	void Rendu3D();
	void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris);
	void Animation(double TempsEcouler, double DeltaTemps);
	void Destruction(); 
	void SetFiltreTexture(int Type);
};
