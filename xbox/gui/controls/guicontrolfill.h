#ifndef H_CGUICONTROLFILL
#define H_CGUICONTROLFILL

#include "..\guicontrol.h"
#include <xtl.h>

struct CUSTOM_FILL_VERTEX
{
    FLOAT x, y, z , RHW;
    DWORD color;
};

class CGUIControlFill : public CGUIControl
{
public:
	CGUIControlFill(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwColor);
	~CGUIControlFill();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();

private:
	LPDIRECT3DDEVICE8			m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8		m_pVertexBuffer;

	DWORD m_dwColor;
};

#endif //H_CGUICONTROLFILL