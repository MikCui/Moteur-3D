
// Classe pour la gestion des lumières

#pragma once
#include "VariableGlobale.h"

class CLumiere
{
public:

	D3DLIGHT9 m_Lumiere;	// Paramètres de la lumière

	CLumiere();
	~CLumiere();

	D3DLIGHT9* GetLumiere(){return &m_Lumiere;}
	void SetDirectionnelle(D3DXVECTOR3* Direction);
	void SetPonctuelle(D3DXVECTOR3* Position, float Range, float Attenuation0 = 1.0f, float Attenuation1 = 0.0f, float Attenuation2 = 0.0f);
	void SetSpot(D3DXVECTOR3* Position, D3DXVECTOR3* Direction, float Range, float ConeInterieur = D3DX_PI/6.0f, float ConeExterieur = D3DX_PI/4.0f, float Attenuation0 = 1.0f, float Attenuation1 = 0.0f, float Attenuation2 = 0.0f, float Facteur = 1.0f);
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
};
