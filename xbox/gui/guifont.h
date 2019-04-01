#ifndef H_CGUIFONT
#define H_CGUIFONT

#define XFONT_TRUETYPE

#include <xtl.h>
#include <xfont.h>
#include <stdio.h>
#include <string>

class CGUIFont
{
public:
	CGUIFont(std::string strFontName, std::string strFilename, DWORD dwStyles);
	~CGUIFont();

	bool Load(bool bFullPath = false);
	std::string GetFontName(){ return m_sFontName; };
	void AlocateResources();
	void Render(int iPosX, int iPosY, unsigned int iSize, D3DCOLOR Color, std::string strText, int iAlignment = XFONT_LEFT);
	void FreeResources();

private:
	XFONT*              m_pFont; 
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DSURFACE8  m_pBackBuffer;

	std::string			m_sFontFile;
	std::string			m_sFontName;
	DWORD				m_dwStyles;
	bool				m_bResoucesAlocated;
	bool				m_bFullPath;
};

#endif //H_CGUIFONT