#include "Lumiere.h"

CLumiere::CLumiere()
{
	ZeroMemory(&m_Lumiere, sizeof(D3DLIGHT9));
	D3DCOLORVALUE Couleur = {1.0f, 1.0f, 1.0f, 1.0f};
	m_Lumiere.Type = D3DLIGHT_DIRECTIONAL;
	m_Lumiere.Ambient = Couleur;
	m_Lumiere.Diffuse = Couleur;
	m_Lumiere.Specular = Couleur;
	m_Lumiere.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
}

CLumiere::~CLumiere()
{
}

// Lumière Directionnelle
void CLumiere::SetDirectionnelle(D3DXVECTOR3* Direction)
{
	m_Lumiere.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Lumiere.Direction, Direction);
	m_Lumiere.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// Lumière Ponctuelle
void CLumiere::SetPonctuelle(D3DXVECTOR3* Position, float Range, float Attenuation0, float Attenuation1, float Attenuation2)
{
	m_Lumiere.Type = D3DLIGHT_POINT;
	m_Lumiere.Position = *Position;
	m_Lumiere.Range = Range;
	m_Lumiere.Attenuation0 = Attenuation0;
	m_Lumiere.Attenuation1 = Attenuation1;
	m_Lumiere.Attenuation2 = Attenuation2;
}

// Spot lumineux 
void CLumiere::SetSpot(D3DXVECTOR3* Position, D3DXVECTOR3* Direction, float Range, float ConeInterieur, float ConeExterieur, float Attenuation0, float Attenuation1, float Attenuation2, float Facteur)
{
	m_Lumiere.Type = D3DLIGHT_SPOT;
	m_Lumiere.Position = *Position;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Lumiere.Direction, Direction);
	m_Lumiere.Range = Range;
	m_Lumiere.Phi = ConeInterieur;
	m_Lumiere.Theta = ConeExterieur;
	m_Lumiere.Attenuation0 = Attenuation0;
	m_Lumiere.Attenuation1 = Attenuation1;
	m_Lumiere.Attenuation2 = Attenuation2;
	m_Lumiere.Falloff = Facteur;
}

void CLumiere::SetAmbiante(D3DCOLOR Couleur)
{
	m_Lumiere.Ambient.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Lumiere.Ambient.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Lumiere.Ambient.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Lumiere.Ambient.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CLumiere::SetAmbiante(D3DCOLORVALUE* Couleur)
{
	m_Lumiere.Ambient = *Couleur;
}

void CLumiere::SetAmbiante(float r, float g, float b, float a)
{
	m_Lumiere.Ambient.r = r;
    m_Lumiere.Ambient.g = g;
    m_Lumiere.Ambient.b = b;
	m_Lumiere.Ambient.a = a;
}

void CLumiere::SetDiffuse(D3DCOLOR Couleur)
{
	m_Lumiere.Diffuse.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Lumiere.Diffuse.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Lumiere.Diffuse.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Lumiere.Diffuse.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CLumiere::SetDiffuse(D3DCOLORVALUE* Couleur)
{
	m_Lumiere.Diffuse = *Couleur;
}

void CLumiere::SetDiffuse(float r, float g, float b, float a)
{
	m_Lumiere.Diffuse.r = r;
    m_Lumiere.Diffuse.g = g;
    m_Lumiere.Diffuse.b = b;
	m_Lumiere.Diffuse.a = a;
}

void CLumiere::SetAmbianteDiffuse(D3DCOLOR Couleur)
{
	SetDiffuse(Couleur);
	SetAmbiante(Couleur);
}

void CLumiere::SetAmbianteDiffuse(D3DCOLORVALUE* Couleur)
{
	SetDiffuse(Couleur);
	SetAmbiante(Couleur);
}

void CLumiere::SetAmbianteDiffuse(float r, float g, float b, float a)
{
	SetDiffuse(r, g, b, a);
	SetAmbiante(r, g, b, a);
}

void CLumiere::SetSpeculaire(D3DCOLOR Couleur)
{
	m_Lumiere.Specular.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Lumiere.Specular.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Lumiere.Specular.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Lumiere.Specular.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CLumiere::SetSpeculaire(D3DCOLORVALUE* Couleur)
{
	m_Lumiere.Specular = *Couleur;
}

void CLumiere::SetSpeculaire(float r, float g, float b, float a)
{
	m_Lumiere.Specular.r = r;
    m_Lumiere.Specular.g = g;
    m_Lumiere.Specular.b = b;
	m_Lumiere.Specular.a = a;
}
