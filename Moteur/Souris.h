
// Classe pour la gestion de la souris

#pragma once
#include "VariableGlobale.h"
#include "Peripherique.h"

class CPeripherique;

class CSouris
{
public:
	CPeripherique* m_pPeripherique;			// pointeur sur le gestionnaire de phériphérique
	LPDIRECTINPUTDEVICE8 m_pDISouris;		// pointeur sur l'objet Souris
	DIMOUSESTATE	m_EtatSouris;			// états de la souris

	CSouris();
	CSouris(CPeripherique* pPeripherique);
	~CSouris();

	bool Creation(LPDIRECTINPUT8 pDI);
	void Destruction();
	bool GetEtatSouris(bool bActiveAquisition);
	bool Acquisition(){m_pDISouris->Acquire(); return true;}
	bool DeAcquisition(){m_pDISouris->Unacquire(); return true;}
	DIMOUSESTATE* GetStructEtatSouris(){return &m_EtatSouris;}
	CPeripherique* GetPeripherique(){return m_pPeripherique;}
};
