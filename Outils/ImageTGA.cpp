#include "ImageTGA.h"
#include "Console.h"

CConsole cout;

CImageTGA::CImageTGA()
{
	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));
	m_Donnee = NULL;
	m_bGris = false;
}

CImageTGA::~CImageTGA()
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}
}

// Constructeur par recopie
CImageTGA::CImageTGA(const CImageTGA& Image)
{
	m_Donnee = NULL;
	m_bGris = Image.m_bGris;
	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.ImageWidth, Image.m_Entete.ImageHeight, m_bGris))
		{
			DWORD TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * (m_Entete.PixelDepth / 8);
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
}


// Operateur d'affectation entre images
CImageTGA& CImageTGA::operator=(const CImageTGA& Image)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}
	m_bGris = Image.m_bGris;
	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.ImageWidth, Image.m_Entete.ImageHeight, m_bGris))
		{
			DWORD TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * (m_Entete.PixelDepth / 8);
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
	return *this;
}

// Création d'une image vide
bool CImageTGA::Creer(unsigned short Largeur, unsigned short Hauteur, bool bGris)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}

	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));

	// On initialise la l'entête en fonction du type d'image
	m_bGris = bGris;
	if(m_bGris)
	{
		m_Entete.ImageType = 3;
		m_Entete.PixelDepth = 8;
	}
	else
	{
		m_Entete.ImageType = 2;
		m_Entete.PixelDepth = 32;
	}

	// Calcule de la taille des données
	DWORD TailleDonnees = Largeur * Hauteur * (m_Entete.PixelDepth / 8);
	m_Entete.ImageWidth = Largeur;
	m_Entete.ImageHeight = Hauteur;
	// Allocation du tableau de données
	m_Donnee = new BYTE[TailleDonnees];
	return m_Donnee != NULL;
}

// Chargement d'une image à partir d'un fichier
bool CImageTGA::Charger(char* NomFichier)
{
	std::ifstream Fichier;

	// On ouvre le ficher en mode binaire
	Fichier.open(NomFichier, std::ios::binary);

	// On vérifie qu'il est bien ouvert
	if(!Fichier.is_open())
	{
		Fichier.close();
		cout << "Impossible d'ouvire le fichier : " << NomFichier << endl;
		return false;
	}

	// Lecture de l'entête du fichier
	Fichier.read((char*)&m_Entete, sizeof(m_Entete));

	// On vérifie que l'on n'a pas atteins la fin du fichier
	if(Fichier.eof())
	{
		Fichier.close();
		cout << "Erreur a la lecture de l'entete du fichier : " << NomFichier << endl;
		return false;
	}

	// On se positionne sur le début des données de l'image
	Fichier.seekg(sizeof(m_Entete) + m_Entete.ColorMapLength + m_Entete.IDLength, std::ios::beg);

	// Calcule de la taille des données
	DWORD TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * (m_Entete.PixelDepth / 8);

	// Si l'image est en mode RGBA 32bits
	if(m_Entete.ImageType == 2 && m_Entete.PixelDepth == 32)
	{
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
	}
	else
	{
		// Si l'image est en mode niveaux de gris 8bits
		if(m_Entete.ImageType == 3 && m_Entete.PixelDepth == 8)
		{
			m_Donnee = new BYTE[TailleDonnees];
			if(m_Donnee != NULL)
			{
				// Chargement des données à partir du fichier
				m_bGris = true;
				Fichier.read((char*)m_Donnee, TailleDonnees);
			}
			else
			{
				Fichier.close();
				cout << "Impossible de creer le tableau de donnees, memoire insuffisante" << endl;
				return false;
			}
		}
		else
		{
			Fichier.close();
			cout << "Format non pris en charge pour le fichier : " << NomFichier << endl;
			return false;
		}
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
bool CImageTGA::Enregister(char* NomFichier)
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

	// Ecriture de l'entête
	Fichier.write((char*)&m_Entete, sizeof(m_Entete));
	// Positionnement sur le début des données de l'image
	Fichier.seekp(sizeof(m_Entete) + m_Entete.ColorMapLength + m_Entete.IDLength, std::ios::beg);
	DWORD TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * (m_Entete.PixelDepth / 8);
	// Ecriture des données de l'image
	Fichier.write((char*)m_Donnee, TailleDonnees);
	// Fermeture du fichier
	Fichier.close();
	return true;
}

// Obtenire la couleur d'un pixel
COLORREF CImageTGA::GetPixel(int X, int Y)
{
	if(X < 0 || Y < 0 || X >= m_Entete.ImageWidth || Y >= m_Entete.ImageHeight)
		return 0;
	if(m_bGris)
		return m_Donnee[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X];
	else
		return ((COLORREF*)m_Donnee)[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X];
}

// Modifier la couleur d'un pixel
void CImageTGA::SetPixel(int X, int Y, COLORREF Couleur)
{
	if(X < 0 || Y < 0 || X >= m_Entete.ImageWidth || Y >= m_Entete.ImageHeight)
		return;
	if(m_bGris)
		m_Donnee[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X] = (BYTE)(Couleur & 0xFF);
	else
		((COLORREF*)m_Donnee)[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X] = Couleur;
}