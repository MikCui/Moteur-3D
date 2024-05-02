#include "Materielle.h"

float TabMaterielle[][10] = 
{
	{0.0215f  ,0.1745f  ,0.0215f  ,0.07568f ,0.61424f ,0.07568f ,0.633f   ,0.727811f,0.633f   ,0.6f},
	{0.135f   ,0.2225f  ,0.1575f  ,0.54f    ,0.89f    ,0.63f    ,0.316228f,0.316228f,0.316228f,0.1f},
	{0.05375f ,0.05f    ,0.06625f ,0.18275f ,0.17f    ,0.22525f ,0.332741f,0.328634f,0.346435f,0.3f},
	{0.25f    ,0.20725f ,0.20725f ,1.0f     ,0.829f   ,0.829f   ,0.296648f,0.296648f,0.296648f,0.088f},
	{0.1745f  ,0.01175f ,0.01175f ,0.61424f ,0.04136f ,0.04136f ,0.727811f,0.626959f,0.626959f,0.6f},
	{0.1f     ,0.18725f ,0.1745f  ,0.396f   ,0.74151f ,0.69102f ,0.297254f,0.30829f ,0.306678f,0.1f},
	{0.329412f,0.223529f,0.027451f,0.780392f,0.568627f,0.113725f,0.992157f,0.941176f,0.807843f,0.21794872f},
	{0.2125f  ,0.1275f  ,0.054f   ,0.714f   ,0.4284f  ,0.18144f ,0.393548f,0.271906f,0.166721f,0.2f},
	{0.25f    ,0.25f    ,0.25f    ,0.4f     ,0.4f     ,0.4f     ,0.774597f,0.774597f,0.774597f,0.6f},
	{0.19125f ,0.0735f  ,0.0225f  ,0.7038f  ,0.27048f ,0.0828f  ,0.256777f,0.137622f,0.086014f,0.1f},
	{0.24725f ,0.1995f  ,0.0745f  ,0.75164f ,0.60648f ,0.22648f ,0.628281f,0.555802f,0.366065f,0.4f},
	{0.19225f ,0.19225f ,0.19225f ,0.50754f ,0.50754f ,0.50754f ,0.508273f,0.508273f,0.508273f,0.4f},
};

CMaterielle::CMaterielle()
{
	ZeroMemory(&m_Materielle, sizeof(D3DMATERIAL9));
	m_Materielle.Ambient.r = 0.2f;
	m_Materielle.Ambient.g = 0.2f;
	m_Materielle.Ambient.b = 0.2f;
	m_Materielle.Ambient.a = 1.0f;
	m_Materielle.Diffuse.r = 0.8f;
    m_Materielle.Diffuse.g = 0.8f;
    m_Materielle.Diffuse.b = 0.8f;
	m_Materielle.Diffuse.a = 1.0f;
	m_Materielle.Specular.r = 1.0f;
	m_Materielle.Specular.g = 1.0f;
	m_Materielle.Specular.b = 1.0f;
	m_Materielle.Specular.a = 1.0f;
	m_Materielle.Power = 30.f;
}

CMaterielle::~CMaterielle()
{
}

void CMaterielle::SetAmbiante(D3DCOLOR Couleur)
{
	m_Materielle.Ambient.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Materielle.Ambient.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Materielle.Ambient.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Materielle.Ambient.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CMaterielle::SetAmbiante(D3DCOLORVALUE* Couleur)
{
	m_Materielle.Ambient = *Couleur;
}

void CMaterielle::SetAmbiante(float r, float g, float b, float a)
{
	m_Materielle.Ambient.r = r;
    m_Materielle.Ambient.g = g;
    m_Materielle.Ambient.b = b;
	m_Materielle.Ambient.a = a;
}

void CMaterielle::SetDiffuse(D3DCOLOR Couleur)
{
	m_Materielle.Diffuse.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Materielle.Diffuse.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Materielle.Diffuse.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Materielle.Diffuse.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CMaterielle::SetDiffuse(D3DCOLORVALUE* Couleur)
{
	m_Materielle.Diffuse = *Couleur;
}

void CMaterielle::SetDiffuse(float r, float g, float b, float a)
{
	m_Materielle.Diffuse.r = r;
    m_Materielle.Diffuse.g = g;
    m_Materielle.Diffuse.b = b;
	m_Materielle.Diffuse.a = a;
}

void CMaterielle::SetAmbianteDiffuse(D3DCOLOR Couleur)
{
	SetDiffuse(Couleur);
	SetAmbiante(Couleur);
}

void CMaterielle::SetAmbianteDiffuse(D3DCOLORVALUE* Couleur)
{
	SetDiffuse(Couleur);
	SetAmbiante(Couleur);
}

void CMaterielle::SetAmbianteDiffuse(float r, float g, float b, float a)
{
	SetDiffuse(r, g, b, a);
	SetAmbiante(r, g, b, a);
}

void CMaterielle::SetSpeculaire(D3DCOLOR Couleur)
{
	m_Materielle.Specular.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Materielle.Specular.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Materielle.Specular.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Materielle.Specular.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CMaterielle::SetSpeculaire(D3DCOLORVALUE* Couleur)
{
	m_Materielle.Specular = *Couleur;
}

void CMaterielle::SetSpeculaire(float r, float g, float b, float a)
{
	m_Materielle.Specular.r = r;
    m_Materielle.Specular.g = g;
    m_Materielle.Specular.b = b;
	m_Materielle.Specular.a = a;
}

void CMaterielle::SetPuissance(float Puissance)
{
	m_Materielle.Power = Puissance;
}

void CMaterielle::SetEmissive(D3DCOLOR Couleur)
{
	m_Materielle.Emissive.r = D3DCOLOR_R(Couleur) / 255.0f;
    m_Materielle.Emissive.g = D3DCOLOR_G(Couleur) / 255.0f;
    m_Materielle.Emissive.b = D3DCOLOR_B(Couleur) / 255.0f;
	m_Materielle.Emissive.a = D3DCOLOR_A(Couleur) / 255.0f;
}

void CMaterielle::SetEmissive(D3DCOLORVALUE* Couleur)
{
	m_Materielle.Emissive = *Couleur;
}

void CMaterielle::SetEmissive(float r, float g, float b, float a)
{
	m_Materielle.Emissive.r = r;
    m_Materielle.Emissive.g = g;
    m_Materielle.Emissive.b = b;
	m_Materielle.Emissive.a = a;
}

void CMaterielle::SetMaterielle(DWORD Type)
{
	if(Type >= sizeof(TabMaterielle)/sizeof(TabMaterielle[0]))
		return;
	SetAmbiante(TabMaterielle[Type][0], TabMaterielle[Type][1], TabMaterielle[Type][2]);
	SetDiffuse(TabMaterielle[Type][3], TabMaterielle[Type][4], TabMaterielle[Type][5]);
	SetSpeculaire(TabMaterielle[Type][6], TabMaterielle[Type][7], TabMaterielle[Type][8]);
	SetPuissance(TabMaterielle[Type][9]*128.0f);
}