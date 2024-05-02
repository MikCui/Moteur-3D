
// Classe pour la gestion de l'eau

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"
#include "..\Moteur\EnvMiroir.h"

const int   NB_VERTEX_EAU   = 4;		// nombre de vertex pour une zone d'eau
const int   NB_INDICE_EAU	= 6;		// nombre d'indice pour une zone d'eau
const int   NB_TRIANGLE_EAU = 2;		// nombre de triangle pour une zone d'eau
const float POSITION_EAU_X  = 97.0f;	// position en x de l'eau
const float POSITION_EAU_Y  = -3.0f;	// position en y de l'eau
const int   NB_ZONE_EAU     = 11;		// nombre de zone d'eau
const float LARGEUR			= 128.0f;	// largeur de l'eau
const float LONGEUR			= 512.0f;	// longeur de l'eau
const float COEF_TEXTURE_X	= 0.5f;		// répétition de la texture
const float COEF_TEXTURE_Y	= 2.0f;		// répétition de la texture
const float VITESSE_EAU		= -60.0f;	// vitesse de déplacement de l'eau

// Nom des fichiers utilisé pour l'eau
static char FcihierTxBump[] = "Donnees\\EauBump.dds";

class CModelEau
{
	// Type de vertex utilisé pour le model
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 p; // position
		D3DXVECTOR2 t; // coordonnées de texture
	};

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

public:
	CScene* m_pScene;					// pointeur sur la scène 3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// pointeur sur le périphérique 3D
	LPDIRECT3DINDEXBUFFER9  m_pIB;		// pointeur sur l'index buffer du model
	LPDIRECT3DVERTEXBUFFER9 m_pVB;		// pointeur sur le vertex buffer du model
	LPDIRECT3DTEXTURE9 m_pTexBump;		// pointeur sur la texture de bump mapping
	CUSTOMVERTEX Donnee[NB_VERTEX_EAU]; // tableau pour stocker les données du vertex buffer
	D3DXMATRIXA16 m_Matrice[NB_ZONE_EAU];// matrice de trasformation du modèle
	D3DXPLANE m_Plan;					// plan du sol utilisé pour la réflexion
	CEnvMiroir* m_pEnvMiroir;			// pointeur sur un objet pour le rendu de l'environement
	bool m_RenduMiroir;					// permet de savoir si la réflexion est en cour de rendu
	D3DXMATRIXA16 m_MatriceReflection;	// matrice de réflexion
	D3DXMATRIXA16 m_MatTransEau;		// matrice de transformation de l'eau

	CModelEau(CScene* pScene);
	~CModelEau();

	bool ChargerEau();
	bool CreationEau();
	void Release();
	void RenduEau();
	void MiseAJourDonnees(double TempsEcouler);
	void RenduEnvMiroir();
	D3DXMATRIXA16 GetMatriceReflection(){return m_MatriceReflection;}
};