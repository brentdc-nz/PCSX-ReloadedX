#ifndef H_CGUICONTROLIMAGE
#define H_CGUICONTROLIMAGE

#include <xtl.h>
#include "..\guicontrol.h"
#include <string>

struct CUSTOM_IMAGE_VERTEX
{
    FLOAT x, y, z , RHW;
    FLOAT tx, ty;
};

class CGUIControlImage : public CGUIControl
{
public:
	CGUIControlImage(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFileName);
	~CGUIControlImage();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	std::string GetImageFilename() { return m_strFilename; };

private:
	std::string m_strFilename;

	LPDIRECT3DDEVICE8			m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8		m_pVertexBuffer;
};

#endif //H_CGUICONTROLIMAGE