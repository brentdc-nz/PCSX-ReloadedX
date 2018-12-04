#ifndef H_GUITEXTUREMANAGER
#define H_GUITEXTUREMANAGER

#include <xtl.h>
#include <string>
#include <vector>

class CTexture
{
public:
	CTexture(std::string strFileName, LPDIRECT3DTEXTURE8 pTexture){ m_strFileName = strFileName; m_pTexture = pTexture;  };
	~CTexture(){};

	void ReleaseTexture(){ if(m_pTexture != NULL) m_pTexture->Release(); m_pTexture = NULL;};
	std::string GetTextureName() { return m_strFileName; };
	LPDIRECT3DTEXTURE8 GetTexture() { return m_pTexture; };

private:
	std::string m_strFileName;
	LPDIRECT3DTEXTURE8 m_pTexture;
};

class CGUITextureManager
{
public:
	CGUITextureManager();
	~CGUITextureManager();

	bool Initialize();

	void LoadTexture(std::string strFileName);
	LPDIRECT3DTEXTURE8 GetTexture(std::string strFileName);

	void ReleaseAllTextures();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	std::vector <CTexture*>m_vecTextures;
};

#endif //H_GUITEXTUREMANAGER