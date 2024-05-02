
// Classe pour la gestion de la partie 3D de l'appilcation

#pragma once
#include "VariableGlobale.h"
#include "Moteur.h"

class CMoteur;

class CMoteur3D
{
public:
	CMoteur*          m_pMoteur;						// pointeur sur le Moteur
	LPDIRECT3D9       m_pD3D;							// pointeur sur l'objet Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// pointeur sur l'objet Direct3DDevice
	char m_Description[MAX_DEVICE_IDENTIFIER_STRING];	// description de la carte 3D (nom)
	D3DPRESENT_PARAMETERS m_d3dpp;						// paramètres des buffers images

	CMoteur3D();
	CMoteur3D(CMoteur* pMoteur);
	~CMoteur3D();

	bool Creation(HWND hWnd, int Largeur = 640, int Hauteur = 480, bool bPleinEcran = false);
	void Destruction();
	bool Redimentionnement(int Largeur, int Hauteur);
	bool SetModeFenetre(int &Largeur, int &Hauteur, bool bPleinEcran);
	LPDIRECT3DDEVICE9 GetDevice3D(){return m_pD3DDevice;}
	CMoteur* GetMoteur() {return m_pMoteur;}
	char* GetDescription() {return m_Description;}
	bool RAZPeripherique3D();
};
