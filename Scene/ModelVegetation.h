
// Classe pour la gestion de la végétation

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"

struct INFOZONEARBRE
{
	D3DXVECTOR3 p;		// position de la zone
	bool  Visible;		// true si visible pour l'affichage
	int   NbTriangle1;	// nombre de triangle pour les arbres de type 1
	int	  NbVertex1;	// nombre de vertex pour les arbres de type 1
	int   Offset1;		// offset dans le vertex buffer pour les arbres de type 1
	int   NbTriangle2;	// nombre de triangle pour les arbres de type 2
	int	  NbVertex2;	// nombre de vertex pour les arbres de type 2
	int   Offset2;		// offset dans le vertex buffer pour les arbres de type 2
	int   NbTriangle3;	// nombre de triangle pour les arbres de type 3
	int	  NbVertex3;	// nombre de vertex pour les arbres de type 3
	int   Offset3;		// offset dans le vertex buffer pour les arbres de type 3
};

const int   NB_VERTEX_VEGETATION    =  8;		// nombre de vertex pour un arbre
const int   NB_INDICE_VEGETATION	= 12;		// nombre d'indice pour un arbre
const int   NB_TRIANGLE_VEGETATION  = 4;		// nombre de triangle pour un arbre
const float LONGEUR_VEGETATION      = 512.0f;	// longeur de la végétation
const float POSITION_Y              = -7.5f;	// position en y des arbres
const float TAILLE_VEGETATION       = 10.0f;	// taille des arbres
const int   NB_ARBRE_MAX            = 650;		// nombre maximum d'arbres
const int   NB_ZONE_VEGETATION      =  8;		// nombre de zone de végétation
const int   NB_MODEL_VEGETATION     =  11;		// nombre de répétition des zones
const int   REF_ZERO                = 168;		// référence du zéro pour la gestion du placement des arbres
const float COEF_HAUTEUR_VEGETATION = 32.0f;	// coéficient pour le calcul de la position en y d'un arbre
const float RAYON_ZONE_VEGETATION   = 100.0f;	// rayon d'une zone

// Nom des fichiers utilisé pour les arbres
static char FichierTxArbre1[] = "Donnees\\Arbre1.dds";
static char FichierTxArbre2[] = "Donnees\\Arbre2.dds";
static char FichierTxArbre3[] = "Donnees\\Arbre3.dds";
static char FichierMapArbre[] = "Donnees\\MapArbre.tga";

class CModelVegetation
{
	// Type de vertex utilisé pour le model
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 p; // position
		D3DXVECTOR2 t; // coordonnées de texture
	};

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

public:
	CScene* m_pScene;				// pointeur sur la scène 3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// pointeur sur le périphérique 3D
	LPDIRECT3DINDEXBUFFER9  m_pIB;	// pointeur sur l'index buffer du model
	LPDIRECT3DVERTEXBUFFER9 m_pVB;	// pointeur sur le vertex buffer du model
	LPDIRECT3DTEXTURE9 m_pTxArbre1;	// pointeur sur la texture d'un arbre
	LPDIRECT3DTEXTURE9 m_pTxArbre2;	// pointeur sur la texture d'un arbre
	LPDIRECT3DTEXTURE9 m_pTxArbre3;	// pointeur sur la texture d'un arbre
	CUSTOMVERTEX* Donnee;			// tableau pour stocker les données du vertex buffer
	D3DXMATRIXA16 m_Matrice[NB_MODEL_VEGETATION];// matrice de trasformation du modèle
	INFOZONEARBRE m_InfoZone[NB_MODEL_VEGETATION][NB_ZONE_VEGETATION];// information sur les zones du modèl

	int m_NbVertexTotal; // nombre de vertex total
	int m_NbIndiceTotal; // nombre d'indice total
	int m_NbArbreTotal;  // nombre d'arbres total

	CModelVegetation(CScene* pScene);
	~CModelVegetation();

	bool ChargerVegetation();
	bool CreationVegetation();
	void Release();
	void RenduVegetation();
	void MiseAJourDonnees();
};