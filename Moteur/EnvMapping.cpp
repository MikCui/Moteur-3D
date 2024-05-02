#include "EnvMapping.h"

CEnvMapping::CEnvMapping(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pCubeMap = NULL;
	m_DepthCube = NULL;
	m_pSphereMap = NULL;
	m_DepthSphere = NULL;
	m_pMesh = NULL;
	D3DXMatrixIdentity(&m_MatriceIdentite);

	m_pCameraCubeMapping = new CCamera(pScene);
	m_pCameraCubeMapping->Creation();
	m_pCameraSphereMapping = new CCamera(pScene);
	m_pCameraSphereMapping->Creation(&D3DXVECTOR3(1.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}

CEnvMapping::~CEnvMapping()
{
	delete m_pCameraCubeMapping;
	delete m_pCameraSphereMapping;
}

// Création des textures et z-buffer pour le rendu
bool CEnvMapping::Creation()
{
	m_pD3DDevice->CreateCubeTexture(CUBEMAP_RESOLUTION,
                                    1,
                                    D3DUSAGE_RENDERTARGET,
                                    m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferFormat,
                                    D3DPOOL_DEFAULT,
                                    &m_pCubeMap,
                                    NULL);

	m_pD3DDevice->CreateDepthStencilSurface(CUBEMAP_RESOLUTION,
                                            CUBEMAP_RESOLUTION,
                                            m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.AutoDepthStencilFormat,
                                            D3DMULTISAMPLE_NONE,
                                            0,
                                            TRUE,
                                            &m_DepthCube,
                                            NULL);

	m_pD3DDevice->CreateTexture(SPHEREMAP_RESOLUTION,
		                        SPHEREMAP_RESOLUTION,
                                1,
                                D3DUSAGE_RENDERTARGET,
                                m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.BackBufferFormat,
                                D3DPOOL_DEFAULT,
                                &m_pSphereMap,
                                NULL);

	m_pD3DDevice->CreateDepthStencilSurface(SPHEREMAP_RESOLUTION,
                                            SPHEREMAP_RESOLUTION,
                                            m_pScene->m_pMoteur->m_pMoteur3D->m_d3dpp.AutoDepthStencilFormat,
                                            D3DMULTISAMPLE_NONE,
                                            0,
                                            TRUE,
                                            &m_DepthSphere,
                                            NULL);

	D3DXCreateSphere(m_pD3DDevice, 0.5f, 64, 32, &m_pMesh, NULL);

	return true;
}

// Libération des ressources utilisé
void CEnvMapping::Release()
{
	SAFE_RELEASE(m_pCubeMap);
	SAFE_RELEASE(m_DepthCube);
	SAFE_RELEASE(m_pSphereMap);
	SAFE_RELEASE(m_DepthSphere);
	SAFE_RELEASE(m_pMesh);
}

// Centre du rendu de l'environnement dans la scène 
void CEnvMapping::SetPostion(D3DXVECTOR3* Position)
{
	m_pCameraCubeMapping->SetPosition(Position);
}

// Rendu de la texture d'environnement cubique
void CEnvMapping::RenduCubeMapping()
{
	// Sauvegarde de la caméra de la scène
	CCamera* pCameraOld = m_pScene->GetCameraCourante();
	// Sauvegarde des surfaces de rendu
	LPDIRECT3DSURFACE9 pRTOld = NULL;
    m_pD3DDevice->GetRenderTarget(0, &pRTOld);
    LPDIRECT3DSURFACE9 pDSOld = NULL;
    m_pD3DDevice->GetDepthStencilSurface(&pDSOld);
 	// On active la caméra pour le rendu du cube mapping
	m_pScene->SetCamera(m_pCameraCubeMapping);
	// Activation du z-buffer du cube mapping
	m_pD3DDevice->SetDepthStencilSurface(m_DepthCube);
	// Rendu des 6 faces du cube
	for(int nFace = 0; nFace < 6; ++nFace)
    {
        LPDIRECT3DSURFACE9 pSurf;
		// On récupère la surface de la face
        m_pCubeMap->GetCubeMapSurface((D3DCUBEMAP_FACES)nFace, 0, &pSurf);
        m_pD3DDevice->SetRenderTarget(0, pSurf);
        SAFE_RELEASE(pSurf);
		// On applique les matrices de transformation de la caméra
		m_pCameraCubeMapping->SetMatriceCubeMap(nFace);
		// Rendu de la face du cube
		m_pD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        m_pD3DDevice->BeginScene();
		m_pScene->Rendu3D();
        m_pD3DDevice->EndScene();
    }
	// Restoration de la caméra de la scène
	m_pScene->SetCamera(pCameraOld);
	// Restoration des surfaces de rendu
	if(pDSOld)
    {
        m_pD3DDevice->SetDepthStencilSurface(pDSOld);
        SAFE_RELEASE(pDSOld);
    }
    m_pD3DDevice->SetRenderTarget(0, pRTOld);
    SAFE_RELEASE(pRTOld);
}

// Rendu de la texture d'environnement sphérique
void CEnvMapping::RenduSphereMapping()
{
	// Sauvegarde de la caméra de la scène
	CCamera* pCameraOld = m_pScene->GetCameraCourante();
	// Sauvegarde des surfaces de rendu
	LPDIRECT3DSURFACE9 pRTOld = NULL;
    m_pD3DDevice->GetRenderTarget(0, &pRTOld);
    LPDIRECT3DSURFACE9 pDSOld = NULL;
    m_pD3DDevice->GetDepthStencilSurface(&pDSOld);
 	// Calcule de la position de la camera
	D3DXVECTOR3 vDelta = (pCameraOld->GetPosition() - pCameraOld->GetCible());
	D3DXVec3Normalize(&vDelta, &vDelta);
	vDelta = vDelta * 12.7f;
	// Position de la camera pour le rendu du sphère mapping
	m_pCameraSphereMapping->SetPosition(&vDelta);
	// On applique les matrices de transformation de la caméra
	m_pCameraSphereMapping->SetMatriceSphereMap();
	// Activation du z-buffer du sphère mapping
	m_pD3DDevice->SetDepthStencilSurface(m_DepthSphere);
	// On récupère la surface de la texture
	LPDIRECT3DSURFACE9 pSurf;
	m_pSphereMap->GetSurfaceLevel(0, &pSurf);
    m_pD3DDevice->SetRenderTarget(0, pSurf);
    SAFE_RELEASE(pSurf);
	// Rendu de la texture
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
    m_pD3DDevice->BeginScene();
	// Modulation entre la réflection de l'environnement et la lumière
	m_pD3DDevice->SetTexture(0, m_pCubeMap);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &pCameraOld->GetMatriceTextureCube());
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	// On place la sphère pour le rendu au centre du repère
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceIdentite);
	// Rendu de la sphère
	m_pMesh->DrawSubset(0);
    m_pD3DDevice->EndScene();
	// réinitialisation des paramètres de texture
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	// Restoration de la caméra de la scène
	m_pScene->SetCamera(pCameraOld);
	// Restoration des surfaces de rendu
	if(pDSOld)
    {
        m_pD3DDevice->SetDepthStencilSurface(pDSOld);
        SAFE_RELEASE(pDSOld);
    }
    m_pD3DDevice->SetRenderTarget(0, pRTOld);
    SAFE_RELEASE(pRTOld);
}