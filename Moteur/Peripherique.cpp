#include "Peripherique.h"

CPeripherique::CPeripherique()
{
	Console << "Constructeur CPeripherique()" << endl;
}

CPeripherique::CPeripherique(CMoteur* pMoteur)
{
	m_pMoteur = pMoteur;
	m_pDI = NULL;
	m_pClavier = new CClavier(this);
	m_pSouris = new CSouris(this);
	//m_pJoystick = NULL;
}

CPeripherique::~CPeripherique()
{
	Console << "Deconstructeur CPeripherique()" << endl;
}

// Création des périphériques d'entrées
bool CPeripherique::Creation(DWORD Type)
{
	HRESULT hr;
	// Création de l'objet DirectInput
	hr = DirectInput8Create(m_pMoteur->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL); 
	if FAILED(hr) 
	{ 
		Console << "Impossible d'initialiser DirectInput" << endl;
		return false;
	}
	// Création du clavier
	if(Type && PERIPHERIQUE_CLAVIER)
	{	if(!m_pClavier) return false;
		if(!m_pClavier->Creation(m_pDI)) return false;
	}
	// Création de la souris
	if(Type && PERIPHERIQUE_SOURIS)
	{
		if(!m_pSouris) return false;
		if(!m_pSouris->Creation(m_pDI)) return false;
	}
	// Création du Joystick
	/*if(Type && PERIPHERIQUE_JOYSTICK)
	{
		m_pJoystick = new CJoystick(this);
		if(!m_pJoystick->Creation(m_pDI)) return false;
	}*/
	return true;
}

// Destruction des périphériques d'entrées
void CPeripherique::Destruction()
{
	// Destruction du clavier
	if(m_pClavier)
	{
		m_pClavier->Destruction();
		m_pClavier = NULL;
	}
	// Destruction de la souris
	if(m_pSouris)
	{
		m_pSouris->Destruction();
		m_pSouris = NULL;
	}
	// Destrction de l'objet DirectInput
	SAFE_RELEASE(m_pDI);
}

// périphériques en mode Acquisition
bool CPeripherique::Acquisition()
{
	if(m_pClavier)m_pClavier->Acquisition();
	if(m_pSouris)m_pSouris->Acquisition();
	//if(m_pJoystick)m_pJoystick->m_pDIJoystick->Acquisition();
	return true;
}

// Déacquisition des périphériques
bool CPeripherique::DeAcquisition()
{
	if(m_pDI)
	{
		if(m_pClavier)m_pClavier->DeAcquisition();
		if(m_pSouris)m_pSouris->DeAcquisition();
		//if(m_pJoystick)m_pJoystick->m_pDIJoystick->DeAcquisition();
	}
	return true;
}

// Récupération des états des périphériques
bool CPeripherique::GetEtatPeripherique(bool bActiveAquisition)
{
	if(m_pClavier)m_pClavier->GetEtatClavier(bActiveAquisition);
	if(m_pSouris)m_pSouris->GetEtatSouris(bActiveAquisition);
	//if(m_pJoystick)m_pJoystick->GetEtatJoystick(bActive);
	return true;
}