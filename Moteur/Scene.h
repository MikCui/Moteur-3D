
// Classe pour la gestion d'une sc�ne 3D

#pragma once
#include "VariableGlobale.h"
#include "Materielle.h"
#include "Lumiere.h"
#include "Moteur.h"
#include "Bouton.h"
#include "Camera.h"
#include "Model.h"

#define KEYDOWN(key) (EtatClavier[key] & 0x80)

class CMoteur;
class CCamera;
class CBouton;
class CModel;

class CScene
{
private:
	CScene();

public:
	LPDIRECT3DDEVICE9   m_pD3DDevice;			// pointeur sur l'objet Direct3DDevice
	CMoteur*			m_pMoteur;				// pointeur sur le Moteur 
	CCamera*            m_pCameraCourante;		// pointeur sur la cam�ra courant dans la sc�ne
	CCamera*            m_pCameraParDefaut;		// pointeur sur une cam�ra cr�e par defaut
	CLumiere			m_LumiereParDefaut;		// lumi�re par defaut de la sc�ne
	CMaterielle			m_MaterielleParDefaut;	// materielle par defaut de la sc�ne
	bool				m_bRenduReflection;		// permet de savoir si le rendu est �ffectu� pour une r�flection
	bool				m_bRenduBump;			// permer de savoir si le rendu est �ffectu� avec bump mapping

	BYTE*         m_pEtatClavier;				// pointeur sur l'�tats du clavier
	DIMOUSESTATE* m_pEtatSouris;				// pointeur sur l'�tats de la souris
	float		  m_RatioFenetre;				// ratio largeur/hauteur de la sc�ne
	//* m_pEtatJoystick;

	CMesureTemps m_Temps;						// Compteur pour la gestion du temps

	CScene(CMoteur* pMoteur); 
	~CScene();

	CMoteur*			GetMoteur(){return m_pMoteur;}
	BYTE*				GetTableauEtatClavier(){return m_pEtatClavier;}
	DIMOUSESTATE*		GetEtatSouris(){return m_pEtatSouris;}
	//* GetEtatJoystick(){return m_pEtatJoystick;}
	LPDIRECT3DDEVICE9   Get3DDevice(){return m_pD3DDevice;}
	void SetCamera(CCamera* pCamera);
	void SetMaterielle(CMaterielle* Materielle){m_pD3DDevice->SetMaterial(Materielle->GetMaterielle());}
	void SetLumiere(int NumerosLumiere, CLumiere* Lumirere, bool bActive = false)
	{
		m_pD3DDevice->SetLight(NumerosLumiere, Lumirere->GetLumiere());
		m_pD3DDevice->LightEnable(NumerosLumiere, bActive);
	}
	void SetLumiereActive(int NumerosLumiere, bool bActive){m_pD3DDevice->LightEnable(NumerosLumiere, bActive);}
	float GetRatioFenetre(){return m_RatioFenetre;}
	void SetRatioFenetre(float Ratio){m_RatioFenetre = Ratio;}
	CCamera* GetCameraCourante(){return m_pCameraCourante;}
	bool IsRenduReflection(){return m_bRenduReflection;}
	bool IsRenduBump(){return m_bRenduBump;}

	bool Creation();
	void DestructionFinal();
	
	virtual bool ConfirmeFonction3D(D3DCAPS9* pCaps){return true;}
	virtual bool Initialisation(){return true;}
	virtual bool CreationObjet();
	virtual void Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris){}
	virtual void Animation(double TempsEcouler, double DeltaTemps){}
	virtual void Rendu();
	virtual void DebutRendu3D();
	virtual void Rendu3D(){}
	virtual HRESULT FinRendu3D();
	//virtual void RenduSon(){}
	//virtual void RenduMusic(){}

	virtual void DestructionObjet(){}
	virtual void Destruction(){}

};
