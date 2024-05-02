
// Classe pour la gestion de la roche

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"

// La roche est divis� en plusieur morceau carr� (zone)
// Information sur une zone
struct INFOZONEROCHE
{
	D3DXVECTOR3 p;		// position de la zone
	bool  Visible;		// true si visible pour l'affichage
	int   OffsetVertex;	// offset dans le vertex buffer
	int   LOD;			// niveau de d�tail
};

// Information du un niveau de d�tail
struct INFOLODROCHE
{
	int   OffsetIndice;	// offset dans l'index buffer
	int   NbVertex;		// nombre de vertex
	int   NbTriangle;	// nombre de triangle
};

const int NB_MODEL_R		=   2;			// nombre de mod�l (un mod�l � droite et un mod�l � gauche
const int NB_ZONE_R			=   8;			// nombre de zone par model
const int NB_VERTEX_R		=  65;			// nombre de vertex d'un cot� de la zone
const int NB_TRIANGLE_R		=  64;			// nombre de triangle d'un cot� de la zone
const int NB_PIXEL_MAP_X_R	=  67;			// nombre de pixel en largeur de la map de d�formation (heighmap)
const int NB_PIXEL_MAP_Y_R	= 515;			// nombre de pixel en hauteur de la map de d�formation (heighmap)
const int LOD_MAX_R			=   5;			// nombre de niveaux de d�tail pour le model
const int NB_MAX_MODEL_R	=  11;			// nombre de r�p�tition du model
const float POSITION_MD_R	=  205.0f;		// position en x du mod�l de droite
const float POSITION_MG_R	=    0.0f;		// position en x du mod�l de gauche
const float RAYON_ZONE_R	=   50.0f;		// rayon d'une zone
const float COEF_HAUTEUR_R  = 2000.0f;		// coefficient de d�formation
const float COEF_TEXTURE_R  = NB_TRIANGLE_R / 3.0f;	// r�p�tition de la texture (3 par zone)

// Nom des fichiers utilis� pour la roche
static char FichierMapRocheDroite[] = "Donnees\\MapRocheDroite.raw";
static char FichierMapRocheGauche[] = "Donnees\\MapRocheGauche.raw";
static char FcihierTxRoche[]        = "Donnees\\Roche.dds";

class CModelRoche
{
	// Type de vertex utilis� pour le model
	struct VERTEXROCHE
	{
		D3DXVECTOR3 p; // position
		D3DXVECTOR3 n; // normale
		D3DXVECTOR2 t; // coordonn�es de texture
	};

	#define D3DFVF_VERTEXROCHE (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

public:
	CScene* m_pScene;							// pointeur sur la sc�ne 3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;				// pointeur sur le p�riph�rique 3D
	LPDIRECT3DINDEXBUFFER9  m_pIB;				// pointeur sur l'index buffer du model
	LPDIRECT3DVERTEXBUFFER9 m_pVB;				// pointeur sur le vertex buffer du model
	LPDIRECT3DTEXTURE9 m_pTexRoche;				// pointeur sur la texture de la roche
	VERTEXROCHE DonneeD[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R];// tableau pour stocker les donn�es du vertex buffer
	VERTEXROCHE DonneeG[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R];// tableau pour stocker les donn�es du vertex buffer
	INFOZONEROCHE m_InfoZoneD[NB_MAX_MODEL_R][NB_ZONE_R];// information sur les zones du mod�l de droite
	INFOZONEROCHE m_InfoZoneG[NB_MAX_MODEL_R][NB_ZONE_R];// information sur les zones du mod�l de gauche
	INFOLODROCHE m_InfoLODD[LOD_MAX_R+1];		// information sur les niveaux de d�tail du mod�l de droite
	INFOLODROCHE m_InfoLODG[LOD_MAX_R+1];		// information sur les niveaux de d�tail du mod�l de gauche
	D3DXMATRIXA16 m_Matrice[NB_MAX_MODEL_R];	// matrice de trasformation pour chaque r�p�tition du mod�le
	CMaterielle m_Materiel;						// mat�riel du terrain pour l'�clairage

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