#include "SceneConfig.h"

CSceneConfig::CSceneConfig(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pModelRoche = NULL;
	m_pModelSol = NULL;
	m_pModelEau = NULL;
	m_pModelVegetation = NULL;
	m_pSkyBox = NULL;

	m_CoefLODSol[0] = 2.0f;
	m_CoefLODSol[1] = 1.0f;
	m_CoefLODSol[2] = 0.0f;

	m_CoefLODSolR[0] = 3.0f;
	m_CoefLODSolR[1] = 2.0f;
	m_CoefLODSolR[2] = 1.0f;

	m_CoefLODRoche[0] = 2.0f;
	m_CoefLODRoche[1] = 1.0f;
	m_CoefLODRoche[2] = 0.0f;

	m_CoefLODRocheR[0] = 3.0f;
	m_CoefLODRocheR[1] = 2.0f;
	m_CoefLODRocheR[2] = 1.0f;

	m_QualiterFiltrage = 1;
	m_QualiterModel = 1;
}

CSceneConfig::~CSceneConfig()
{
	Console << "Deconstructeur CSceneConfig" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneConfig::ConfirmeFonction3D(D3DCAPS9* pCaps)
{
	if(!(pCaps->TextureCaps & D3DPTEXTURECAPS_PROJECTED))
	{
		Console << "Le peripherique ne permet pas la projection de texture" << endl;
		return false;
	}
	if(pCaps->MaxTextureWidth < 1024 || pCaps->MaxTextureHeight < 1024)
	{
		Console << "Le peripherique ne permet pas l'utilisation de texture de taille egale a 1024" << endl;
		return false;
	}
	if(!(pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP))
	{
		Console << "Le peripherique ne permet pas l'utilisation du bump mapping" << endl;
		return false;
	}
	if(pCaps->MaxSimultaneousTextures < 2)
	{
		Console << "Le peripherique ne permet pas l'utilisation de 2 etages de texture" << endl;
		return false;
	}
	if(pCaps->MaxUserClipPlanes < 1)
	{
		Console << "Le peripherique ne permet pas l'utilisation de plan de clipping" << endl;
		return false;
	}
	return true;
}

// Initialisation de la scène 3D
bool CSceneConfig::Initialisation()
{
	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(100.0f, 10.0f, 0.0f));

	// Création des modèls
	m_pModelRoche = new CModelRoche(this);
	m_pModelRoche->CreationRoche();
	m_pModelSol = new CModelSol(this);
	m_pModelSol->CreationSol();
	m_pModelEau = new CModelEau(this);
	m_pModelEau->CreationEau();
	m_pModelVegetation = new CModelVegetation(this);
	m_pModelVegetation->CreationVegetation();
	m_pSkyBox = new CSkyBox(this);

	return true;
}

// Création des objets 3D de la scène
bool CSceneConfig::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);

    // Configuration du brouillard
	float Start = 500.0f;
    float End   = 2500.0f;
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, 0x0087bffa);
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
    m_pD3DDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));

	// Chargement des modèls
	m_pModelRoche->ChargerRoche();
	m_pModelSol->ChargerSol();
	m_pModelEau->ChargerEau();
	m_pModelVegetation->ChargerVegetation();
	m_pSkyBox->ChargerModel("Donnees\\Sky\\skybox2.x");

	// Initialisation de la lumière 1 et activation
	m_LumiereD1.SetDirectionnelle(&D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	m_LumiereD1.SetAmbiante(1.0f, 1.0f, 1.0f);
	m_LumiereD1.SetDiffuse(1.0f, 1.0f, 1.0f);
	m_LumiereD1.SetSpeculaire(1.0f, 1.0f, 1.0f);
	m_pD3DDevice->SetLight(0, m_LumiereD1.GetLumiere()); 
	m_pD3DDevice->LightEnable(0, TRUE);

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneConfig::DestructionObjet()
{
	m_pModelRoche->Release();
	m_pModelSol->Release();
	m_pModelEau->Release();
	m_pModelVegetation->Release();
	m_pSkyBox->Release();
}

// Destruction des objets
void CSceneConfig::Destruction()
{
	delete m_pModelRoche;
	delete m_pModelSol;
	delete m_pModelEau;
	delete m_pModelVegetation;
	delete m_pSkyBox;
}

// Rendu 3D de la scène
void CSceneConfig::Rendu3D()
{
	m_pSkyBox->RenduModel();
	m_pModelRoche->RenduRoche();
	m_pModelSol->RenduSol();
	m_pModelEau->RenduEau();
	m_pModelVegetation->RenduVegetation();
}

// Calculs des animations de la scène
void CSceneConfig::Animation(double TempsEcouler, double DeltaTemps)
{
	// Mise à jour des information des modèls
	m_pModelEau->MiseAJourDonnees(TempsEcouler);
	m_pModelVegetation->MiseAJourDonnees();
	// On applique une transformation à la caméra (nécésaire pour le bon calcul des LOD)
	GetCameraCourante()->SetMatriceMiroir(&m_pModelEau->GetMatriceReflection());
	// Mise à jour des information des modèls pour le rendu de la réflexion
	m_pModelRoche->SetCoefLOD(1000.0f, m_CoefLODRocheR[m_QualiterModel]);
	m_pModelSol->SetCoefLOD(500.0f, m_CoefLODSolR[m_QualiterModel]);
	m_pModelRoche->MiseAJourDonnees();
	m_pModelSol->MiseAJourDonnees();
	// Rendu de la texture de réflexion
	m_pModelEau->RenduEnvMiroir();
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
	// Mise à jour des information des modèls pour le rendu
	m_pModelRoche->SetCoefLOD(3000.0f, m_CoefLODRoche[m_QualiterModel]);
	m_pModelSol->SetCoefLOD(1000.0f, m_CoefLODSol[m_QualiterModel]);
	m_pModelRoche->MiseAJourDonnees();
	m_pModelSol->MiseAJourDonnees();
}

// Test des entrées des périphériques
void CSceneConfig::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris)
{
	// On test si on a changé un paramètre de qualité
	if(KEYDOWN(DIK_F1))
	{
		m_QualiterFiltrage = 0;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F2))
	{
		m_QualiterFiltrage = 1;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F3))
	{
		m_QualiterFiltrage = 2;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F5))  m_QualiterModel = 0;
	if(KEYDOWN(DIK_F6))  m_QualiterModel = 1;
	if(KEYDOWN(DIK_F7))  m_QualiterModel = 2;
}

// Appliqué les paramètres de filtrage de texture
void CSceneConfig::SetFiltreTexture(int Type)
{
	switch(Type)
	{
		// Filtrage Bilinéaire
		case 0:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			break;
		// Filtrage Tilinéaire
		case 1:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			break;
		// Filtrage Anisotropic
		case 2:
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			break;
	}
}
