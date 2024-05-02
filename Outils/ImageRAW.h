
// Classe pour la gestion des images RAW

#pragma once
#include <fstream>
#include <Windows.h>

class CImageRAW
{
public:
	CImageRAW();
	CImageRAW(const CImageRAW& Image);
	~CImageRAW();

	// Structure de l'image RAW
	struct EnteteRAW
	{
		WORD Largeur;				// Largeur de l'image
		WORD Hauteur;				// Hauteur de l'image
		BYTE NbComposante;			// Nombre de composante pour un pixel
		BYTE NbBits;				// Nombre de bits par composante
	};

	EnteteRAW m_Entete;				// Structure de l'image
	BYTE* m_Donnee;					// Pointeur sur les données de l'image

	bool Charger(char* NomFichier, WORD Largeur, WORD Hauteur, BYTE NbComposante, BYTE NbBits);
	bool Creer(WORD Largeur, WORD Hauteur, BYTE NbComposante, BYTE NbBits);
	bool Enregister(char* NomFichier);

	void*		GetDonnees(){return m_Donnee;}
	DWORD		GetLargeur(){return m_Entete.Largeur;}
	DWORD		GetHauteur(){return m_Entete.Hauteur;}
	void*		GetPixelAdresse(int X, int Y);
	void		SetPixel(int X, int Y, void* pCouleur);
	bool        IsVide(){return m_Donnee == NULL;}
	EnteteRAW*  GetEntete(){return &m_Entete;}

	CImageRAW& operator=(const CImageRAW& Image);
};