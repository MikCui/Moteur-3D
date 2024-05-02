#include "Skybox.h"

CSkyBox::CSkyBox(CScene* pScene):CModel(pScene)
{
	D3DXMatrixIdentity(&m_MatriceWorld);
}

CSkyBox::~CSkyBox()
{
}

// Rendu de la SkyBox
void CSkyBox::RenduModel()
{
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceWorld);
	// On récupère la matrice de vision pour placer la camera au centre de la SkyBox
	m_MatriceView = m_pScene->GetCameraCourante()->GetMatriceVision();
	m_MatriceView._41 = m_MatriceView._42 = m_MatriceView._43 = 0.0f;
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceView);
	// On desactive le ZBuffer
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// On active D3DTADDRESS_CLAMP pour une bonne jointure des textures de la SkyBox
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP);
	// On initialise Stage de rendu
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	// On parcours chaque partie de la SkyBox
	for(DWORD i=0; i<m_NumMaterielles; i++)
	{
		// On active la texture de la partie à dessiner
        m_pD3DDevice->SetTexture(0, m_pMeshTextures[i]);
        // Dessin de la partie de la SkyBox
        m_pMesh->DrawSubset(i);
	}
	// On remet la matrice de vision dans son état initial 
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_pScene->GetCameraCourante()->GetMatriceVision());
	// On réactive le ZBuffer
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);
}