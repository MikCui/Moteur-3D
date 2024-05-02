#pragma once
#include <windows.h>

class CMesureTemps
{
	double m_TicksParSec;
	LONGLONG m_TempsDebut;
	LONGLONG m_TempsFin;
	LONGLONG m_AncienTempsEcouler;
	bool m_bCompteurArreter;

public:
	CMesureTemps(void);
	~CMesureTemps(void);
	void SetCoefTemps(double Coef);
	void Demarrer();
	void Arreter();
	void RemiseAZero();
	double TempsAbsolus();
	double TempsCourant();
	double TempsEcouler();
	double GetFrequence();

};
