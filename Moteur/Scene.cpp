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

// Activation d'une cam�ra
void CScene::SetCamera(CCamera* pCamera)
{
	m_pCameraCourante = pCamera;
	m_pCameraCourante->SetMatrice();
}

// Cr�ation de la sc�ne 3D
bool CScene::Creation()
{
	m_pD3DDevice = m_pMoteur->GetMoteur3D()->GetDevice3D();
	// Cr�ation d'une cam�ra par defaut
	m_pCameraParDefaut = new CCamera(this);
	m_pCameraParDefaut->Creation();

	// Structure sur les capacit�s du p�riph�rique 3D
	D3DCAPS9 Caps;
	// On r�cup�re les informations sur les capacit�s du p�riph�rique 3D
	m_pD3DDevice->GetDeviceCaps(&Caps);

	// V�rification des fonctionalit� du p�riph�rique 3D
    if(!ConfirmeFonction3D(&Caps))return false;
	// Initialisation de la sc�ne
	if(!Initialisation())return false;
	// Cr�ation des diff�rent objets de la sc�ne
	if(!CreationObjet())return false;

	// On r�cup�re l'adresses des variables des �tats des p�riph�riques
	m_pEtatClavier = m_pMoteur->GetPeripherique()->GetClavier()->GetTableauEtatClavier();
	m_pEtatSouris = m_pMoteur->GetPeripherique()->GetSouris()->GetStructEtatSouris();

	// On lance le compteur de temps
	m_Temps.Demarrer();
	return true;
}

// Destruction des objets cr�e par defaut
void CScene::DestructionFinal()
{
	// Destruction des objets de la sc�ne
	DestructionObjet();
	// Destrcution de la sc�ne
	Destruction();
	// Destrcution de la camera par defaut
	if(m_pCameraParDefaut)
	{
		delete m_pCameraParDefaut;
		m_pCameraParDefaut = NULL;
	}
}

// Rendu de la sc�ne
void CScene::Rendu()
{
	// On v�rifie que le rendu 3D est possible
	if(!m_pMoteur->GetDeviceLost())
	{
		// On r�cup�re le temps �coul� depuis le dernier rendu de la sc�ne
		double DeltaTemps = m_Temps.TempsEcouler();
		// Mise � jour des �tats des p�riph�riques
		m_pMoteur->GetEtatPeripherique();
		// Mise � jour des donn�e de la sc�ne en fonction des �tats des p�riph�riques
		Aquisition(m_pEtatClavier, m_pEtatSouris);
		// Si la camera courante est celle par defaut, on la bouge en fonction de la souris et du clavier
		if(m_pCameraCourante == m_pCameraParDefaut)
			m_pCameraParDefaut->Animation(m_pEtatClavier, m_pEtatSouris, (float)DeltaTemps);
		// Animation de la sc�ne 
		Animation(m_Temps.TempsCourant(), DeltaTemps);
		DebutRendu3D();
		// Rendu 3D de le sc�ne
		Rendu3D();
		if(FinRendu3D() == D3DERR_DEVICELOST)
		{
			Console << "Perte du pheripherique 3D" << endl;
			m_pMoteur->SetDeviceLost(true);
		}
	}
}

// Initialisation de la sc�ne par defaut
bool CScene::CreationObjet()
{
	SetMaterielle(&m_MaterielleParDefaut);
	SetLumiere(1, &m_LumiereParDefaut, true);
	return true;
}

// D�but du rendu 3D de la sc�ne
void CScene::DebutRendu3D()
{
	// On efface le buffer image et le zbuffer
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	m_pD3DDevice->BeginScene();
}

// Fin du rendu 3D de la sc�ne
HRESULT CScene::FinRendu3D()
{
	m_pD3DDevice->EndScene();
	// On transmet au p�riph�rique les ordres d'affichage
	return m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}