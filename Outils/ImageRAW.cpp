#include "ImageRAW.h"

//#include "Console.h"
//CConsole cout;

#include <iostream>
using namespace std;

CImageRAW::CImageRAW()
{
	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));
	m_Donnee = NULL;
}

CImageRAW::~CImageRAW()
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}
}

// Constructeur par recopie
CImageRAW::CImageRAW(const CImageRAW& Image)
{
	m_Donnee = NULL;
	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.Largeur, Image.m_Entete.Hauteur, Image.m_Entete.NbComposante, Image.m_Entete.NbBits))
		{
			DWORD TailleDonnees = m_Entete.Largeur * m_Entete.Hauteur * m_Entete.NbComposante * (Image.m_Entete.NbBits / 8);
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
}


// Operateur d'affectation entre images
CImageRAW& CImageRAW::operator=(const CImageRAW& Image)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}

	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.Largeur, Image.m_Entete.Hauteur, Image.m_Entete.NbComposante, Image.m_Entete.NbBits))
		{
			DWORD TailleDonnees = m_Entete.Largeur * m_Entete.Hauteur * m_Entete.NbComposante * (Image.m_Entete.NbBits / 8);
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
	return *this;
}

// Création d'une image vide
bool CImageRAW::Creer(WORD Largeur, WORD Hauteur, BYTE NbComposante, BYTE NbBits)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}

	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));

	// On vérifie si le nombre de bits d'une composante est un multiple de 8
	if(NbBits%8 != 0)
		return false;

	// On initialise la Structure de l'image
	m_Entete.Largeur = Largeur;
	m_Entete.Hauteur = Hauteur;
	m_Entete.NbComposante = NbComposante;
	m_Entete.NbBits = NbBits;

	// Calcule de la taille des données
	DWORD TailleDonnees = m_Entete.Largeur * m_Entete.Hauteur * m_Entete.NbComposante * (m_Entete.NbBits / 8);

	// Allocation du tableau de données
	m_Donnee = new BYTE[TailleDonnees];
	return m_Donnee != NULL;
}

bool CImageRAW::Charger(char* NomFichier, WORD Largeur, WORD Hauteur, BYTE NbComposante, BYTE NbBits)
{
	std::ifstream Fichier;

	// On vérifie si le nombre de bits d'une composante est un multiple de 8
	if(NbBits%8 != 0)
		return false;

	// On initialise la Structure de l'image
	m_Entete.Largeur = Largeur;
	m_Entete.Hauteur = Hauteur;
	m_Entete.NbComposante = NbComposante;
	m_Entete.NbBits = NbBits;

	// On ouvre le ficher en mode binaire
	Fichier.open(NomFichier, std::ios::binary);

	// On vérifie qu'il est bien ouvert
	if(!Fichier.is_open())
	{
		Fichier.close();
		cout << "Impossible d'ouvire le fichier : " << NomFichier << endl;
		return false;
	}

    // Calcule de la taille des données
	DWORD TailleDonnees = m_Entete.Largeur * m_Entete.Hauteur * m_Entete.NbComposante * (m_Entete.NbBits / 8);

	// Allocation du tableau de données
	m_Donnee = new BYTE[TailleDonnees];

	if(m_Donnee != NULL)
	{
		// Chargement des données à partir du fichier
		Fichier.read((char*)m_Donnee, TailleDonnees);
	}
	else
	{
		Fichier.close();
		cout << "Impossible de creer le tableau de donnees, memoire insuffisante" << endl;
		return false;
	}

	// On à atteins la fin du fichier prématurément
	if(Fichier.eof())
	{
		Fichier.close();
		cout << "Erreur a la lecture des donnees du fichier : " << NomFichier << endl;
		return false;
	}

	// on ferme le ficher ouvert
	Fichier.close();
	return true;
}

// Enregistrer l'image dans un fichier
bool CImageRAW::Enregister(char* NomFichier)
{
	// On vérifie si il y a des données
	if(m_Donnee == NULL)
	{
		cout << "Erreur aucune donnees a enregistrer pour le fichier : " << NomFichier << endl;
		return false;
	}

	std::ofstream Fichier;
	
	// On ouvre le ficher en mode binaire
	Fichier.open(NomFichier, std::ios::binary);

	// On vérifie qu'il est bien ouvert
	if(!Fichier.is_open())
	{
		Fichier.close();
		cout << "Impossible de creer le fichier : " << NomFichier << endl;
		return false;
	}

	DWORD TailleDonnees = m_Entete.Largeur * m_Entete.Hauteur * m_Entete.NbComposante * (m_Entete.NbBits / 8);
	// Ecriture des données de l'image
	Fichier.write((char*)m_Donnee, TailleDonnees);
	// Fermeture du fichier
	Fichier.close();
	return true;
}

// Obtenire l'adresse d'un pixel
void* CImageRAW::GetPixelAdresse(int X, int Y)
{
	if(X < 0 || Y < 0 || X >= m_Entete.Largeur || Y >= m_Entete.Hauteur)
		return NULL;
	else
		//return &m_Donnee[(m_Entete.Largeur * (m_Entete.Hauteur - 1 - Y) + X)];
		return &m_Donnee[(m_Entete.Largeur * Y + X) * (m_Entete.NbBits / 8)];

}

// Modifier la donnée d'un pixel
void CImageRAW::SetPixel(int X, int Y, void* pCouleur)
{
	if(X < 0 || Y < 0 || X >= m_Entete.Largeur || Y >= m_Entete.Hauteur)
		return;
	else
		memcpy(GetPixelAdresse(X, Y), pCouleur, m_Entete.NbComposante * (m_Entete.NbBits / 8));
}

