#pragma once

#include <Windows.h>
#include <cmath>

const double pi = 3.14159265358979323846;

/*  #### Déclaration des focntions ####  */

// Interpolation entre les bornes a et b, sx € [0 ; 1]
template<typename T> 
T Interpolation(T a, T b, double s);

// Interpolation entre les bornes ay et by, sx € [cx ; dx]
template<typename T, typename U> 
T Interpolation(T ay, T by, U cx, U dx, U sx);

// Interpolation entre les couleurs a et b, sx € [0 ; 1]
COLORREF InterpolationCouleur(COLORREF a, COLORREF b, double s);

// Interpolation entre les couleurs a et b, s € [c ; d]
template<typename T> 
COLORREF InterpolationCouleur(COLORREF a, COLORREF b, T c, T d, T s);

// Interpolation logarithmique entre les bornes ay et by, sx € [cx ; dx]
// (Permet de favoriser les valeurs proche de cx)
double InterpolationLog(double ay, double by, double cx, double dx, double sx, double max);

// Inverse les octets d'un entier
int InverserOctet(int Donnee);

/*  #### Définition des focntions ####  */

// Interpolation entre les bornes a et b, sx € [0 ; 1]
template<typename T>
inline T Interpolation(T a, T b, double s)
{
	return T(a + s * (b - a));
}

// Interpolation entre les bornes ay et by, sx € [cx ; dx]
template<typename T, typename U>
inline T Interpolation(T ay, T by, U cx, U dx, U sx)
{
	if(sx<cx) sx = cx;
	if(sx>dx) sx = dx;
	double Coefficient = double(by - ay) / double(dx - cx);
	double Offset = by - Coefficient * dx;
	return T(Coefficient * sx + Offset);
}

// Interpolation entre les couleurs a et b, sx € [0 ; 1]
inline COLORREF InterpolationCouleur(COLORREF a, COLORREF b, double s)
{
	BYTE a_a = BYTE(a >> 24) & 0xFF;
	BYTE r_a = BYTE(a >> 16) & 0xFF;
	BYTE g_a = BYTE(a >>  8) & 0xFF;
	BYTE b_a = BYTE(a      ) & 0xFF;

	BYTE a_b = BYTE(b >> 24) & 0xFF;
	BYTE r_b = BYTE(b >> 16) & 0xFF;
	BYTE g_b = BYTE(b >>  8) & 0xFF;
	BYTE b_b = BYTE(b      ) & 0xFF;

	BYTE a_s = Interpolation(a_a, a_b, s);
	BYTE r_s = Interpolation(r_a, r_b, s);
	BYTE g_s = Interpolation(g_a, g_b, s);
	BYTE b_s = Interpolation(b_a, b_b, s);
	
	return COLORREF((a_s<<24) | (r_s<<16) | (g_s<<8) | b_s);
}

// Interpolation entre les couleurs a et b, s € [c ; d]
template<typename T>
inline COLORREF InterpolationCouleur(COLORREF a, COLORREF b, T c, T d, T s)
{
	BYTE a_a = BYTE(a >> 24) & 0xFF;
	BYTE r_a = BYTE(a >> 16) & 0xFF;
	BYTE g_a = BYTE(a >>  8) & 0xFF;
	BYTE b_a = BYTE(a      ) & 0xFF;

	BYTE a_b = BYTE(b >> 24) & 0xFF;
	BYTE r_b = BYTE(b >> 16) & 0xFF;
	BYTE g_b = BYTE(b >>  8) & 0xFF;
	BYTE b_b = BYTE(b      ) & 0xFF;

	BYTE a_s = Interpolation(a_a, a_b, c, d, s);
	BYTE r_s = Interpolation(r_a, r_b, c, d, s);
	BYTE g_s = Interpolation(g_a, g_b, c, d, s);
	BYTE b_s = Interpolation(b_a, b_b, c, d, s);
	
	return COLORREF((a_s<<24) | (r_s<<16) | (g_s<<8) | b_s);
}

// Interpolation logarithmique entre les bornes ay et by, sx € [cx ; dx]
// (Permet de favoriser les valeurs proche de cx)
inline double InterpolationLog(double ay, double by, double cx, double dx, double sx, double max)
{
	if(sx<cx) sx = cx;
	if(sx>dx) sx = dx;
	double x = Interpolation(1.0, max, cx, dx, sx);
	return Interpolation(ay, by, 0.0, log10(max), log10(x));
}

// Inverse les octets d'un entier
inline int InverserOctet(int Donnee)
{
	DWORD a = ((Donnee >> 24) & 0xFF);
	DWORD b = ((Donnee >> 16) & 0xFF);
	DWORD c = ((Donnee >>  8) & 0xFF);
	DWORD d = ( Donnee        & 0xFF);
	return int((d<<24) | (c<<16) | (b<<8) | a);
}

inline DWORD VectorToDWORD(D3DXVECTOR3* vVector)
{
    DWORD dwRed   = (DWORD)(127 * vVector->x + 128);
    DWORD dwGreen = (DWORD)(127 * vVector->y + 128);
    DWORD dwBlue  = (DWORD)(127 * vVector->z + 128);

    return (DWORD)(0xff000000 + (dwRed << 16) + (dwGreen << 8) + dwBlue);
}