#include "ModelVegetation.h"
#include "..\Outils\sobol.h"

// Constructeur
CModelVegetation::CModelVegetation(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTxArbre1		= NULL;
	m_pTxArbre2		= NULL;
	m_pTxArbre3		= NULL;
	m_NbArbreTotal  = 0;
	m_NbIndiceTotal = 0;
	m_NbVertexTotal = 0;
}

// Déconstructeur
CModelVegetation::~CModelVegetation()
{	
	delete Donnee;
}

// Création de la végétation
bool CModelVegetation::CreationVegetation()
{
	CImageTGA MapArbre;
	// On charge la map de densité des arbres
	if(!MapArbre.Charger(FichierMapArbre))return false;
	// Variable pour le générateur de nombre quasi aléatoire
	long double Tab[3];
	int Dim = 2;
	// Initialisation du générateur pseudo aléatoire
	srand(0);
	// Initialisation du générateur quasi aléatoire
	sobseq(&seedSOB, Tab);
	// Tableau pour contenir les position valide des arbres généré
	D3DXVECTOR3* TabPosition = new D3DXVECTOR3[NB_ARBRE_MAX];
	
	float x, z;
	float Largeur = (float)MapArbre.GetLargeur();
	float Profondeur = (float)MapArbre.GetHauteur();
	float ProfondeurZone = Profondeur / (float)NB_ZONE_VEGETATION;
	float Amplitude = 0.0f;
	DWORD Pixel;
	
	// Génération des position des arbres
	for(int i = 0 ; i < NB_ARBRE_MAX ; i++)
	{
		// Tirage d'une position
		sobseq(&Dim, Tab);
		x = (float)Tab[1]*Largeur;
		z = (float)Tab[2]*Profondeur;
		Pixel = MapArbre.GetPixel(int(x), int(z));
		// On vérifie si la position est valide
		if(Pixel > REF_ZERO)
		{
			// On sauvegarde la position dans le tableau
			TabPosition[m_NbArbreTotal] = D3DXVECTOR3(x, Pixel / COEF_HAUTEUR_VEGETATION + POSITION_Y, z);
			m_NbArbreTotal++;
		}
	}

	int Compteur = 0;
	int NbArbreZone[NB_ZONE_VEGETATION]; // Nombre d'arbre par zone
	Donnee = new CUSTOMVERTEX[m_NbArbreTotal*NB_VERTEX_VEGETATION];

	// Création des vertex pour chaque arbres
	for(int j = 0; j < NB_ZONE_VEGETATION; j++)
	{
		NbArbreZone[j] = 0;
		for(int i = 0; i < m_NbArbreTotal; i++)
		{
			if(int(TabPosition[i].z / ProfondeurZone) == j)
			{
				// Variation de 50% de la taille d'un arbre
				Amplitude = (1.0f + (rand() / (float)RAND_MAX)*0.5f) * TAILLE_VEGETATION;
				// Premier quad de l'arbre
				Donnee[Compteur].p = D3DXVECTOR3(-Amplitude+TabPosition[i].x, TabPosition[i].y+2.0f*Amplitude, -Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(0.0f, 0.0f);
				Donnee[Compteur].p = D3DXVECTOR3( Amplitude+TabPosition[i].x, TabPosition[i].y+2.0f*Amplitude,  Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(1.0f, 0.0f);
				Donnee[Compteur].p = D3DXVECTOR3( Amplitude+TabPosition[i].x, TabPosition[i].y               ,  Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(1.0f, 1.0f);
				Donnee[Compteur].p = D3DXVECTOR3(-Amplitude+TabPosition[i].x, TabPosition[i].y               , -Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(0.0f, 1.0f);
				// Deuxième quad de l'arbre
				Donnee[Compteur].p = D3DXVECTOR3(-Amplitude+TabPosition[i].x, TabPosition[i].y+2.0f*Amplitude,  Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(0.0f, 0.0f);
				Donnee[Compteur].p = D3DXVECTOR3( Amplitude+TabPosition[i].x, TabPosition[i].y+2.0f*Amplitude, -Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(1.0f, 0.0f);
				Donnee[Compteur].p = D3DXVECTOR3( Amplitude+TabPosition[i].x, TabPosition[i].y               , -Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(1.0f, 1.0f);
				Donnee[Compteur].p = D3DXVECTOR3(-Amplitude+TabPosition[i].x, TabPosition[i].y               ,  Amplitude+TabPosition[i].z);
				Donnee[Compteur++].t = D3DXVECTOR2(0.0f, 1.0f);
				NbArbreZone[j]++; 
			}
		}
	}

	int OffsetVertex = 0;
	// Création des informations sur chaque zones
	for(int j = 0; j < NB_MODEL_VEGETATION; j++)
	{
		OffsetVertex = 0;
		for(int i = 0; i < NB_ZONE_VEGETATION; i++)
		{
			// Position de la zone
			m_InfoZone[j][i].p = D3DXVECTOR3(Largeur/2.0f, POSITION_Y+TAILLE_VEGETATION, float(ProfondeurZone*i + ProfondeurZone/2));
			// Information sur les arbres de type 1
			m_InfoZone[j][i].NbTriangle1 = (NbArbreZone[i]/3)*NB_TRIANGLE_VEGETATION;
			m_InfoZone[j][i].NbVertex1   = (NbArbreZone[i]/3)*NB_VERTEX_VEGETATION;
			m_InfoZone[j][i].Offset1     = OffsetVertex;
			OffsetVertex += m_InfoZone[j][i].NbVertex1;
			// Information sur les arbres de type 2
			m_InfoZone[j][i].NbTriangle2 = (NbArbreZone[i]/3)*NB_TRIANGLE_VEGETATION;
			m_InfoZone[j][i].NbVertex2   = (NbArbreZone[i]/3)*NB_VERTEX_VEGETATION;
			m_InfoZone[j][i].Offset2     = OffsetVertex;
			OffsetVertex += m_InfoZone[j][i].NbVertex2;
			// Information sur les arbres de type 3
			m_InfoZone[j][i].NbTriangle3 = (NbArbreZone[i]-2*NbArbreZone[i]/3)*NB_TRIANGLE_VEGETATION;
			m_InfoZone[j][i].NbVertex3   = (NbArbreZone[i]-2*NbArbreZone[i]/3)*NB_VERTEX_VEGETATION;
			m_InfoZone[j][i].Offset3     = OffsetVertex;
			OffsetVertex += m_InfoZone[j][i].NbVertex3;
		}
	}

	m_NbVertexTotal = m_NbArbreTotal*NB_VERTEX_VEGETATION;
	m_NbIndiceTotal = m_NbArbreTotal*NB_INDICE_VEGETATION;

	delete TabPosition;

	return true;
}

// Charge les données dans la mémoire vidéo
bool CModelVegetation::ChargerVegetation()
{
	VOID* pVerticesVideo = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(m_NbVertexTotal*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// Copie des vertex du modèl dans la mémoire video
	m_pVB->Lock(0, m_NbVertexTotal*sizeof(CUSTOMVERTEX), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, Donnee, m_NbVertexTotal*sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();
	// Création d'un tableau pour stocker les indices
	WORD* pIndices = new WORD[m_NbIndiceTotal];
	WORD* pt = pIndices;

	// On va remplire le tableau d'indice pour former une liste de triangle de type D3DPT_TRIANGLELIST
	for(int i = 0 ; i < m_NbArbreTotal ; i++)
	{
		*pt++ = i * NB_VERTEX_VEGETATION + 0;
		*pt++ = i * NB_VERTEX_VEGETATION + 1;
		*pt++ = i * NB_VERTEX_VEGETATION + 2;
		*pt++ = i * NB_VERTEX_VEGETATION + 2;
		*pt++ = i * NB_VERTEX_VEGETATION + 3;
		*pt++ = i * NB_VERTEX_VEGETATION + 0;

		*pt++ = i * NB_VERTEX_VEGETATION + 4;
		*pt++ = i * NB_VERTEX_VEGETATION + 5;
		*pt++ = i * NB_VERTEX_VEGETATION + 6;
		*pt++ = i * NB_VERTEX_VEGETATION + 6;
		*pt++ = i * NB_VERTEX_VEGETATION + 7;
		*pt++ = i * NB_VERTEX_VEGETATION + 4;
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
	D3DXCreateTextureFromFile(m_pD3DDevice, FichierTxArbre1, &m_pTxArbre1);
	D3DXCreateTextureFromFile(m_pD3DDevice, FichierTxArbre2, &m_pTxArbre2);
	D3DXCreateTextureFromFile(m_pD3DDevice, FichierTxArbre3, &m_pTxArbre3);
	return true;
}

// Libération des ressources
void CModelVegetation::Release()
{
	SAFE_RELEASE(m_pTxArbre1);
	SAFE_RELEASE(m_pTxArbre2);
	SAFE_RELEASE(m_pTxArbre3);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

// Mise à jour des informations des zones
void CModelVegetation::MiseAJourDonnees()
{
	static D3DXVECTOR3 PositionCamera;
	static D3DXVECTOR3 PositionZone;
	static D3DXVECTOR3 DeltaPosition;

	static float OffsetZone = -float(LONGEUR_VEGETATION * NB_MODEL_VEGETATION)/2.0f;
	static float OffsetModel;

	// On récupère la position de la caméra
	PositionCamera = m_pScene->GetCameraCourante()->GetPosition();

	// Mise à jour des informations des zones
	for(int j = 0 ; j < NB_MODEL_VEGETATION ; j++)
	{
		// Calcul de la matrice de trasformation du model
		OffsetModel = float(LONGEUR_VEGETATION * j + (int(PositionCamera.z) / int(LONGEUR_VEGETATION)) * LONGEUR_VEGETATION) + OffsetZone;
		D3DXMatrixTranslation(&m_Matrice[j], 0.0f, 0.0f, OffsetModel);
		for(int i = 0 ; i < NB_ZONE_VEGETATION ; i++)
		{
			// Calcul de la position de la zone pour le modèl
			PositionZone = m_InfoZone[j][i].p;
			PositionZone.z += OffsetModel;
			// On test si la zone est visible
			m_InfoZone[j][i].Visible = m_pScene->GetCameraCourante()->IsVisible(&PositionZone, RAYON_ZONE_VEGETATION);
		}
	}
}

// Rendu de la roche
void CModelVegetation::RenduVegetation()
{
	// Déactivation de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// On déactive le culling pour voir les arbres des deux cotés
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// On initialise le type de test sur l'alpha
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// Seuil de transparencre de la texture
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000090);
	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	// Initialisation des buffers
	m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pD3DDevice->SetIndices(m_pIB);
	// Rendu des arbres de type 1
	m_pD3DDevice->SetTexture(0, m_pTxArbre1);
	for(int j = 0 ; j < NB_MODEL_VEGETATION ; j++)
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[j]);
		for(int i = 0 ; i < NB_ZONE_VEGETATION ; i++)
		{
			if(m_InfoZone[j][i].Visible)
			{
				m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
												m_InfoZone[j][i].Offset1, 
												0, 
												m_InfoZone[j][i].NbVertex1, 
												0, 
												m_InfoZone[j][i].NbTriangle1);
			}
		}
	}
	// Rendu des arbres de type 1
	m_pD3DDevice->SetTexture(0, m_pTxArbre2);
	for(int j = 0 ; j < NB_MODEL_VEGETATION ; j++)
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[j]);
		for(int i = 0 ; i < NB_ZONE_VEGETATION ; i++)
		{
			if(m_InfoZone[j][i].Visible)
			{
				m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
												m_InfoZone[j][i].Offset2,
												0, 
												m_InfoZone[j][i].NbVertex2, 
												0, 
												m_InfoZone[j][i].NbTriangle2);
			}
		}
	}
	// Rendu des arbres de type 1
	m_pD3DDevice->SetTexture(0, m_pTxArbre3);
	for(int j = 0 ; j < NB_MODEL_VEGETATION ; j++)
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[j]);
		for(int i = 0 ; i < NB_ZONE_VEGETATION ; i++)
		{
			if(m_InfoZone[j][i].Visible)
			{
				m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
												m_InfoZone[j][i].Offset3, 
												0, 
												m_InfoZone[j][i].NbVertex2, 
												0, 
												m_InfoZone[j][i].NbTriangle3);
			}
		}
	}
	// On remet les paramètres dans l'état initiale
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}