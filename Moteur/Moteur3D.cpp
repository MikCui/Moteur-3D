#include "Moteur3D.h"

CMoteur3D::CMoteur3D()
{
	Console << "Constructeur CMoteur3D()" << endl;
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

CMoteur3D::CMoteur3D(CMoteur* pMoteur)
{
	m_pMoteur = pMoteur;
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

CMoteur3D::~CMoteur3D()
{
	Console << "Deconstructeur CMoteur3D" << endl;
}

// création du Moteur3D (DirectX 9.0c)
bool CMoteur3D::Creation(HWND hWnd, int Largeur, int Hauteur, bool bPleinEcran)
{
	D3DDISPLAYMODE d3ddm;
	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;

	// création de l'objet Direct3D
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		Console << "Impossible de crée l'objet Direct3D" << endl;
		return false;
	}

	// on récupère le mode d'affichage de windows
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	// on récupère des informations sur la carte graphique
	m_pD3D->GetAdapterIdentifier(0, NULL, &d3dAdapterIdentifier);
	strcpy(m_Description, d3dAdapterIdentifier.Description);
	Console << "Description de la carte 3D : " << m_Description << endl;

	// initialisation des paramètres du buffer image
    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
    m_d3dpp.Windowed = !bPleinEcran;								// Si l'application est en mode Windows ou Plein écran
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// Mode d'échange entre le FontBuffer et le BackBuffer
    m_d3dpp.BackBufferFormat = d3ddm.Format;						// Format du pixel du BackBuffer
    m_d3dpp.EnableAutoDepthStencil = TRUE;							// Activation du Depth et Stencil Buffer
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;					// Format du Depth et Stencil
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// Type de synchonisation de l'échange des Buffers
	m_d3dpp.BackBufferWidth = Largeur;								// Largeur du Buffer
	m_d3dpp.BackBufferHeight = Hauteur;								// Hauteru du Buffer

	// on crée le device en mode HARDWARE (calcule des vertex par le GPU)
	if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &m_d3dpp, &m_pD3DDevice) != D3D_OK)
		// si la création en mode HARDWARE à échoué, on crée le device en mode SOFTWARE (calcule des vertex par le CPU)
		if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pD3DDevice) != D3D_OK)
		{
			Console << "Impossible d'initialiser le peripherique 3D" << endl;
			return false;
		}

	return true;
}

// Destrction du Moteur3D
void CMoteur3D::Destruction()
{
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D);
}

// Fonction pour le redimensionnement de la fenêtre
bool CMoteur3D::Redimentionnement(int Largeur, int Hauteur)
{
	m_d3dpp.BackBufferWidth  = Largeur;
	m_d3dpp.BackBufferHeight = Hauteur;

	Console << "redimensionner la fenetre : " << Largeur << ", " << Hauteur << endl;

	// on réinitialise le périphérique avec les nouveaux paramètres
	if(FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
	{
		Console << "Impoissible de redimensionner la fenetre" << endl;
		return false;
	}
	return true;
}

// Fonction pour passer du mode Windows/Plein écran
bool CMoteur3D::SetModeFenetre(int &Largeur, int &Hauteur, bool bPleinEcran)
{
	// On teste dans quel mode on veux passer
	if(bPleinEcran)
	{
		D3DDISPLAYMODE d3ddm;

		// on récupère le mode d'affichage de windows
		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		// on initialise les dimentions de l'image
		Largeur = d3ddm.Width;
		Hauteur = d3ddm.Height;
		m_d3dpp.BackBufferWidth  = Largeur;
		m_d3dpp.BackBufferHeight = Hauteur;
		m_d3dpp.Windowed = !bPleinEcran;
	}
	else
	{
		// on initialise les dimentions de l'image
		m_d3dpp.BackBufferWidth  = Largeur;
		m_d3dpp.BackBufferHeight = Hauteur;
		m_d3dpp.Windowed = !bPleinEcran;
	}

	// on réinitialise le périphérique avec les nouveaux paramètres
	if(FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
	{
		Console << "Impoissible de changer le mode de la fenetre" << endl;
		return false;
	}
	return true;
}

// Fonction pour récuperer le périphérique 3D
bool CMoteur3D::RAZPeripherique3D()
{
	HRESULT hr;

	// on test l'état du périphérique 3D
    if(FAILED(hr = m_pD3DDevice->TestCooperativeLevel()))
    {
		// si le périphérique 3D est toujours perdu on ne fait pas le rendu
        if(D3DERR_DEVICELOST == hr)
		{
            return false;
		}
		// on test si le périphérique 3D est près à être réinitialisé
		if(D3DERR_DEVICENOTRESET == hr)
		{
			// on réinitialise le périphérique 3D
			if(FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
			{
				Console << "Impoissible de réinitialiser le peripherique 3D" << endl;
				return false;
			}
			else
			{
				Console << "Recuperation du peripherique 3D" << endl;
				return true;
			}
		}
		Console << "ERREUR du peripherique 3D" << endl;
	}
	return false;
}