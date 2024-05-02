
// Classe pour la gestion des images TGA (32bits et 8bits niveaux de gris)

#pragma once
#include <fstream>
#include <Windows.h>

class CImageTGA
{
public:
	CImageTGA();
	CImageTGA(const CImageTGA& Image);
	~CImageTGA();

	// configuration du compilateur pour alligner la structure sur un octect
	#pragma pack(push, 1)
	// Structure d'un Entête de fichier TGA
	struct EnteteTGA
	{
		unsigned char  IDLength;				// Taille du champ de description de l'image
		unsigned char  ColorMapType;			// A 1 si il y a une palette de couleur
		unsigned char  ImageType;				// Type d'image
		unsigned short ColorMapFirstEntryIndex;	// Index de la première couleur dans la palette
		unsigned short ColorMapLength;			// Nombre de couleur dans la palette
		unsigned char  ColorMapEntrySize;		// Nombre de bits par pixel de la palette
		unsigned short X_OriginOfImage;			// Coordonnée absolue de l'image
		unsigned short Y_OriginOfImage;			// Coordonnée absolue de l'image
		unsigned short ImageWidth;				// Largeur de l'image
		unsigned short ImageHeight;				// Hauteur de l'image
		unsigned char  PixelDepth;				// Nombre de bits par pixel
		unsigned char  ImageDescriptor;			// Description de l'image
	};
	#pragma pack(pop)

	
	EnteteTGA m_Entete;							// Entête du fichier
	BYTE* m_Donnee;								// Pointeur sur les données de l'image
	bool m_bGris;								// Boolean pour savoir si l'image est en niveau de gris 

	bool Charger(char* NomFichier);
	bool Creer(unsigned short Largeur, unsigned short Hauteur, bool bGris = false);
	bool Enregister(char* NomFichier);

	BYTE*			GetDonnees(){return m_Donnee;}
	unsigned short	GetLargeur(){return m_Entete.ImageWidth;}
	unsigned short	GetHauteur(){return m_Entete.ImageHeight;}
	COLORREF		GetPixel(int X, int Y);
	void			SetPixel(int X, int Y, COLORREF Couleur);
	bool			IsVide(){return m_Donnee == NULL;}
	bool			IsGris(){return m_bGris;}
	EnteteTGA*		GetEntete(){return &m_Entete;}

	CImageTGA& operator=(const CImageTGA& Image);
};
