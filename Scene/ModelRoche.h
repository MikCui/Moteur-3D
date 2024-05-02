
// Classe pour la gestion de la roche

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"

// La roche est divisé en plusieur morceau carré (zone)
// Information sur une zone
struct INFOZONEROCHE
{
	D3DXVECTOR3 p;		// position de la zone
	bool  Visible;		// true si visible pour l'affichage
	int   OffsetVertex;	// offset dans le vertex buffer
	int   LOD;			// niveau de détail
};

// Information du un niveau de détail
struct INFOLODROCHE
{
	int   OffsetIndice;	// offset dans l'index buffer
	int   NbVertex;		// nombre de vertex
	int   NbTriangle;	// nombre de triangle
};

const int NB_MODEL_R		=   2;			// nombre de modèl (un modèl à droite et un modèl à gauche
const int NB_ZONE_R			=   8;			// nombre de zone par model
const int NB_VERTEX_R		=  65;			// nombre de vertex d'un coté de la zone
const int NB_TRIANGLE_R		=  64;			// nombre de triangle d'un coté de la zone
const int NB_PIXEL_MAP_X_R	=  67;			// nombre de pixel en largeur de la map de déformation (heighmap)
const int NB_PIXEL_MAP_Y_R	= 515;			// nombre de pixel en hauteur de la map de déformation (heighmap)
const int LOD_MAX_R			=   5;			// nombre de niveaux de détail pour le model
const int NB_MAX_MODEL_R	=  11;			// nombre de répétition du model
const float POSITION_MD_R	=  205.0f;		// position en x du modèl de droite
const float POSITION_MG_R	=    0.0f;		// position en x du modèl de gauche
const float RAYON_ZONE_R	=   50.0f;		// rayon d'une zone
const float COEF_HAUTEUR_R  = 2000.0f;		// coefficient de déformation
const float COEF_TEXTURE_R  = NB_TRIANGLE_R / 3.0f;	// répétition de la texture (3 par zone)

// Nom des fichiers utilisé pour la roche
static char FichierMapRocheDroite[] = "Donnees\\MapRocheDroite.raw";
static char FichierMapRocheGauche[] = "Donnees\\MapRocheGauche.raw";
static char FcihierTxRoche[]        = "Donnees\\Roche.dds";

class CModelRoche
{
	// Type de vertex utilisé pour le model
	struct VERTEXROCHE
	{
		D3DXVECTOR3 p; // position
		D3DXVECTOR3 n; // normale
		D3DXVECTOR2 t; // coordonnées de texture
	};

	#define D3DFVF_VERTEXROCHE (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

public:
	CScene* m_pScene;							// pointeur sur la scène 3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;				// pointeur sur le périphérique 3D
	LPDIRECT3DINDEXBUFFER9  m_pIB;				// pointeur sur l'index buffer du model
	LPDIRECT3DVERTEXBUFFER9 m_pVB;				// pointeur sur le vertex buffer du model
	LPDIRECT3DTEXTURE9 m_pTexRoche;				// pointeur sur la texture de la roche
	VERTEXROCHE DonneeD[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R];// tableau pour stocker les données du vertex buffer
	VERTEXROCHE DonneeG[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R];// tableau pour stocker les données du vertex buffer
	INFOZONEROCHE m_InfoZoneD[NB_MAX_MODEL_R][NB_ZONE_R];// information sur les zones du modèl de droite
	INFOZONEROCHE m_InfoZoneG[NB_MAX_MODEL_R][NB_ZONE_R];// information sur les zones du modèl de gauche
	INFOLODROCHE m_InfoLODD[LOD_MAX_R+1];		// information sur les niveaux de détail du modèl de droite
	INFOLODROCHE m_InfoLODG[LOD_MAX_R+1];		// information sur les niveaux de détail du modèl de gauche
	D3DXMATRIXA16 m_Matrice[NB_MAX_MODEL_R];	// matrice de trasformation pour chaque répétition du modèle
	CMaterielle m_Materiel;						// matériel du terrain pour l'éclairage

	int m_NbVertexZone;		// nombre de vertex par zone
	int m_NbIndiceTotal;	// nomdre d'indice totale
	int m_NbVertexModel;	// nombre de vertex par model
	float m_OffsetLOD  ;    // distance pour atteindre le LOD max
	float m_MaxLOD;         // LOD maximum
	float m_AmplitudeLOD;   // amplitude du LOD

	CModelRoche(CScene* pScene);
	~CModelRoche();

	bool ChargerRoche();
	bool CreationRoche();
	void Release();
	void RenduRoche();
	void MiseAJourDonnees();
	void CreationMaillage(VERTEXROCHE Donnee[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R], bool bSens, char* Fichier);
	void SetCoefLOD(float Offset, float Min);
};