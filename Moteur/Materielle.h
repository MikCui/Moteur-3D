
// Classe pour la gestion des Matériaux

#pragma once
#include "VariableGlobale.h"

#define EMERAUDE		0
#define JADE			1
#define OBSIDIENNE		2
#define PERLE			3
#define RUBIS			4
#define TURQUOISE		5
#define LAITON			6
#define BRONZE			7
#define CHROME			8
#define CUIVRE			9
#define OR				10
#define ARGENT			11

class CMaterielle
{
public:
	
	D3DMATERIAL9 m_Materielle;	// Paramètres du matétrielle

	CMaterielle();
	~CMaterielle();

	D3DMATERIAL9* GetMaterielle(){return &m_Materielle;}
	void SetAmbiante(D3DCOLOR Couleur);
	void SetAmbiante(D3DCOLORVALUE* Couleur);
	void SetAmbiante(float r, float g, float b, float a = 1.0f);
	void SetDiffuse(D3DCOLOR Couleur);
	void SetDiffuse(D3DCOLORVALUE* Couleur);
	void SetDiffuse(float r, float g, float b, float a = 1.0f);
	void SetAmbianteDiffuse(D3DCOLOR Couleur);
	void SetAmbianteDiffuse(D3DCOLORVALUE* Couleur);
	void SetAmbianteDiffuse(float r, float g, float b, float a = 1.0f);
	void SetSpeculaire(D3DCOLOR Couleur);
	void SetSpeculaire(D3DCOLORVALUE* Couleur);
	void SetSpeculaire(float r, float g, float b, float a = 1.0f);
	void SetPuissance(float Puissance);
	void SetEmissive(D3DCOLOR Couleur);
	void SetEmissive(D3DCOLORVALUE* Couleur);
	void SetEmissive(float r, float g, float b, float a = 1.0f);
	void SetMaterielle(DWORD Type);
};
