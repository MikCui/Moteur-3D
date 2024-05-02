
// Classe pour la gestion du sol

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"

// Le sol est divis� en plusieur morceau carr� (zone)
// Information sur une zone
struct INFOZONE
{
	D3DXVECTOR3 p;		// position de la zone
	bool  Visible;		// true si visible pour l'affichage
	int   OffsetVertex;	// offset dans le vertex buffer
	int   LOD;			// niveau de d�tail
};

// Information du un niveau de d�tail
struct INFOLOD
{
	int   OffsetIndice;	// offset dans l'index buffer
	int   NbVertex;		// nombre de vertex
	int   NbTriangle;	// nombre de triangle
};

const int NB_ZONE_X      =   3;			// nombre de zone en x
const int NB_ZONE_Z      =   8;			// nombre de zone en z
const int NB_ZONE        =   NB_ZONE_X * NB_ZONE_Z;// nombre de zone total
const int NB_VERTEX      =  65;			// nombre de vertex d'un cot� de la zone
const int NB_TRIANGLE    =  64;			// nombre de triangle d'un cot� de la zone
const int NB_PIXEL_MAP_X = 195;			// nombre de pixel en largeur de la map de d�formation (heighmap)
const int NB_PIXEL_MAP_Y = 515;			// nombre de pixel en hauteur de la map de d�formation (heighmap)
const int LOD_MAX        =   5;			// nombre de niveaux de d�tail pour le model
const int NB_MAX_MODEL   =  11;			// nombre de r�p�tition du model
const float POSITION     =  -7.0f;		// position en y du mod�l
const float RAYON_ZONE	 =  50.0f;		// rayon d'une zone
const float COEF_HAUTEUR = 8000.0f;		// coefficient de d�formation
const float COEF_TEXTURE = NB_TRIANGLE / 6.0f;	// r�p�tition de la texture (6 par zone)

// Nom des fichiers utilis� pour le sol
static char FichierMapSol[]   = "Donnees\\MapSol.raw";
static char FcihierTxMapSol[] = "Donnees\\MapSol.dds";
static char FcihierTxSol[]    = "Donnees\\sable.dds";

class CModelSol
{
	// Type de vertex utilis� pour le model
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 p; // position
		D3DXVECTOR3 n; // normale
		D3DXVECTOR2 t1; // coordonn�es de texture
		D3DXVECTOR2 t2; // coordonn�es de texture
	};

	#define D3DFVF_VERTEXSOL (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)

public:
	CScene* m_pScene;							// pointeur sur la sc�ne 3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;				// pointeur sur le p�riph�rique 3D
	LPDIRECT3DINDEXBUFFER9  m_pIB;				// pointeur sur l'index buffer du model
	LPDIRECT3DVERTEXBUFFER9 m_pVB;				// pointeur sur le vertex buffer du model
	LPDIRECT3DTEXTURE9 m_pTexSol;				// pointeur sur la texture du sol
	LPDIRECT3DTEXTURE9 m_pTexMapSol;			// pointeur sur la texture de map du sol
	CUSTOMVERTEX Donnee[NB_ZONE][NB_VERTEX][NB_VERTEX];// tableau pour stocker les donn�es du vertex buffer
	INFOZONE m_InfoZone[NB_MAX_MODEL][NB_ZONE]; // information sur les zones du mod�l
	INFOLOD m_InfoLOD[LOD_MAX+1];				// information sur les niveaux de d�tail du mod�l
	D3DXMATRIXA16 m_Matrice[NB_MAX_MODEL];		// matrice de trasformation pour chaque r�p�tition du mod�le
	CMaterielle m_Materiel;						// mat�riel du sol pour l'�clairage

	int m_NbVertexZone;		// nombre de vertex par zone
	int m_NbIndiceTotal;	// nomdre d'indice totale
	int m_NbVertexModel;	// nombre de vertex par model
	float m_OffsetLOD  ;    // distance pour atteindre le LOD max
	float m_MaxLOD;         // LOD maximum
	float m_AmplitudeLOD;   // amplitude du LOD

	CModelSol(CScene* pScene);
	~CModelSol();

	bool ChargerSol();
	bool CreationSol();
	void Release();
	void RenduSol();
	void MiseAJourDonnees();
	void CreationMaillage(CUSTOMVERTEX Donnee[NB_ZONE][NB_VERTEX][NB_VERTEX], char* Fichier);
	void SetCoefLOD(float Offset, float Min);
};