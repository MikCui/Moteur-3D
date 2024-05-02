
// Classe pour la gestion du clavier

#pragma once
#include "VariableGlobale.h"
#include "Peripherique.h"

class CPeripherique;

class CClavier
{
public:
	CPeripherique* m_pPeripherique;			// pointeur sur le gestionnaire de phériphérique
	LPDIRECTINPUTDEVICE8 m_pDIClavier;		// pointeur sur l'objet Clavier
	BYTE m_EtatClavier[256];				// états du clavier

	CClavier();
	CClavier(CPeripherique* pPeripherique);
	~CClavier();

	bool Creation(LPDIRECTINPUT8 pDI);
	void Destruction();
	bool GetEtatClavier(bool bActiveAquisition);
	bool Acquisition(){m_pDIClavier->Acquire(); return true;}
	bool DeAcquisition(){m_pDIClavier->Unacquire(); return true;}
	BYTE* GetTableauEtatClavier(){return m_EtatClavier;}
	CPeripherique* GetPeripherique(){return m_pPeripherique;}
};
