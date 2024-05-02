#include "Scene.h"

CScene::CScene()
{
	Console << "Constructeur CScene()" << endl;
}

CScene::CScene(CMoteur* pMoteur)
{
	m_pD3DDevice = NULL;
	m_pCameraCourante = NULL;
	m_pCameraParDefaut = NULL;
	m_pMoteur = pMoteur;
	m_pEtatClavier = NULL;
	m_pEtatSouris = NULL;
	m_RatioFenetre = 0.0f;
	m_bRenduReflection = false;
	m_bRenduBump = false;
}

CScene::~CScene()
{
	Console << "Deconstructeur CScene" << endl;
}

// Activation d'une caméra
void CScene::SetCamera(CCamera* pCamera)
{
	m_pCameraCourante = pCamera;
	m_pCameraCourante->SetMatrice();
}

// Création de la scène 3D
bool CScene::Creation()
{
	m_pD3DDevice = m_pMoteur->GetMoteur3D()->GetDevice3D();
	// Création d'une caméra par defaut
	m_pCameraParDefaut = new CCamera(this);
	m_pCameraParDefaut->Creation();

	// Structure sur les capacités du périphérique 3D
	D3DCAPS9 Caps;
	// On récupère les informations sur les capacités du périphérique 3D
	m_pD3DDevice->GetDeviceCaps(&Caps);

	// Vérification des fonctionalité du périphérique 3D
    if(!ConfirmeFonction3D(&Caps))return false;
	// Initialisation de la scène
	if(!Initialisation())return false;
	// Création des différent objets de la scène
	if(!CreationObjet())return false;

	// On récupère l'adresses des variables des états des périphériques
	m_pEtatClavier = m_pMoteur->GetPeripherique()->GetClavier()->GetTableauEtatClavier();
	m_pEtatSouris = m_pMoteur->GetPeripherique()->GetSouris()->GetStructEtatSouris();

	// On lance le compteur de temps
	m_Temps.Demarrer();
	return true;
}

// Destruction des objets crée par defaut
void CScene::DestructionFinal()
{
	// Destruction des objets de la scène
	DestructionObjet();
	// Destrcution de la scène
	Destruction();
	// Destrcution de la camera par defaut
	if(m_pCameraParDefaut)
	{
		delete m_pCameraParDefaut;
		m_pCameraParDefaut = NULL;
	}
}

// Rendu de la scène
void CScene::Rendu()
{
	// On vérifie que le rendu 3D est possible
	if(!m_pMoteur->GetDeviceLost())
	{
		// On récupère le temps écoulé depuis le dernier rendu de la scène
		double DeltaTemps = m_Temps.TempsEcouler();
		// Mise à jour des états des périphériques
		m_pMoteur->GetEtatPeripherique();
		// Mise à jour des donnée de la scène en fonction des états des périphériques
		Aquisition(m_pEtatClavier, m_pEtatSouris);
		// Si la camera courante est celle par defaut, on la bouge en fonction de la souris et du clavier
		if(m_pCameraCourante == m_pCameraParDefaut)
			m_pCameraParDefaut->Animation(m_pEtatClavier, m_pEtatSouris, (float)DeltaTemps);
		// Animation de la scène 
		Animation(m_Temps.TempsCourant(), DeltaTemps);
		DebutRendu3D();
		// Rendu 3D de le scène
		Rendu3D();
		if(FinRendu3D() == D3DERR_DEVICELOST)
		{
			Console << "Perte du pheripherique 3D" << endl;
			m_pMoteur->SetDeviceLost(true);
		}
	}
}

// Initialisation de la scène par defaut
bool CScene::CreationObjet()
{
	SetMaterielle(&m_MaterielleParDefaut);
	SetLumiere(1, &m_LumiereParDefaut, true);
	return true;
}

// Début du rendu 3D de la scène
void CScene::DebutRendu3D()
{
	// On efface le buffer image et le zbuffer
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	m_pD3DDevice->BeginScene();
}

// Fin du rendu 3D de la scène
HRESULT CScene::FinRendu3D()
{
	m_pD3DDevice->EndScene();
	// On transmet au périphérique les ordres d'affichage
	return m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}