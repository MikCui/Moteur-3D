#include "Model.h"

CModel::CModel()
{
}

CModel::CModel(CScene* pScene)
{
	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_pMesh = NULL;
	m_pMeshMaterielles = NULL;
	m_pMeshTextures = NULL;
	m_NumMaterielles = 0;
}

CModel::~CModel()
{
}

// Chargement d'un Model 3D à d'un fichier *.x
bool CModel::ChargerModel(char* NomFichier)
{
    LPD3DXBUFFER pMtrlBuffer = NULL;
	LPD3DXBUFFER pAdjacencyBuffer = NULL;
	char Repertoire[MAX_PATH] = "";
	char Fichier[MAX_PATH] = "";

	// On charge le Mesh à partir d'un fichier
    if(FAILED(D3DXLoadMeshFromX(NomFichier, D3DXMESH_SYSTEMMEM, m_pD3DDevice, &pAdjacencyBuffer, &pMtrlBuffer, NULL, &m_NumMaterielles, &m_pMesh)))
    {
		Console << "Impossible de charger le model 3D --> " << NomFichier << endl;
        return false;
    }

	// Optimisation du Mesh
	if(FAILED(m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL )))
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        SAFE_RELEASE( pMtrlBuffer );
		Console << "Impossible d'optimiser le model 3D --> " << NomFichier << endl;
        return false;
    }

	// On récupère un pointeur sur les propriétés des matérielles et textures
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
    // On récupère le répertoire du model 3D
	strcpy(Repertoire, NomFichier);
	PathRemoveFileSpec(Repertoire);
	if(Repertoire[0] != 0) strcat(Repertoire, "\\");
	
	if(m_NumMaterielles > 0)
	{
		// allocation des tableaux de matérielles et de pointeur sur les textures
		m_pMeshMaterielles = new D3DMATERIAL9[m_NumMaterielles];
		m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_NumMaterielles];

		// Initialisation de tout les matérielles
		for(DWORD i=0; i<m_NumMaterielles; i++)
		{
			// Copie du matérielle
			m_pMeshMaterielles[i] = d3dxMaterials[i].MatD3D;

			// Initialisation de la couleur Ambiant
			m_pMeshMaterielles[i].Ambient = m_pMeshMaterielles[i].Diffuse;

			// Chargement de la texture
			m_pMeshTextures[i] = NULL;
			if(d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
			{
				// Création du chemein pour accéder à la texture
				strcpy(Fichier, Repertoire);
				strcat(Fichier, d3dxMaterials[i].pTextureFilename);
				// Création de la texture
				if(FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, Fichier, &m_pMeshTextures[i])))
				{
					Console << "Impossible de trouver la texture du model 3D --> " << NomFichier << " : " << Fichier << endl;
				}
			}
		}
	}

    // Supression du buffer de matérielles
    pMtrlBuffer->Release();

	// Si le Mesh n'a pas de nomale, on va lui en rajouter
	if ((m_pMesh->GetFVF() & D3DFVF_NORMAL) == 0)
	{
		Console << NomFichier << " --> Ajout de Normale" << endl;
		LPD3DXMESH TempMesh;
		// On fais une copie du mesh avec la normale en plus
		if(m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), m_pMesh->GetFVF() | D3DFVF_NORMAL, m_pD3DDevice, &TempMesh) != D3D_OK)
		{
			Console << "Impossible de copier le Mesh --> " << NomFichier << " : creation des normales impossible" << endl;
			return false;
		}
		// Calcule de l'orientation des normales
		D3DXComputeNormals(TempMesh, NULL);
		m_pMesh->Release();
		m_pMesh = TempMesh;
	}
	return true;
}

// Destruction du Model 3D
void CModel::Release()
{
	// Destruction du tableaux de matérielles
	SAFE_DELETE_ARRAY(m_pMeshMaterielles);

	// Destruction des textures
	if(m_pMeshTextures)
	{
		for(DWORD i = 0; i < m_NumMaterielles; i++)
		{
			if(m_pMeshTextures[i]) m_pMeshTextures[i]->Release();
		}
		SAFE_DELETE_ARRAY(m_pMeshTextures);
	}

	// Destruction du Mesh
	SAFE_RELEASE(m_pMesh);
}

// Rendu du Model 3D
void CModel::RenduModel()
{
	// On initialise Stage de rendu
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	// On parcours chaque partie du Mesh
	for(DWORD i=0; i<m_NumMaterielles; i++)
	{
		// On active le matérielle et la texture de la partie à dessiner
        m_pD3DDevice->SetMaterial(&m_pMeshMaterielles[i]);
        m_pD3DDevice->SetTexture(0, m_pMeshTextures[i]);
        
        // Dessin de la partie du Mesh
        m_pMesh->DrawSubset(i);
	}
}
