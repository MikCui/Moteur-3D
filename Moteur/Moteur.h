
// Classe pour la gestion de l'application

#pragma once

#include "VariableGlobale.h"
#include "Moteur3d.h"
#include "Peripherique.h"
#include "Scene.h"

static char Titre[] = "Projet : Terrain 3D et LOD";
static char WindowClass[] = "3D Windows";

class CMoteur3D;
class CPeripherique;
class CScene;

class CMoteur
{
public:

	int			m_LargeurFenetre;			// Largeur de la fen�tre
	int			m_HauteurFenetre;			// Hauteur de la fen�tre
	bool		m_bActive;					// Si application au premier plan
	bool		m_bActiveAquisition;		// Si aquisition des ph�riph�riques activ�s
	bool		m_bDeviceLost;				// Si on a perdu le ph�riph�rique
	DWORD		m_dwWindowStyle;			// Style de la fen�tre
	HWND		m_hWnd;						// Pointeur sur le fen�tre
	HINSTANCE	m_hInstance;				// Pointeur sur l'application
	bool		m_bMaximized;				// Si l'application est maximiz�
	bool		m_bPleinEcran;				// Si l'application est en plein �cran

	CScene*		m_pSceneConfig;				// Pointeur sur la Sc�ne � afficher
	static CMoteur*     m_pMoteur;			// Pointeur sur le Moteur
	CMoteur3D*          m_pMoteur3D;		// Pointeur sur le Moteur 3D
	CPeripherique*		m_pPeripherique;	// Pointeur sur le P�riph�rique d'entr�e

	RECT m_rcWindowMin;						// Taille minimum de la fen�tre
	RECT m_rcClient;						// Taille de la zone cliente en mode windows
	RECT m_rcWindow;						// Taille de la fen�tre

	CMoteur();
	~CMoteur();

	int Demarrer();

	bool     Creation(HINSTANCE hInstance, int Largeur = 640, int Hauteur = 480, bool PleinEcran = false);
	void	 Destruction();
	bool     CreationFenetre(HINSTANCE hInstance, int Largeur = 640, int Hauteur = 480, bool PleinEcran = false);
	LRESULT  MgsProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static   LRESULT CALLBACK WndProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool     RedimFenetre();
	bool	 ChangementFenetre();

	CMoteur3D*          GetMoteur3D(){return m_pMoteur3D;}
	CPeripherique*		GetPeripherique(){return m_pPeripherique;}
	HWND                GetFenetre(){return m_hWnd;}
	HINSTANCE			GetInstance(){return m_hInstance;}
	bool				GetActive(){return m_bActive;}
	bool				GetEtatPeripherique();
	void				SetDeviceLost(bool bDeviceLost){m_bDeviceLost = bDeviceLost;}
	bool				GetDeviceLost();
};