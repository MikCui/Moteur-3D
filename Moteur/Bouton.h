
// Classe pour la gestion de bouton

#pragma once
#include "VariableGlobale.h"
#include "Scene.h"

#define EVENEMENT	1
#define SCRUTATION	2

class CScene;

class CBouton
{
public:

	DWORD	m_Type;					// Tupe de bouton : EVENEMENT ou SCRUTATION
	CScene*	m_pScene;				// pointeur sur la scène
	BYTE	m_ToucheClavier;		// touche du clavier associé au bouton
	BYTE	m_ToucheSouris;			// touche dde la souris associée au bouton
	BYTE	m_ToucheJoystick;		// touche du joystick associé au bouton
	BYTE	m_ToucheClavierBuf;		// sauvegarde de l'état de la touche
	BYTE	m_ToucheSourisBuf;		// sauvegarde de l'état de la touche
	BYTE	m_ToucheJoystickBuf;	// sauvegarde de l'état de la touche

	CBouton();
	CBouton(CScene*	pScene);
	~CBouton();
	bool Creation(BYTE ToucheClavier, DWORD Type = SCRUTATION);
	void SetToucheClavier(BYTE ToucheClavier){m_ToucheClavier = ToucheClavier;}
	void SetToucheSouris(BYTE ToucheSouris){m_ToucheSouris = ToucheSouris;}
	void SetToucheJoystick(BYTE ToucheJoystick){m_ToucheJoystick = ToucheJoystick;}
	void SetType(DWORD Type){m_Type = Type;}
	bool GetActive();
};
