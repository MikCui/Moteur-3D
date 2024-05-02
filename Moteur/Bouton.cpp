#include "Bouton.h"

CBouton::CBouton()
{
}

CBouton::CBouton(CScene* pScene)
{
	m_pScene = NULL;
	m_Type = 0;
	m_ToucheClavier = 0;
	m_ToucheSouris = 0;
	m_ToucheJoystick = 0;
	m_ToucheClavierBuf = 0;
	m_ToucheSourisBuf = 0;
	m_ToucheJoystickBuf = 0;
	m_pScene = pScene;
}

CBouton::~CBouton()
{
}


// Création du bouton
bool CBouton::Creation(BYTE ToucheClavier, DWORD Type)
{
	m_ToucheClavier = ToucheClavier;
	m_Type = Type;
	return true;
}

// Test si le bouton est enfoncé
bool CBouton::GetActive()
{
	bool bActive = false;
	BYTE Etat = (m_pScene->GetTableauEtatClavier())[m_ToucheClavier];

	if(m_Type == EVENEMENT)
	{
		if(Etat & 0x80 & ~m_ToucheClavierBuf) bActive = true;
		m_ToucheClavierBuf = Etat;
	}
	else
	{
		if(Etat & 0x80)	bActive = true;
	}
	return bActive;
}