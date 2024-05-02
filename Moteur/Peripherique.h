
// Classe pour la gestion des phériphériques d'entrées

#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "VariableGlobale.h"
#include "Moteur.h"
#include "Clavier.h"
#include "Souris.h"

#define PERIPHERIQUE_CLAVIER	0x00000001
#define PERIPHERIQUE_SOURIS		0x00000002
#define PERIPHERIQUE_JOYSTICK	0x00000004
#define PERIPHERIQUE_TOUS		0xFFFFFFFF

class CMoteur;
class CClavier;
class CSouris;
//class CJoystick;

class CPeripherique
{
public:

	CMoteur*			m_pMoteur;		// pointeur sur le Moteur
	LPDIRECTINPUT8		m_pDI;			// pointeur sur DirectInput
	CClavier*           m_pClavier;		// pointeur sur le Clavier
	CSouris*			m_pSouris;		// pointeur sur la Souris
	//CJoystick			m_pJoystick;	// pointeur sur le Joystick

	CPeripherique();
	CPeripherique(CMoteur* pMoteur);
	~CPeripherique();

	bool			Creation(DWORD Type = PERIPHERIQUE_TOUS);
	void			Destruction();
	bool			Acquisition();
	bool			DeAcquisition();
	bool			GetEtatPeripherique(bool bActiveAquisition);
	CMoteur*		GetMoteur(){return m_pMoteur;}
	CClavier*		GetClavier(){return m_pClavier;}
	CSouris*		GetSouris(){return m_pSouris;}
	LPDIRECTINPUT8	GetDirectInput(){return m_pDI;}
};
