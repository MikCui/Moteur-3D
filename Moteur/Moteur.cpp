#include "Moteur.h"
#include "..\Scene\SceneConfig.h"

CMoteur* CMoteur::m_pMoteur = NULL;

CMoteur::CMoteur()
{
	m_LargeurFenetre = 0;
	m_HauteurFenetre = 0;
	m_bDeviceLost = false;
	m_hWnd = NULL;
	m_hInstance = NULL;
	m_dwWindowStyle = NULL;
	m_bActive = true;
	m_bActiveAquisition = true;
	m_pMoteur = this;
	m_bMaximized = false;
	m_bPleinEcran = false;

	m_pSceneConfig = NULL;
	m_pMoteur3D = NULL;
	m_pPeripherique = NULL;
	
	m_rcWindowMin.left = 0;
	m_rcWindowMin.top = 0;
	m_rcWindowMin.right = 320;
	m_rcWindowMin.bottom = 240;
}

CMoteur::~CMoteur()
{
	Console << "Deconstructeur CMoteur" << endl;
}

// cr�ation du Moteur
bool CMoteur::Creation(HINSTANCE hInstance, int Largeur, int Hauteur, bool PleinEcran)
{
	m_hInstance = hInstance;
	m_LargeurFenetre = Largeur;
	m_HauteurFenetre = Hauteur;
	m_bPleinEcran = PleinEcran;

	// Initialisation des controles de Windows (pour les bo�tes de dialogue)
	InitCommonControls();
	
	// Instancation des diff�rents objets
	m_pMoteur3D = new CMoteur3D(this);
	m_pPeripherique = new CPeripherique(this);
	m_pSceneConfig = new CSceneConfig(this);
	if(!(m_pMoteur3D && m_pPeripherique && m_pSceneConfig)) return false;
	// Cr�ation de la fen�tre de l'application
	if(!CreationFenetre(m_hInstance, Largeur, Hauteur, PleinEcran)) return false;
	// Cr�ation du moteur 3D de l'appication
	if(!m_pMoteur3D->Creation(m_hWnd, Largeur, Hauteur, PleinEcran)) return false;
	// Cr�ation de la sc�ne � afficher
	m_pSceneConfig->SetRatioFenetre((float)m_LargeurFenetre/(float)m_HauteurFenetre);
	if(!m_pSceneConfig->Creation()) return false;
	// Cr�ation du p�riph�rique d'aquisition des entr�es
	if(!m_pPeripherique->Creation()) return false;
	// On affiche la fen�tre
	ShowWindow(m_hWnd, SW_SHOW);
	// On lance l'aquisition des p�riph�rique d'entr�es
	m_pPeripherique->Acquisition();
	return true;
}

// Destruction du Moteur 
void CMoteur::Destruction()
{
	// Destruction de la sc�ne
	m_pSceneConfig->DestructionFinal();
	// Destruction des p�riph�riques d'aquisition
	m_pPeripherique->Destruction();
	// Destruction du Moteur3D
	m_pMoteur3D->Destruction();
}

// Cr�ation de la fen�tre
bool CMoteur::CreationFenetre(HINSTANCE hInstance, int Largeur, int Hauteur, bool PleinEcran)
{
	Console.Printf("Creation Fenetre : %i, %i, %u\n", Largeur, Hauteur, PleinEcran);
	// Ic�ne de l'application
	HICON hIcone = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1));
	DWORD dwWindowStyle = 0;

	// Param�tre de la classe windows
	WNDCLASS wndClass;

	wndClass.style         = NULL;						// Style de la fen�tre
    wndClass.lpfnWndProc   = WndProcess;				// Pointeur sur la fonction qui traite les m�ssages
    wndClass.cbClsExtra    = 0;							// Octect suppl�mentaire disponible pour la classe
    wndClass.cbWndExtra    = 0;							// Octect suppl�mentaire disponible pour la fen�tre
    wndClass.hInstance     = hInstance;					// Handle d'instance de l'application
	wndClass.hIcon         = hIcone;					// Icone de le fen�tre
    wndClass.hCursor       = LoadCursor(0, IDC_ARROW);	// Curseur de la fen�tre
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);	// Couleur de fond
    wndClass.lpszMenuName  = NULL;						// Nom du menu
    wndClass.lpszClassName = WindowClass;				// Nom de la classe

    // Enregistre la classe
	if (!RegisterClass( &wndClass ))						
	{
		Console << "Ce programme requi�re Win32!" << endl;
		return false;
	}

	// Taille la fen�tre
	RECT rc = {0, 0, Largeur, Hauteur};

	// Style de la fen�tre en mode fen�tr� WS_OVERLAPPEDWINDOW
	m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	// Style de la fen�tre en fonction du mode choisi
	if(PleinEcran)
	{
		dwWindowStyle = WS_POPUP | WS_SYSMENU;
	}
	else
	{
		dwWindowStyle = m_dwWindowStyle;	
	}

	// On ajuste la taille de la fen�tre en fonction du style
	AdjustWindowRect(&rc, m_dwWindowStyle, false);

	// On ajuste le rectangle minimum en fonction du style de la fen�tre
	AdjustWindowRect(&m_rcWindowMin, m_dwWindowStyle, false);

	// Cr�ation de la fen�tre
	m_hWnd = CreateWindow
		(
			WindowClass,		// Nom de la classe enregistr�e
			Titre,				// Nom de la fen�tre
			dwWindowStyle,		// Style de la fen�tre
			CW_USEDEFAULT,		// Position horizontale de la fen�tre
			CW_USEDEFAULT,		// Position verticale de la fen�tre
			rc.right-rc.left,	// Largeur de la fen�tre
			rc.bottom-rc.top,	// Hauteur de la fen�tre
			NULL,				// Gestionnaire de la fen�tre parente
			NULL,				// Gestionnaire de menu
			hInstance,			// Gestionnaire de l'instance d'application
			NULL				// Param�tre de cr�ation de la fen�tre
		);

	// On v�rifie si la fen�tre est bien cr�e
	if(!m_hWnd)
	{
		Console << "Impossible de creer la fenetre!" << endl;
		return false;
	}

	if(PleinEcran)
	{
		// On sauvegarde la taille de la zone cliente de la fen�tre
		m_rcClient.bottom = Hauteur;
		m_rcClient.left = 0;
		m_rcClient.right = Largeur;
		m_rcClient.top = 0;
		// On r�cup�re les dimentions de la fen�tre
		GetWindowRect(m_hWnd, &m_rcWindow);
	}
	else
	{
		// On r�cup�re taille de la zone cliente de la fen�tre
		GetClientRect(m_hWnd, &m_rcClient);
		// On r�cup�re les dimentions de la fen�tre
		GetWindowRect(m_hWnd, &m_rcWindow);
	}
	return true;
}

// fonction appel�e pour traiter les messages de windows
LRESULT CALLBACK CMoteur::WndProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMoteur->MgsProcess(hWnd, uMsg, wParam, lParam);
}

// fonction pour traiter les messages de windows
LRESULT CMoteur::MgsProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
    {
		// Message : dessiner le contenu de la fen�tre
        case WM_PAINT:
			m_pSceneConfig->Rendu();
			break;
		
		// Message : changement d'�tat de la fen�tre
		case WM_ACTIVATE:
			if(WA_INACTIVE == LOWORD(wParam))
			{
				m_bActiveAquisition = false;
				if(m_pPeripherique)m_pPeripherique->DeAcquisition();
			}
			else
			{
				m_bActive = true;
			}
			if(HIWORD(wParam) != 0)
			{
				m_bActive = false;
			}
			break;

		// Message : Bouton gauche de la souris enfonc�  
		case WM_LBUTTONDOWN:
			m_bActiveAquisition = true;
			if(m_pPeripherique)m_pPeripherique->Acquisition();
			break;

		// Message : Modification de la fen�tre
		case WM_SIZE:
			if(!m_bPleinEcran)
				m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);

			if(SIZE_MINIMIZED == wParam)
			{
				m_bActive = false;
			}
			else if(SIZE_MAXIMIZED == wParam)
            {
				RedimFenetre();
				m_bMaximized = true;
			}
			else if(SIZE_RESTORED == wParam)
			{
				if(m_bMaximized)
				{
					RedimFenetre();
					m_bMaximized = false;
				}
			}

			break;

		// Message : Fin de redimensionnement de la fen�tre
		case WM_EXITSIZEMOVE:
			RedimFenetre();
			break;

		// Message : demande la taille minimum de la fen�tre
		case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = m_rcWindowMin.right  - m_rcWindowMin.left;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = m_rcWindowMin.bottom - m_rcWindowMin.top;
            break;

		// Message : destruction de la fen�tre
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		// Message : fermeture de la fen�tre
        case WM_CLOSE:
			Destruction();
            DestroyWindow(hWnd);
            m_hWnd = NULL;
            return 0;

		// Message : touche clavier enfonc�e
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_SPACE:
					m_bActiveAquisition = false;
					m_pPeripherique->DeAcquisition();
					return 0;

				case VK_ESCAPE :
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
			}
			break;

		// Message : Commande pour l'application
		case WM_COMMAND:
            switch(LOWORD(wParam))
            {
				case IDM_PLEINECRAN:
					ChangementFenetre();
					return 0;
			}
			break;

		// Blocage de certain message
		case WM_SYSCOMMAND:
            switch(wParam)
            {
                case SC_KEYMENU:
                case SC_MONITORPOWER:
                        return 0;
            }
            break;
    }

	// traitement des autre messages par default
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

// Remimentionnement de la fen�tre
bool CMoteur::RedimFenetre()
{
	// On r�cup�re la nouvelle taille de la zone cliente de la fen�tre
	GetClientRect(m_hWnd, &m_rcClient);
	// On r�cup�re les nouvelles dimentions de la fen�tre
	GetWindowRect(m_hWnd, &m_rcWindow);

	int LargeurFenetre = m_rcClient.right - m_rcClient.left;
	int HauteurFenetre = m_rcClient.bottom - m_rcClient.top;

	// On test si la taille de la fen�tre � chang�
	if(LargeurFenetre != m_LargeurFenetre || HauteurFenetre != m_HauteurFenetre)
	{
		m_LargeurFenetre = LargeurFenetre;
		m_HauteurFenetre = HauteurFenetre;

		// Destruction des objets de la sc�ne
		m_pSceneConfig->DestructionObjet();

		// Redimentionnement de la zone cliente de Direct3D
		if(m_pMoteur3D->Redimentionnement(LargeurFenetre, HauteurFenetre))
		{
			m_pSceneConfig->SetRatioFenetre((float)m_LargeurFenetre/(float)m_HauteurFenetre);
			m_pSceneConfig->CreationObjet();
			return true;
		}
		return false;
	}
	return true;
}

// Fonction pour passer du mode Windows/Plein �cran
bool CMoteur::ChangementFenetre()
{
	m_bPleinEcran = !m_bPleinEcran;

	// On teste dans quel mode on veux passer
	if(m_bPleinEcran)
	{
		// On applique le style de fen�tre plein �cran
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE);
	}
	else
	{
		// On applique le style de fen�tre windows
		SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle);
		m_LargeurFenetre = m_rcClient.right - m_rcClient.left;
		m_HauteurFenetre = m_rcClient.bottom - m_rcClient.top;
	}

	// Destrcution des objets de la sc�ne
	m_pSceneConfig->DestructionObjet();

	// changement de mode de la zone cliente de Direct3D
	if(m_pMoteur3D->SetModeFenetre(m_LargeurFenetre, m_HauteurFenetre ,m_bPleinEcran))
	{
		m_pSceneConfig->SetRatioFenetre((float)m_LargeurFenetre/(float)m_HauteurFenetre);
		m_pSceneConfig->CreationObjet();

		// Si on revient en mode windows, on remet la fen�tre dans son �tat  
		if(!m_bPleinEcran)
		{
			SetWindowPos(m_hWnd, 
						 HWND_NOTOPMOST, 
						 m_rcWindow.left, 
						 m_rcWindow.top, 
						 m_rcWindow.right - m_rcWindow.left, 
						 m_rcWindow.bottom - m_rcWindow.top, 
						 SWP_SHOWWINDOW);
		}
		return true;
	}
	return false;
}

// Mise � jours des stats des p�riph�riques
bool CMoteur::GetEtatPeripherique()
{
	return m_pPeripherique->GetEtatPeripherique(m_bActiveAquisition);
}

// Demande si le p�riph�rique 3D est perdu
bool CMoteur::GetDeviceLost()
{
	if(!m_bDeviceLost)
		return false;

	// on � perdu le p�riph�rique 3D, on d�truit les objets de la sc�ne
	m_pSceneConfig->DestructionObjet();

	// on appel la fonction DeviceLost() pour essayer de r�cup�rer le p�riph�rique 3D
	m_bDeviceLost = !m_pMoteur3D->RAZPeripherique3D();

	// si on � r�cup�rer le p�riph�rique 3D, on cr�e les objets de la sc�ne
	if(!m_bDeviceLost)
		m_pSceneConfig->CreationObjet();

	return m_bDeviceLost;
}

// D�mmarage du moteur
int CMoteur::Demarrer()
{
	// Chargement des raccourcis clavier
    HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_ACCELERATOR));

	bool bMessageRecu;
    MSG  Msg;

	// Initialisation de la variable des Messages
	ZeroMemory(&Msg, sizeof(Msg));

	// Boucle de messages principale
	while(Msg.message != WM_QUIT)
    {
		// Si l'application est active on traite le message et on effectue le rendu de la sc�ne
		// Sinon on attend la r�ception d'un message avec GetMessage()
        if(m_bActive)
            bMessageRecu = (PeekMessage( &Msg, NULL, 0U, 0U, PM_REMOVE) != 0);
        else
            bMessageRecu = (GetMessage( &Msg, NULL, 0U, 0U ) != 0);

		// Si on a re�u un message on le traduit et le traite
        if(bMessageRecu)
        {
			TranslateAccelerator(m_hWnd, hAccel, &Msg);
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        else
        {
			// Si on � perdu le p�riph�rique on attend 100 millisecondes
            if(m_bDeviceLost)
            {
                Sleep(100);
            }
            // Rendu de la sc�ne
            if(m_bActive)
            {
				m_pSceneConfig->Rendu();
            }
        }
    }

	// Destrcution des raccourcis clavier
	if( hAccel != NULL )
        DestroyAcceleratorTable(hAccel);

	// D�chargement de la classe
	UnregisterClass("3D Windows", m_hInstance);
	return (int)Msg.wParam;
}
