#include "Mesuretemps.h"

CMesureTemps::CMesureTemps(void)
{
	m_TempsDebut = 0;
	m_TempsFin = 0;
	m_AncienTempsEcouler = 0;
	m_bCompteurArreter = true;
	
	LARGE_INTEGER TicksParSec;
	QueryPerformanceFrequency(&TicksParSec);
	m_TicksParSec = (double)TicksParSec.QuadPart;
}

CMesureTemps::~CMesureTemps(void)
{
}

void CMesureTemps::SetCoefTemps(double Coef)
{
	LARGE_INTEGER TicksParSec;
	QueryPerformanceFrequency(&TicksParSec);
	m_TicksParSec = (double)(TicksParSec.QuadPart / Coef);
}

void CMesureTemps::Demarrer()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	if(m_bCompteurArreter)
		m_TempsDebut += Time.QuadPart - m_TempsFin;
	m_TempsFin = 0;
	m_AncienTempsEcouler = Time.QuadPart;
	m_bCompteurArreter = false;
}

void CMesureTemps::Arreter()
{
	if(!m_bCompteurArreter)
	{
		LARGE_INTEGER Time;
		QueryPerformanceCounter(&Time);
		m_TempsFin = Time.QuadPart;
		m_bCompteurArreter = true;
	}
}

void CMesureTemps::RemiseAZero()
{
	if(m_bCompteurArreter)
	{
		m_TempsDebut = 0;
		m_AncienTempsEcouler = 0;
	}
	else
	{
		LARGE_INTEGER Time;
		QueryPerformanceCounter(&Time);
		m_TempsDebut = Time.QuadPart;
		m_AncienTempsEcouler = Time.QuadPart;
	}
	m_TempsFin = 0;
}

double CMesureTemps::TempsAbsolus()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	return (double)(Time.QuadPart / m_TicksParSec);
}

double CMesureTemps::TempsCourant()
{
	if(m_bCompteurArreter)
	{
		return (double)(m_TempsFin - m_TempsDebut) / m_TicksParSec;
	}
	else
	{
		LARGE_INTEGER Time;
		QueryPerformanceCounter(&Time);
		return (double)(Time.QuadPart - m_TempsDebut) / m_TicksParSec;
	}
}

double CMesureTemps::TempsEcouler()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	double TempsEcouler = (double)(Time.QuadPart - m_AncienTempsEcouler) / m_TicksParSec;
	m_AncienTempsEcouler = Time.QuadPart;
	return TempsEcouler;
}

double CMesureTemps::GetFrequence()
{
	return m_TicksParSec;
}
