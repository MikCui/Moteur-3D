#include "VariableGlobale.h"
#include "Moteur.h"

CConsole Console;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CMoteur* pMoteur = new CMoteur;
	if(!pMoteur->Creation(hInstance, 800, 600, false))
	{
		MessageBox(NULL, "Erreur à la création de l'application", "Erreur", MB_OK);
		return -1;
	}
	return pMoteur->Demarrer();
}
