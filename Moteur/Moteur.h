
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

	int			m_LargeurFenetre;			// Largeur de la fenêtre
	int			m_HauteurFenetre;			// Hauteur de la fenêtre
	bool		m_bActive;					// Si application au premier plan
	bool		m_bActiveAquisition;		// Si aquisition des phériphériques activés
	bool		m_bDeviceLost;				// Si on a perdu le phériphérique
	DWORD		m_dwWindowStyle;			// Style de la fenêtre
	HWND		m_hWnd;						// Pointeur sur le fenêtre
	HINSTANCE	m_hInstance;				// Pointeur sur l'application
	bool		m_bMaximized;				// Si l'application est maximizé
	bool		m_bPleinEcran;				// Si l'application est en plein écran

	CScene*		m_pSceneConfig;				// Pointeur sur la Scène à afficher
	static CMoteur*     m_pMoteur;			// Pointeur sur le Moteur
	CMoteur3D*          m_pMoteur3D;		// Pointeur sur le Moteur 3D
	CPeripherique*		m_pPeripherique;	// Pointeur sur le Périphérique d'entrée

	RECT m_rcWindowMin;						// Taille minimum de la fenêtre
	RECT m_rcClient;						// Taille de la zone cliente en mode windows
	RECT m_rcWindow;						// Taille de la fenêtre

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