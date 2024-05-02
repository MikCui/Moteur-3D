
// Classe pour la gestion d'une SkyBox

#pragma once
#include "..\Moteur\Model.h"

class CSkyBox :	public CModel
{
public:

	D3DXMATRIXA16 m_MatriceView;	// matrice de transformation de Vision
	D3DXMATRIXA16 m_MatriceWorld;	// matrice de transformation du monde

	CSkyBox(CScene* pScene);
	~CSkyBox();

	void RenduModel();
};
