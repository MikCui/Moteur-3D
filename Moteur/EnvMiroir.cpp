#include "EnvMiroir.h"

CEnvMiroir::CEnvMiroir(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pMiroir = NULL;
	m_DepthMiroir = NULL;
}

CEnvMiroir::~CEnvMiroir()
{
}

// Création de la texture et z-buffer pour le rendu
bool CEnvMiroir::Creation(D3DXPLANE* Plan)
{
	m_pD3DDevice->CreateTexture(m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferWidth/2,
		                        m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferHeight/2,
                                1,
                                D3DUSAGE_RENDERTARGET,
                                m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferFormat,
                                D3DPOOL_DEFAULT,
                                &m_pMiroir,
                                NULL);

	m_pD3DDevice->CreateDepthStencilSurface(m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferWidth/2,
                                            m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferHeight/2,
                                            m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.AutoDepthStencilFormat,
                                            D3DMULTISAMPLE_NONE,
                                            0,
                                            TRUE,
                                            &m_DepthMiroir,
                                            NULL);

	// Matrice de réflection par rapport au plan du miroir
	D3DXMatrixReflect(&m_MatriceReflection, Plan);

	return true;
}

// Libération des ressources utilisé
void CEnvMiroir::Release()
{
	SAFE_RELEASE(m_pMiroir);
	SAFE_RELEASE(m_DepthMiroir);
}

// Rendu de la texture du miroir
void CEnvMiroir::RenduMiroir()
{
	// On inverse la matrice de vue de la caméra
	m_pScene->GetCameraCourante()->SetMatriceMiroir(&m_MatriceReflection);
	// On inverse le culling
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	// Sauvegarde des surfaces de rendu
	LPDIRECT3DSURFACE9 pRTOld = NULL;
    m_pD3DDevice->GetRenderTarget(0, &pRTOld);
    LPDIRECT3DSURFACE9 pDSOld = NULL;
    m_pD3DDevice->GetDepthStencilSurface(&pDSOld);
 	// Activation du z-buffer du miroir
	m_pD3DDevice->SetDepthStencilSurface(m_DepthMiroir);
	// On récupère la surface de la texture
	LPDIRECT3DSURFACE9 pSurf;
	m_pMiroir->GetSurfaceLevel(0, &pSurf);
    m_pD3DDevice->SetRenderTarget(0, pSurf);
    SAFE_RELEASE(pSurf);
	// Rendu de la texture
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    m_pD3DDevice->BeginScene();
	m_pScene->Rendu3D();
    m_pD3DDevice->EndScene();
	// On remet la matrice de vue de la caméra
	m_pScene->GetCameraCourante()->SetMatrice();
	// On restore le culling
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Restoration des surfaces de rendu
	if(pDSOld)
    {
        m_pD3DDevice->SetDepthStencilSurface(pDSOld);
        SAFE_RELEASE(pDSOld);
    }
    m_pD3DDevice->SetRenderTarget(0, pRTOld);
    SAFE_RELEASE(pRTOld);
}