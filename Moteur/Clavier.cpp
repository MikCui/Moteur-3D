#include "Clavier.h"

CClavier::CClavier()
{
	Console << "Constructeur CClavier()" << endl;
}

CClavier::CClavier(CPeripherique* pPeripherique)
{
	m_pPeripherique = pPeripherique;
	m_pDIClavier = NULL;
}

CClavier::~CClavier()
{
	Console << "Deconstructeur CClavier()" << endl;
}

// Cr�ation du clavier
bool CClavier::Creation(LPDIRECTINPUT8 pDI)
{
	HRESULT hr;

	// Cr�ation de l'objet Clavier
	hr = pDI->CreateDevice(GUID_SysKeyboard, &m_pDIClavier, NULL); 
	if FAILED(hr) 
	{ 
		Console << "Impossible d'initialiser le clavier" << endl;
		return false;
	}

	// Initialisation du format de donn�es
	hr = m_pDIClavier->SetDataFormat(&c_dfDIKeyboard);
	if FAILED(hr) 
	{ 
		Console << "Impossible d'initialiser le format de donnees pour le clavier" << endl;
		return false;
	}

	// Initialisation du niveau de coop�ration du clavier
	hr = m_pDIClavier->SetCooperativeLevel(GetPeripherique()->GetMoteur()->GetFenetre(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if FAILED(hr) 
	{ 
		Console << "Impossible d'etablire le niveau de cooperation du clavier" << endl;
		return false;
	}
	return true;
}

// Destruction du clavier
void CClavier::Destruction()
{
	if(m_pDIClavier)m_pDIClavier->Unacquire();
	SAFE_RELEASE(m_pDIClavier);
}

// Mise � jour de l'�tats du clavier
bool CClavier::GetEtatClavier(bool bActiveAquisition)
{
	if(!bActiveAquisition)
	{
		ZeroMemory(&m_EtatClavier, sizeof(m_EtatClavier));
		return true;
	}

	HRESULT hr;
	// on r�cup�re l'�tats du clavier
	hr = m_pDIClavier->GetDeviceState(sizeof(m_EtatClavier), (LPVOID)&m_EtatClavier);
	if FAILED(hr) 
	{ 
		ZeroMemory(&m_EtatClavier, sizeof(m_EtatClavier));
		Console << "Impossible de recuperer l'etat du clavier" << endl;
		return false;
	}
	return true;
}
