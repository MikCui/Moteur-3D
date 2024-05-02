#include "ModelRoche.h"
#include "..\Outils\ImageRAW.h"

// Constructeur
CModelRoche::CModelRoche(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexRoche = NULL;

	m_NbVertexZone = NB_VERTEX_R * NB_VERTEX_R;
	m_NbVertexModel = NB_VERTEX_R * NB_VERTEX_R * NB_ZONE_R;

	// définition des paramètres du matériel de la roche
	m_Materiel.SetAmbiante(0.5f, 0.5f, 0.5f);
	m_Materiel.SetDiffuse(1.4f, 1.3f, 1.0f);
	m_Materiel.SetSpeculaire(1.0f, 1.0f, 1.0f);
	m_Materiel.SetPuissance(100.0f);

	int LOD2; // = 2^LOD
	
	// création des informations sur les niveaux de détail du modèl de droite
    // niveau de base : LOD = 0
	m_InfoLODD[0].NbTriangle = NB_TRIANGLE_R * NB_TRIANGLE_R * 2;
	m_InfoLODD[0].NbVertex = NB_VERTEX_R * NB_VERTEX_R;
	m_InfoLODD[0].OffsetIndice = 0;
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX_R; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		m_InfoLODD[LOD].NbTriangle   = (NB_TRIANGLE_R/LOD2) * (NB_TRIANGLE_R/LOD2) * 2 + NB_TRIANGLE_R/LOD2 * 2;
		m_InfoLODD[LOD].NbVertex     = NB_VERTEX_R * NB_VERTEX_R;
		m_InfoLODD[LOD].OffsetIndice = m_InfoLODD[LOD-1].OffsetIndice + m_InfoLODD[LOD-1].NbTriangle * 3;
	}
	// niveau max : LOD = LOD_MAX
	LOD2 = int(pow(2.0f, (float)(LOD_MAX_R-1)));
	m_InfoLODD[LOD_MAX_R].NbTriangle   = (NB_TRIANGLE_R/LOD2) * (NB_TRIANGLE_R/LOD2) * 2;
	m_InfoLODD[LOD_MAX_R].NbVertex     = NB_VERTEX_R * NB_VERTEX_R;
	m_InfoLODD[LOD_MAX_R].OffsetIndice = m_InfoLODD[LOD_MAX_R-1].OffsetIndice;

	// création des informations sur les niveaux de détail du modèl de gauche
	// niveau de base : LOD = 0
	m_InfoLODG[0].NbTriangle = NB_TRIANGLE_R * NB_TRIANGLE_R * 2;
	m_InfoLODG[0].NbVertex = NB_VERTEX_R * NB_VERTEX_R;
	m_InfoLODG[0].OffsetIndice = m_InfoLODD[LOD_MAX_R-1].OffsetIndice + m_InfoLODD[LOD_MAX_R-1].NbTriangle * 3;
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX_R; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		m_InfoLODG[LOD].NbTriangle   = (NB_TRIANGLE_R/LOD2) * (NB_TRIANGLE_R/LOD2) * 2 + NB_TRIANGLE_R/LOD2 * 2;
		m_InfoLODG[LOD].NbVertex     = NB_VERTEX_R * NB_VERTEX_R;
		m_InfoLODG[LOD].OffsetIndice = m_InfoLODG[LOD-1].OffsetIndice + m_InfoLODG[LOD-1].NbTriangle * 3;
	}
	// niveau max : LOD = LOD_MAX
	LOD2 = int(pow(2.0f, (float)(LOD_MAX_R-1)));
	m_InfoLODG[LOD_MAX_R].NbTriangle   = (NB_TRIANGLE_R/LOD2) * (NB_TRIANGLE_R/LOD2) * 2;
	m_InfoLODG[LOD_MAX_R].NbVertex     = NB_VERTEX_R * NB_VERTEX_R;
	m_InfoLODG[LOD_MAX_R].OffsetIndice = m_InfoLODG[LOD_MAX_R-1].OffsetIndice;

	m_NbIndiceTotal = m_InfoLODG[LOD_MAX_R-1].OffsetIndice + m_InfoLODG[LOD_MAX_R-1].NbTriangle * 3;

	// information sur les zones du modèl de droite et de gauche
	for(int j = 0 ; j < NB_MAX_MODEL_R ; j++)
	{
		for(int i = 0 ; i < NB_ZONE_R ; i++)
		{
			m_InfoZoneD[j][i].p = D3DXVECTOR3(POSITION_MD_R, float(NB_TRIANGLE_R/2), float(NB_TRIANGLE_R*i + NB_TRIANGLE_R/2));
			m_InfoZoneD[j][i].Visible = true;
			m_InfoZoneD[j][i].OffsetVertex = m_NbVertexZone * i;
			m_InfoZoneD[j][i].LOD = 0;

			m_InfoZoneG[j][i].p = D3DXVECTOR3(POSITION_MG_R, float(NB_TRIANGLE_R/2), float(NB_TRIANGLE_R*i + NB_TRIANGLE_R/2));
			m_InfoZoneG[j][i].Visible = true;
			m_InfoZoneG[j][i].OffsetVertex = m_NbVertexZone * (NB_ZONE_R+i);
			m_InfoZoneG[j][i].LOD = 0;
		}
	}

	// Paramètre par defaut du LOD
	SetCoefLOD(1000.0f, 0.0f);
}

// Déconstructeur
CModelRoche::~CModelRoche()
{	
}

// Création de la roche
bool CModelRoche::CreationRoche()
{
	CreationMaillage(DonneeD,  true, FichierMapRocheDroite);
	CreationMaillage(DonneeG, false, FichierMapRocheGauche);
	return true;
}

// Charge les données dans la mémoire vidéo
bool CModelRoche::ChargerRoche()
{
	VOID* pVerticesVideo = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(m_NbVertexModel*NB_MODEL_R*sizeof(VERTEXROCHE), 0, D3DFVF_VERTEXROCHE, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// Copie des vertex du modèl de droite dans la mémoire video
	m_pVB->Lock(0, m_NbVertexModel*sizeof(VERTEXROCHE), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, DonneeD, m_NbVertexModel*sizeof(VERTEXROCHE));
	m_pVB->Unlock();
	// Copie des vertex du modèl de gauche dans la mémoire video
	m_pVB->Lock(m_NbVertexModel*sizeof(VERTEXROCHE), m_NbVertexModel*sizeof(VERTEXROCHE), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, DonneeG, m_NbVertexModel*sizeof(VERTEXROCHE));
	m_pVB->Unlock();
	// Création d'un tableau pour stocker les indices
	WORD* pIndices = new WORD[m_NbIndiceTotal];
	WORD* pt = pIndices;

	int LOD2; // = 2^LOD

	// On va remplire le tableau d'indice pour former une liste de triangle de type D3DPT_TRIANGLELIST
    // LOD de base du modèl de droite : LOD = 0
	for(int z = 0 ; z < NB_TRIANGLE_R ; z++)
	{
		for(int y = 0 ; y < NB_TRIANGLE_R ; y++)
		{
			*pt++ = (y+1) + (z+1)*NB_VERTEX_R;
			*pt++ = (y+1) +   (z)*NB_VERTEX_R;
			*pt++ =   (y) +   (z)*NB_VERTEX_R;
			*pt++ =   (y) +   (z)*NB_VERTEX_R;
			*pt++ =   (y) + (z+1)*NB_VERTEX_R;
			*pt++ = (y+1) + (z+1)*NB_VERTEX_R;
		}
	}
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX_R; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		// indice des triangles d'une zone
		for(int z = 0 ; z < NB_TRIANGLE_R/LOD2 ; z++)
		{
			for(int y = 0 ; y < NB_TRIANGLE_R/LOD2 ; y++)
			{
				*pt++ = (y+1)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
				*pt++ = (y+1)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
				*pt++ =   (y)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
				*pt++ =   (y)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
				*pt++ =   (y)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
				*pt++ = (y+1)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
			}
		}
		// indice des triangles de jointure entre les zones
		for(int y = 0 ; y < NB_TRIANGLE_R ; y+=LOD2)
		{
			*pt++ = y+LOD2   +             0*NB_VERTEX_R;
			*pt++ = y+LOD2/2 +             0*NB_VERTEX_R;
			*pt++ = y        +             0*NB_VERTEX_R;
			*pt++ = y        + NB_TRIANGLE_R*NB_VERTEX_R;
			*pt++ = y+LOD2/2 + NB_TRIANGLE_R*NB_VERTEX_R;
			*pt++ = y+LOD2   + NB_TRIANGLE_R*NB_VERTEX_R;		
		}
	}

	// LOD de base du modèl de gauche : LOD = 0
	for(int z = 0 ; z < NB_TRIANGLE_R ; z++)
	{
		for(int y = 0 ; y < NB_TRIANGLE_R ; y++)
		{
			*pt++ =   (y) +   (z)*NB_VERTEX_R;
			*pt++ = (y+1) +   (z)*NB_VERTEX_R;
			*pt++ = (y+1) + (z+1)*NB_VERTEX_R;
			*pt++ = (y+1) + (z+1)*NB_VERTEX_R;
			*pt++ =   (y) + (z+1)*NB_VERTEX_R;
			*pt++ =   (y) +   (z)*NB_VERTEX_R;
		}
	}
	// différents niveau de détail
	for(int LOD = 1; LOD < LOD_MAX_R; LOD++)
	{
		LOD2 = int(pow(2.0f, (float)LOD));
		// indice des triangles d'une zone
		for(int z = 0 ; z < NB_TRIANGLE_R/LOD2 ; z++)
		{
			for(int y = 0 ; y < NB_TRIANGLE_R/LOD2 ; y++)
			{
				*pt++ =   (y)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
				*pt++ = (y+1)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
				*pt++ = (y+1)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
				*pt++ = (y+1)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
				*pt++ =   (y)*LOD2 + (z+1)*NB_VERTEX_R*LOD2;
				*pt++ =   (y)*LOD2 +   (z)*NB_VERTEX_R*LOD2;
			}
		}
		// indice des triangles de jointure entre les zones
		for(int y = 0 ; y < NB_TRIANGLE_R ; y+=LOD2)
		{
			*pt++ = y        +             0*NB_VERTEX_R;
			*pt++ = y+LOD2/2 +             0*NB_VERTEX_R;
			*pt++ = y+LOD2   +             0*NB_VERTEX_R;
			*pt++ = y+LOD2   + NB_TRIANGLE_R*NB_VERTEX_R;
			*pt++ = y+LOD2/2 + NB_TRIANGLE_R*NB_VERTEX_R;
			*pt++ = y        + NB_TRIANGLE_R*NB_VERTEX_R;
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

	// Chargement de la texture
	D3DXCreateTextureFromFile(m_pD3DDevice, FcihierTxRoche,  &m_pTexRoche);
	return true;
}

// Libération des ressources
void CModelRoche::Release()
{
	SAFE_RELEASE(m_pTexRoche);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

// Mise à jour des informations des zones
void CModelRoche::MiseAJourDonnees()
{
	static D3DXVECTOR3 PositionCamera;
	static D3DXVECTOR3 PositionZone;
	static D3DXVECTOR3 DeltaPosition;
	static int   ProfondeurModel = NB_ZONE_R * NB_TRIANGLE_R;
	static float OffsetZone = -float(ProfondeurModel * NB_MAX_MODEL_R)/2.0f;
	static float OffsetModel;
	static float Delta;

	// On récupère la position de la caméra
	PositionCamera = m_pScene->GetCameraCourante()->GetPosition();
	
	// Mise à jour des informations des zones
	for(int j = 0 ; j < NB_MAX_MODEL_R ; j++)
	{
		// Calcul de la matrice de trasformation du model
		OffsetModel = float(ProfondeurModel * j + (int(PositionCamera.z) / ProfondeurModel) * ProfondeurModel) + OffsetZone;
		D3DXMatrixTranslation(&m_Matrice[j], 0.0f, 0.0f, OffsetModel);
		for(int i = 0 ; i < NB_ZONE_R ; i++)
		{
			// Calcul de la position de la zone pour le modèl de droite
			PositionZone = m_InfoZoneD[j][i].p;
			PositionZone.z += OffsetModel;
			// On test si la zone est visible
			m_InfoZoneD[j][i].Visible = m_pScene->GetCameraCourante()->IsVisible(&PositionZone, RAYON_ZONE_R);
			// Calcul de la distance entre la zone et la camera
			DeltaPosition = PositionZone - PositionCamera;
			Delta = D3DXVec3Length(&DeltaPosition);
			// Calcul du niveau de détail en fonction de la distance
			if(Delta > m_OffsetLOD) m_InfoZoneD[j][i].LOD = LOD_MAX_R;
			else m_InfoZoneD[j][i].LOD = int(-((Delta-m_OffsetLOD)/m_AmplitudeLOD)*((Delta-m_OffsetLOD)/m_AmplitudeLOD)+m_MaxLOD);
			// Calcul de la position de la zone pour le modèl de gauche
			PositionZone = m_InfoZoneG[j][i].p;
			PositionZone.z += OffsetModel;
			// On test si la zone est visible
			m_InfoZoneG[j][i].Visible = m_pScene->GetCameraCourante()->IsVisible(&PositionZone, RAYON_ZONE_R);
			// Calcul de la distance entre la zone et la camera
			DeltaPosition = PositionZone - PositionCamera;
			Delta = D3DXVec3Length(&DeltaPosition);
			// Calcul du niveau de détail en fonction de la distance
			if(Delta > m_OffsetLOD) m_InfoZoneG[j][i].LOD = LOD_MAX_R;
			else m_InfoZoneG[j][i].LOD = int(-((Delta-m_OffsetLOD)/m_AmplitudeLOD)*((Delta-m_OffsetLOD)/m_AmplitudeLOD)+m_MaxLOD);
		}
	}
}

// Rendu de la roche
void CModelRoche::RenduRoche()
{
	static int LOD; // niveau LOD
	// Activation de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Initialisation du matériel
	m_pD3DDevice->SetMaterial(m_Materiel.GetMaterielle());
	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTexture(0, m_pTexRoche);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	// Initialisation des buffers
	m_pD3DDevice->SetFVF(D3DFVF_VERTEXROCHE);
	m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEXROCHE));
	m_pD3DDevice->SetIndices(m_pIB);
	// Rendu de chaque zones
	for(int j = 0 ; j < NB_MAX_MODEL_R ; j++)
	{
		// Initialisation de la matrice de transformation
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[j]);
		for(int i = 0 ; i < NB_ZONE_R ; i++)
		{
			// Rendu zone de droite
			if(m_InfoZoneD[j][i].Visible)
			{
				LOD = m_InfoZoneD[j][i].LOD;
				m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
												m_InfoZoneD[j][i].OffsetVertex, 0, 
												m_InfoLODD[LOD].NbVertex, 
												m_InfoLODD[LOD].OffsetIndice, 
												m_InfoLODD[LOD].NbTriangle);
			}
			// Rendu zone de gauche
			if(m_InfoZoneG[j][i].Visible)
			{
				LOD = m_InfoZoneG[j][i].LOD;
				m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
												m_InfoZoneG[j][i].OffsetVertex, 0, 
												m_InfoLODG[LOD].NbVertex, 
												m_InfoLODG[LOD].OffsetIndice, 
												m_InfoLODG[LOD].NbTriangle);
			}
		}
	}
}

// Création du maillage de la roche
void CModelRoche::CreationMaillage(VERTEXROCHE Donnee[NB_ZONE_R][NB_VERTEX_R][NB_VERTEX_R], bool bSens, char* Fichier)
{
	CImageRAW Image;
	// On charge la map de déformation (heighmap)
	Image.Charger(Fichier, NB_PIXEL_MAP_X_R, NB_PIXEL_MAP_Y_R, 1, 16);
	// On récupère un pointeur sur les données
	unsigned short* DonneeImage = (unsigned short*)Image.GetDonnees();

	// Tableau pour calculer l'amplitude de la déformation
	float DonneeBrute[NB_VERTEX_R+2][NB_VERTEX_R+2];
	float OffsetX;
	float CoefHauteur;

	// true si modèl de droite, false si modèl de gauche
	if(bSens)
	{
		CoefHauteur = -COEF_HAUTEUR_R;
		OffsetX = POSITION_MD_R;
	}
	else
	{
		CoefHauteur =  COEF_HAUTEUR_R;
		OffsetX = POSITION_MG_R;
	}

	D3DXVECTOR3 v1, v2, v3, v4, v5, v6, v;

	// On parcours chaque zone
	for(int c = 0; c < NB_ZONE_R; c++)
	{
		// Calcul de l'amplitude de déformation
		for(int iy = 0; iy < NB_VERTEX_R+2; iy++)
			for(int ix = 0; ix < NB_VERTEX_R+2; ix++)
				DonneeBrute[iy][ix] = (float)DonneeImage[NB_PIXEL_MAP_X_R * (iy + NB_TRIANGLE_R * c) + ix] / CoefHauteur;

		// Calcul des vertex d'une zone
		for(int z = 0; z < NB_VERTEX_R; z++)
		{
			for(int y = 0; y < NB_VERTEX_R; y++)
			{
				Donnee[c][z][y].n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				// Calcul des vertex adjacent au vertex
				v1 = D3DXVECTOR3(DonneeBrute[z+2][y+1] - DonneeBrute[z+1][y+1], 0.0f,  1.0f);
				v2 = D3DXVECTOR3(DonneeBrute[z+2][y+2] - DonneeBrute[z+1][y+1], 1.0f,  1.0f);
				v3 = D3DXVECTOR3(DonneeBrute[z+1][y+2] - DonneeBrute[z+1][y+1], 1.0f,  0.0f);
				v4 = D3DXVECTOR3(DonneeBrute[  z][y+1] - DonneeBrute[z+1][y+1], 0.0f, -1.0f);
				v5 = D3DXVECTOR3(DonneeBrute[  z][  y] - DonneeBrute[z+1][y+1],-1.0f, -1.0f);
				v6 = D3DXVECTOR3(DonneeBrute[z+1][  y] - DonneeBrute[z+1][y+1],-1.0f,  0.0f);
				// Calcul de la normale
				D3DXVec3Cross(&v, &v1, &v2);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				D3DXVec3Cross(&v, &v2, &v3);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				D3DXVec3Cross(&v, &v3, &v4);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				D3DXVec3Cross(&v, &v4, &v5);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				D3DXVec3Cross(&v, &v5, &v6);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				D3DXVec3Cross(&v, &v6, &v1);
				D3DXVec3Normalize(&v, &v);
				D3DXVec3Add(&Donnee[c][z][y].n, &Donnee[c][z][y].n, &v);
				// Normalisation de la normale
				D3DXVec3Normalize(&Donnee[c][z][y].n, &Donnee[c][z][y].n);
				// Si on est dans en sens inverse, on inverse la normale
				if(!bSens) Donnee[c][z][y].n = -Donnee[c][z][y].n;
				// Corrdonnées de texture
				Donnee[c][z][y].t = D3DXVECTOR2(float(z) / COEF_TEXTURE_R, -float(y) / COEF_TEXTURE_R - 0.4f);
				// Position du vertex
				Donnee[c][z][y].p = D3DXVECTOR3(DonneeBrute[z+1][y+1]+OffsetX, float(y), float(z + NB_TRIANGLE_R * c));
			}
		}
	}
}

// Calcul les paramètres du LOD
void CModelRoche::SetCoefLOD(float Offset, float Min)
{
	m_OffsetLOD = Offset;
	m_MaxLOD = LOD_MAX_R - 1 + 0.1f;
	m_AmplitudeLOD = m_OffsetLOD / sqrt(m_MaxLOD-Min);
}