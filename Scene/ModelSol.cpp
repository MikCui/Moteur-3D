#include "ModelSol.h"
#include "..\Outils\ImageRAW.h"

// Constructeur
CModelSol::CModelSol(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexSol = NULL;
	m_pTexMapSol = NULL;

	m_NbVertexZone = NB_VERTEX * NB_VERTEX;
	m_NbVertexModel = NB_VERTEX * NB_VERTEX * NB_ZONE;

	// définition des paramètres du matériel du sol
	m_Materiel.SetAmbiante(0.0f, 0.0f, 0.0f);
	m_Materiel.SetDiffuse(1.3f, 1.2f, 1.0f);
	m_Materiel.SetSpeculaire(1.0f, 1.0f, 1.0f);
	m_Materiel.SetPuissance(100.0f);

	int LOD2; // = 2^LOD
	
	// création des informations sur les niveaux de détail du modèl de droite
    // niveau de base : LOD = 0
	m_InfoLOD[0].NbTriangle = NB_TRIANGLE * NB_TRIANGLE * 2;
	m_InfoLOD[0].NbVertex = NB_VERTEX * NB_VERTEX;
	m_InfoLOD[0].OffsetIndice = 0;
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		m_InfoLOD[LOD].NbTriangle   = (NB_TRIANGLE/LOD2) * (NB_TRIANGLE/LOD2) * 2 + NB_TRIANGLE/LOD2 * 4;
		m_InfoLOD[LOD].NbVertex     = NB_VERTEX * NB_VERTEX;
		m_InfoLOD[LOD].OffsetIndice = m_InfoLOD[LOD-1].OffsetIndice + m_InfoLOD[LOD-1].NbTriangle * 3;
	}
	// niveau max : LOD = LOD_MAX
	LOD2 = int(pow(2.0f, (float)(LOD_MAX-1)));
	m_InfoLOD[LOD_MAX].NbTriangle   = (NB_TRIANGLE/LOD2) * (NB_TRIANGLE/LOD2) * 2;
	m_InfoLOD[LOD_MAX].NbVertex     = NB_VERTEX * NB_VERTEX;
	m_InfoLOD[LOD_MAX].OffsetIndice = m_InfoLOD[LOD_MAX-1].OffsetIndice;

	m_NbIndiceTotal = m_InfoLOD[LOD_MAX-1].OffsetIndice + m_InfoLOD[LOD_MAX-1].NbTriangle * 3;

	// information sur les zones du modèl
	for(int m = 0 ; m < NB_MAX_MODEL ; m++)
	{
		for(int iz = 0 ; iz < NB_ZONE_Z ; iz++)
		{
			for(int ix = 0 ; ix < NB_ZONE_X ; ix++)
			{
				m_InfoZone[m][ix+iz*NB_ZONE_X].p = D3DXVECTOR3(float(NB_TRIANGLE*ix + NB_TRIANGLE/2), POSITION, float(NB_TRIANGLE*iz + NB_TRIANGLE/2));
				m_InfoZone[m][ix+iz*NB_ZONE_X].Visible = true;
				m_InfoZone[m][ix+iz*NB_ZONE_X].OffsetVertex = m_NbVertexZone * (ix+iz*NB_ZONE_X);
				m_InfoZone[m][ix+iz*NB_ZONE_X].LOD = 0;
			}
		}
	}

	// Paramètre par defaut du LOD
	SetCoefLOD(1000.0f, 0.0f);
}

// Déconstructeur
CModelSol::~CModelSol()
{	
}

// Création du sol
bool CModelSol::CreationSol()
{
	CreationMaillage(Donnee, FichierMapSol);
	return true;
}

// Charge les données dans la mémoire vidéo
bool CModelSol::ChargerSol()
{
	VOID* pVerticesVideo = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(m_NbVertexModel*sizeof(CUSTOMVERTEX), 0, D3DFVF_VERTEXSOL, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// Copie des vertex du modèl dans la mémoire video
	m_pVB->Lock(0, m_NbVertexModel*sizeof(CUSTOMVERTEX), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, Donnee, m_NbVertexModel*sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();
	// Création d'un tableau pour stocker les indices
	WORD* pIndices = new WORD[m_NbIndiceTotal];
	WORD* pt = pIndices;

	int LOD2; // = 2^LOD

	// On va remplire le tableau d'indice pour former une liste de triangle de type D3DPT_TRIANGLELIST
    // LOD de base du modèl : LOD = 0
	for(int z = 0 ; z < NB_TRIANGLE ; z++)
	{
		for(int x = 0 ; x < NB_TRIANGLE ; x++)
		{
			*pt++ = (x+1) + (z+1)*NB_VERTEX;
			*pt++ = (x+1) +   (z)*NB_VERTEX;
			*pt++ =   (x) +   (z)*NB_VERTEX;
			*pt++ =   (x) +   (z)*NB_VERTEX;
			*pt++ =   (x) + (z+1)*NB_VERTEX;
			*pt++ = (x+1) + (z+1)*NB_VERTEX;
		}
	}
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		// indice des triangles d'une zone
		for(int z = 0 ; z < NB_TRIANGLE/LOD2 ; z++)
		{
			for(int x = 0 ; x < NB_TRIANGLE/LOD2 ; x++)
			{
				*pt++ = (x+1)*LOD2 + (z+1)*NB_VERTEX*LOD2;
				*pt++ = (x+1)*LOD2 +   (z)*NB_VERTEX*LOD2;
				*pt++ =   (x)*LOD2 +   (z)*NB_VERTEX*LOD2;
				*pt++ =   (x)*LOD2 +   (z)*NB_VERTEX*LOD2;
				*pt++ =   (x)*LOD2 + (z+1)*NB_VERTEX*LOD2;
				*pt++ = (x+1)*LOD2 + (z+1)*NB_VERTEX*LOD2;
			}
		}
		// indice des triangles de jointure entre les zones
		for(int x = 0 ; x < NB_TRIANGLE ; x+=LOD2)
		{
			*pt++ = x+LOD2   +           0*NB_VERTEX;
			*pt++ = x+LOD2/2 +           0*NB_VERTEX;
			*pt++ = x        +           0*NB_VERTEX;
			*pt++ = x        + NB_TRIANGLE*NB_VERTEX;
			*pt++ = x+LOD2/2 + NB_TRIANGLE*NB_VERTEX;
			*pt++ = x+LOD2   + NB_TRIANGLE*NB_VERTEX;		
		}
		for(int z = 0 ; z < NB_TRIANGLE ; z+=LOD2)
		{
			*pt++ =           0 +        (z)*NB_VERTEX;
			*pt++ =           0 + (z+LOD2/2)*NB_VERTEX;
			*pt++ =           0 +   (z+LOD2)*NB_VERTEX;
			*pt++ = NB_TRIANGLE +   (z+LOD2)*NB_VERTEX;
			*pt++ = NB_TRIANGLE + (z+LOD2/2)*NB_VERTEX;
			*pt++ = NB_TRIANGLE +        (z)*NB_VERTEX;
		}
	}

	VOID* pIndicesVideo = NULL;
	// Création de l'index buffer
	m_pD3DDevice->CreateIndexBuffer(m_NbIndiceTotal*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
	m_pIB->Lock(0, m_NbIndiceTotal*sizeof(WORD), (void**)&pIndicesVideo, 0);
	// Copie des indices dans la mémoire video
	memcpy(pIndicesVideo, pIndices, m_NbIndiceTotal*sizeof(WORD));
	m_pIB->Unlock();

	delete[] pIndices;

	// Chargement des textures
	D3DXCreateTextureFromFile(m_pD3DDevice, FcihierTxSol,  &m_pTexSol);
	D3DXCreateTextureFromFile(m_pD3DDevice, FcihierTxMapSol,  &m_pTexMapSol);
	return true;
}

// Libération des ressources
void CModelSol::Release()
{
	SAFE_RELEASE(m_pTexSol);
	SAFE_RELEASE(m_pTexMapSol);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

// Mise à jour des informations des zones
void CModelSol::MiseAJourDonnees()
{
	static D3DXVECTOR3 PositionCamera;
	static D3DXVECTOR3 PositionZone;
	static D3DXVECTOR3 DeltaPosition;
	static int   ProfondeurModel = NB_ZONE_Z * NB_TRIANGLE;
	static float OffsetZone = -float(ProfondeurModel * NB_MAX_MODEL)/2.0f;
	static float OffsetModel;
	static float Delta;

	// On récupère la position de la caméra
	PositionCamera = m_pScene->GetCameraCourante()->GetPosition();
	
	// Mise à jour des informations des zones
	for(int m = 0 ; m < NB_MAX_MODEL ; m++)
	{
		// Calcul de la matrice de trasformation du model
		OffsetModel = float(ProfondeurModel * m + (int(PositionCamera.z) / ProfondeurModel) * ProfondeurModel) + OffsetZone;
		D3DXMatrixTranslation(&m_Matrice[m], 0.0f, 0.0f, OffsetModel);
		for(int iz = 0 ; iz < NB_ZONE_Z ; iz++)
		{
			for(int ix = 0 ; ix < NB_ZONE_X ; ix++)
			{
				// Calcul de la position de la zone
				PositionZone = m_InfoZone[m][ix+iz*NB_ZONE_X].p;
				PositionZone.z += OffsetModel;
				// On test si la zone est visible
				m_InfoZone[m][ix+iz*NB_ZONE_X].Visible = m_pScene->GetCameraCourante()->IsVisible(&PositionZone, RAYON_ZONE);
				// Calcul de la distance entre la zone et la camera
				DeltaPosition = PositionZone - PositionCamera;
				Delta = D3DXVec3Length(&DeltaPosition);
				// Calcul du niveau de détail en fonction de la distance
				if(Delta > m_OffsetLOD) m_InfoZone[m][ix+iz*NB_ZONE_X].LOD = LOD_MAX;
				else m_InfoZone[m][ix+iz*NB_ZONE_X].LOD = int(-((Delta-m_OffsetLOD)/m_AmplitudeLOD)*((Delta-m_OffsetLOD)/m_AmplitudeLOD)+m_MaxLOD);
			}
		}
	}
}

// Rendu du sol
void CModelSol::RenduSol()
{
	static int LOD; // niveau LOD
	// Activation de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Initialisation du matériel
	m_pD3DDevice->SetMaterial(m_Materiel.GetMaterielle());
	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTexture(0, m_pTexMapSol);
	m_pD3DDevice->SetTexture(1, m_pTexSol);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	// Initialisation des buffers
	m_pD3DDevice->SetFVF(D3DFVF_VERTEXSOL);
	m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pD3DDevice->SetIndices(m_pIB);
	// Rendu de chaque zones
	for(int m = 0 ; m < NB_MAX_MODEL ; m++)
	{
		// Initialisation de la matrice de transformation
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[m]);
		for(int iz = 0 ; iz < NB_ZONE_Z ; iz++)
		{
			for(int ix = 0 ; ix < NB_ZONE_X ; ix++)
			{
				// Rendu zone
				if(m_InfoZone[m][ix+iz*NB_ZONE_X].Visible)
				{
					LOD = m_InfoZone[m][ix+iz*NB_ZONE_X].LOD;
					m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
													m_InfoZone[m][ix+iz*NB_ZONE_X].OffsetVertex, 0, 
													m_InfoLOD[LOD].NbVertex, 
													m_InfoLOD[LOD].OffsetIndice, 
													m_InfoLOD[LOD].NbTriangle);
				}
			}
		}
	}
}

// Création du maillage du sol
void CModelSol::CreationMaillage(CUSTOMVERTEX Donnee[NB_ZONE][NB_VERTEX][NB_VERTEX], char* Fichier)
{
	CImageRAW Image;
	// On charge la map de déformation (heighmap)
	Image.Charger(Fichier, NB_PIXEL_MAP_X, NB_PIXEL_MAP_Y, 1, 16);
	// On récupère un pointeur sur les données
	unsigned short* DonneeImage = (unsigned short*)Image.GetDonnees();

	// Tableau pour calculer l'amplitude de la déformation
	float DonneeBrute[NB_VERTEX+2][NB_VERTEX+2];
	
	D3DXVECTOR3 v1, v2, v3, v4, v5, v6, v;

	// On parcours chaque zone
	for(int iz = 0 ; iz < NB_ZONE_Z ; iz++)
	{
		for(int ix = 0 ; ix < NB_ZONE_X ; ix++)
		{
			// Calcul de l'amplitude de déformation
			for(int imy = 0; imy < NB_VERTEX+2; imy++)
				for(int imx = 0; imx < NB_VERTEX+2; imx++)
					DonneeBrute[imy][imx] = (float)DonneeImage[NB_PIXEL_MAP_X * (imy + NB_TRIANGLE * iz) + (imx + NB_TRIANGLE * ix)] / COEF_HAUTEUR;

			// Calcul des vertex d'une zone
			for(int z = 0; z < NB_VERTEX; z++)
			{
				for(int x = 0; x < NB_VERTEX; x++)
				{
					Donnee[ix+iz*NB_ZONE_X][z][x].n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					// Calcul des vertex adjacent au vertex
					v1 = D3DXVECTOR3( 0.0f, DonneeBrute[z+2][x+1] - DonneeBrute[z+1][x+1],  1.0f);
					v2 = D3DXVECTOR3( 1.0f, DonneeBrute[z+2][x+2] - DonneeBrute[z+1][x+1],  1.0f);
					v3 = D3DXVECTOR3( 1.0f, DonneeBrute[z+1][x+2] - DonneeBrute[z+1][x+1],  0.0f);
					v4 = D3DXVECTOR3( 0.0f, DonneeBrute[  z][x+1] - DonneeBrute[z+1][x+1], -1.0f);
					v5 = D3DXVECTOR3(-1.0f, DonneeBrute[  z][  x] - DonneeBrute[z+1][x+1], -1.0f);
					v6 = D3DXVECTOR3(-1.0f, DonneeBrute[z+1][  x] - DonneeBrute[z+1][x+1],  0.0f);
					// Calcul de la normale
					D3DXVec3Cross(&v, &v1, &v2);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					D3DXVec3Cross(&v, &v2, &v3);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					D3DXVec3Cross(&v, &v3, &v4);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					D3DXVec3Cross(&v, &v4, &v5);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					D3DXVec3Cross(&v, &v5, &v6);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					D3DXVec3Cross(&v, &v6, &v1);
					D3DXVec3Normalize(&v, &v);
					D3DXVec3Add(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n, &v);
					// Normalisation de la normale
					D3DXVec3Normalize(&Donnee[ix+iz*NB_ZONE_X][z][x].n, &Donnee[ix+iz*NB_ZONE_X][z][x].n);
					// Corrdonnées de texture
					Donnee[ix+iz*NB_ZONE_X][z][x].t1 = D3DXVECTOR2(float(x + NB_TRIANGLE * ix)/float(NB_TRIANGLE * NB_ZONE_X), float(z + NB_TRIANGLE * iz)/float(NB_TRIANGLE * NB_ZONE_Z));
					Donnee[ix+iz*NB_ZONE_X][z][x].t2 = D3DXVECTOR2(float(z) / COEF_TEXTURE, float(x) / COEF_TEXTURE);
					// Position du vertex
					Donnee[ix+iz*NB_ZONE_X][z][x].p = D3DXVECTOR3(float(x + NB_TRIANGLE * ix), DonneeBrute[z+1][x+1]+POSITION, float(z + NB_TRIANGLE * iz));
				}
			}
		}
	}
}

// Calcul les paramètres du LOD
void CModelSol::SetCoefLOD(float Offset, float Min)
{
	m_OffsetLOD = Offset;
	m_MaxLOD = LOD_MAX - 1 + 0.1f;
	m_AmplitudeLOD = m_OffsetLOD / sqrt(m_MaxLOD-Min);
}