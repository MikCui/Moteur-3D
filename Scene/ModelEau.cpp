#include "ModelEau.h"

inline DWORD F2DW(float f) {return *((DWORD *)&f);}

// Constructeur
CModelEau::CModelEau(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexBump = NULL;
	m_RenduMiroir = false;
	m_pEnvMiroir = new CEnvMiroir(pScene);
	D3DXMatrixIdentity(&m_MatTransEau);
}

// Déconstructeur
CModelEau::~CModelEau()
{	
	delete m_pEnvMiroir;
}

// Création de l'eau
bool CModelEau::CreationEau()
{
	// Vertex pour une zone
	Donnee[0].p = D3DXVECTOR3(POSITION_EAU_X - LARGEUR/2.0f, POSITION_EAU_Y, LONGEUR);
	Donnee[0].t = D3DXVECTOR2(0.0f, 0.0f);
	Donnee[1].p = D3DXVECTOR3(POSITION_EAU_X + LARGEUR/2.0f, POSITION_EAU_Y, LONGEUR);
	Donnee[1].t = D3DXVECTOR2(1.0f*COEF_TEXTURE_X, 0.0f);
	Donnee[2].p = D3DXVECTOR3(POSITION_EAU_X + LARGEUR/2.0f, POSITION_EAU_Y, 0.0f);
	Donnee[2].t = D3DXVECTOR2(1.0f*COEF_TEXTURE_X, 1.0f*COEF_TEXTURE_Y);
	Donnee[3].p = D3DXVECTOR3(POSITION_EAU_X - LARGEUR/2.0f, POSITION_EAU_Y, 0.0f);
	Donnee[3].t = D3DXVECTOR2(0.0f, 1.0f*COEF_TEXTURE_Y);

	// On crée un plan par rapport au point de la zone
	D3DXPlaneFromPoints(&m_Plan, &Donnee[0].p, &Donnee[1].p, &Donnee[2].p);
	// Création d'un matrice de réflexion
	D3DXMatrixReflect(&m_MatriceReflection, &m_Plan);
	return true;
}

// Charge les données dans la mémoire vidéo
bool CModelEau::ChargerEau()
{
	VOID* pVerticesVideo = NULL;
	// Création du vertex buffer
	m_pD3DDevice->CreateVertexBuffer(NB_VERTEX_EAU*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);
	// Copie des vertex du modèl dans la mémoire video
	m_pVB->Lock(0, NB_VERTEX_EAU*sizeof(CUSTOMVERTEX), (void**)&pVerticesVideo, 0);
	memcpy(pVerticesVideo, Donnee, NB_VERTEX_EAU*sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();
	// Création d'un tableau pour stocker les indices
	WORD* pIndices = new WORD[NB_INDICE_EAU];
	WORD* pt = pIndices;

	// On va remplire le tableau d'indice pour former une liste de triangle de type D3DPT_TRIANGLELIST
	*pt++ = 0;
	*pt++ = 1;
	*pt++ = 2;
	*pt++ = 2;
	*pt++ = 3;
	*pt++ = 0;

	VOID* pIndicesVideo = NULL;
	// Création de l'index buffer
	m_pD3DDevice->CreateIndexBuffer(NB_INDICE_EAU*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
	m_pIB->Lock(0, NB_INDICE_EAU*sizeof(WORD), (void**)&pIndicesVideo, 0);
	// Copie des indices dans la mémoire video
	memcpy(pIndicesVideo, pIndices, NB_INDICE_EAU*sizeof(WORD));
	m_pIB->Unlock();

	delete[] pIndices;

	// Création de l'objet Miroir 
	m_pEnvMiroir->Creation(&m_Plan);
	// Chargement de la texture
	D3DXCreateTextureFromFileEx(m_pD3DDevice, FcihierTxBump, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &m_pTexBump);
	// Création d'un plan pour le clipping
	D3DXPLANE PlanClipping;
	D3DXPlaneFromPoints(&PlanClipping, &D3DXVECTOR3(0.0f, POSITION_EAU_Y-0.5f, 0.0f), &D3DXVECTOR3(0.0f, POSITION_EAU_Y-0.5f, 1.0f), &D3DXVECTOR3(1.0f, POSITION_EAU_Y-0.5f, 1.0f));
	m_pD3DDevice->SetClipPlane(0, PlanClipping);
	return true;
}

// Libération des ressources
void CModelEau::Release()
{
	SAFE_RELEASE(m_pTexBump);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	m_pEnvMiroir->Release();
}

// Mise à jour des informations des zones
void CModelEau::MiseAJourDonnees(double TempsEcouler)
{
	static float OffsetZone = -float(LONGEUR * NB_ZONE_EAU)/2.0f;
	static D3DXVECTOR3 PositionCamera;
	static float OffsetModel;

	// On récupère la position de la caméra
	PositionCamera = m_pScene->GetCameraCourante()->GetPosition();

	// Mise à jour des informations des zones
	for(int i = 0 ; i < NB_ZONE_EAU ; i++)
	{
		// Calcul de la matrice de trasformation du model
		OffsetModel = float(LONGEUR * i + (int(PositionCamera.z) / int(LONGEUR)) * LONGEUR) + OffsetZone;
		D3DXMatrixTranslation(&m_Matrice[i], 0.0f, 0.0f, OffsetModel);
	}

	// Calcul de la matrice de transformation de l'eau
	m_MatTransEau._32 = (float)TempsEcouler / VITESSE_EAU;
}

// Rendu de la texture de réflexion
void CModelEau::RenduEnvMiroir()
{
	// On active le clliping
	m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	m_RenduMiroir = true;
	m_pEnvMiroir->RenduMiroir();
	m_RenduMiroir = false;
	// On déactive le clipping
	m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
}

// Rendu de la roche
void CModelEau::RenduEau()
{
	// Déactivation de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// On n'affiche pas lae sol pendant le rendu du miroir
	if(m_RenduMiroir == false)
	{
		// Activation de la transparence
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		// Niveau de transparence de la texture
		m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0x90000000);
		// Etage 0 : activation des paramètres du bump mapping
		m_pD3DDevice->SetTexture(0, m_pTexBump);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVMAT00, F2DW(0.05f));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVMAT11, F2DW(0.05f));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVLSCALE, F2DW(1.0f));
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f));
		m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &m_MatTransEau);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		// Etage 1 : affichage de la texture de réflexion
		m_pD3DDevice->SetTexture(1, m_pEnvMiroir->GetMiroir());
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		m_pD3DDevice->SetTransform(D3DTS_TEXTURE1, &m_pScene->GetCameraCourante()->GetMatriceProjectionTexture());
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED |D3DTTFF_COUNT3);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP);
		// Etage 2 : fin des opérations
		m_pD3DDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		m_pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
		// Initialisation des buffers
		m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
		m_pD3DDevice->SetIndices(m_pIB);
		// Rendu de l'eau
		for(int i = 0 ; i < NB_ZONE_EAU ; i++)
		{
			m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_Matrice[i]);
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NB_VERTEX_EAU, 0, NB_TRIANGLE_EAU);
		}
		// On remet les paramètres dans l'état initiale
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}