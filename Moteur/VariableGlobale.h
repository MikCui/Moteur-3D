#define WIN32_LEAN_AND_MEAN

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
#endif

#include <Windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <d3dx9.h>
#include <dinput.h>
#include "..\Outils\Console.h"
#include "..\Outils\MesureTemps.h"
#include "..\Outils\ImageTGA.h"
#include "..\Resource.h"

extern CConsole Console;

#define D3DCOLOR_R(Couleur)  ((Couleur >> 16) & 0xFF)
#define D3DCOLOR_G(Couleur)  ((Couleur >> 8) & 0xFF)
#define D3DCOLOR_B(Couleur)  (Couleur & 0xFF)
#define D3DCOLOR_A(Couleur)  ((Couleur >> 24) & 0xFF)
#define D3DCOLOR_Gris(Couleur) ((D3DCOLOR_R(Couleur)+D3DCOLOR_G(Couleur)+D3DCOLOR_B(Couleur))/3)

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }