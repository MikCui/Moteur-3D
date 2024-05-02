#include "Souris.h"

CSouris::CSouris()
{
	Console << "Constructeur CSouris()" << endl;
}

CSouris::CSouris(CPeripherique* pPeripherique)
{
	m_pPeripherique = pPeripherique;
	m_pDISouris = NULL;
}

CSouris::~CSouris()
{
	Console << "Deconstructeur CSouris()" << endl;
}

// Création de la souris
bool CSouris::Creation(LPDIRECTINPUT8 pDI)
{
	HRESULT hr;

	// Création de l'objet Souris
	hr = pDI->CreateDevice(GUID_SysMouse, &m_pDISouris, NULL);
	if FAILED(hr) 
	{ 
		Console << "Impossible d'initialiser la souris" << endl;
		return false;
	}

	// Initialisation du format de données
	hr = m_pDISouris->SetDataFormat(&c_dfDIMouse);
	if FAILED(hr) 
	{ 
		Console << "Impossible d'initialiser le format de donnees pour la souris" << endl;
		return false;
	}

	// Initialisation du niveau de coopération du souris
	hr = m_pDISouris->SetCooperativeLevel(GetPeripherique()->GetMoteur()->GetFenetre(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if FAILED(hr) 
	{ 
		Console << "Impossible d'etablire le niveau de cooperation de la souris" << endl;
		return false;
	}
	return true;
}

// Destruction de al souris
void CSouris::Destruction()
{
	if(m_pDISouris)m_pDISouris->Unacquire();
	SAFE_RELEASE(m_pDISouris);
}

// Mise à jour de l'états de  la souris
bool CSouris::GetEtatSouris(bool bActiveAquisition)
{
	if(!bActiveAquisition)
	{
		ZeroMemory(&m_EtatSouris, sizeof(m_EtatSouris));
		return true;
	}

	HRESULT hr;
	// on récupère l'états de la souris
	hr = m_pDISouris->GetDeviceState(sizeof(m_EtatSouris), (LPVOID)&m_EtatSouris);
	if FAILED(hr) 
	{ 
		ZeroMemory(&m_EtatSouris, sizeof(m_EtatSouris));
		Console << "Impossible de recuperer l'etat de la souris" << endl;
		return false;
	}
	return true;
}
