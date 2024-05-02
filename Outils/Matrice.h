#pragma once

#include <Windows.h>
#include "Console.h"

template<class T> 
class CMatrice
{
public:
	CMatrice();
	CMatrice(int Colonne, int Ligne);
	CMatrice(int Colonne, int Ligne, T ValeurParDefaut);
	CMatrice(const CMatrice& m);
	~CMatrice();
	bool Creation(int Colonne, int Ligne);
	T* GetDonnees(){return m_pDonnees;}
	CMatrice& operator= (const CMatrice& m);
	T& operator() (int  Colonne, int  Ligne);

	T* m_pDonnees;
	int m_NbColonne;
	int m_NbLigne;
	int m_TailleDonnnes;
};

template<class T> 
CMatrice<T>::CMatrice()
{
	m_pDonnees = NULL;
	m_NbColonne = 0;
	m_NbLigne = 0;
	m_TailleDonnnes = 0;
}

template<class T> 
CMatrice<T>::CMatrice(int Colonne, int Ligne)
{
	m_pDonnees = NULL;
	m_NbColonne = Colonne;
	m_NbLigne = Ligne;
	m_TailleDonnnes = m_NbColonne*m_NbLigne;
	m_pDonnees = new T[m_TailleDonnnes];
}

template<class T> 
CMatrice<T>::CMatrice(int Colonne, int Ligne, T ValeurParDefaut)
{
	m_pDonnees = NULL;
	m_NbColonne = Colonne;
	m_NbLigne = Ligne;
	m_TailleDonnnes = m_NbColonne*m_NbLigne;
	m_pDonnees = new T[m_TailleDonnnes];
	for(int c=0; c<m_TailleDonnnes; c++)
		m_pDonnees[c] = ValeurParDefaut;
}

template<class T> 
CMatrice<T>::CMatrice(const CMatrice& m)
{
	m_NbColonne = m.m_NbColonne;
	m_NbLigne = m.m_NbColonne;
	m_TailleDonnnes = m.m_TailleDonnnes;
	m_pDonnees = new T[m_TailleDonnnes];
	memcpy(m_pDonnees, m.m_pDonnees, sizeof(T)*m_TailleDonnnes);
}

template<class T> 
CMatrice<T>::~CMatrice()
{
	delete[] m_pDonnees;
}

template<class T> 
bool CMatrice<T>::Creation(int Colonne, int Ligne)
{
	if(m_pDonnees)
		delete[] m_pDonnees;

	m_pDonnees = NULL;
	m_NbColonne = Colonne;
	m_NbLigne = Ligne;
	m_TailleDonnnes = m_NbColonne*m_NbLigne;
	m_pDonnees = new T[m_TailleDonnnes];
	return m_pDonnees != NULL;
}

template<class T> 
CMatrice<T>& CMatrice<T>::operator = (const CMatrice& m)
{
	if(this == &m)
		return *this;

	if(m_pDonnees)
		delete[] m_pDonnees;

	m_NbColonne = m.m_NbColonne;
	m_NbLigne = m.m_NbColonne;
	m_TailleDonnnes = m.m_TailleDonnnes;
	m_pDonnees = new T[m_TailleDonnnes];
	memcpy(m_pDonnees, m.m_pDonnees, sizeof(T)*m_TailleDonnnes);
	return *this;
}

template<class T>
T& CMatrice<T>::operator() (int Colonne, int Ligne)
{
	return m_pDonnees[Colonne+Ligne*m_NbColonne];
}