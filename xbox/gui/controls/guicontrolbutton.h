#ifndef H_CGUICONTROLBUTTON
#define H_CGUICONTROLBUTTON

#include "..\guicontrol.h"

#include "guicontrollabel.h"
#include "guicontrolimage.h"

#include <string>

class CGUIControlButton : public CGUIControl
{
public:
	CGUIControlButton(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, DWORD dwSelctedColor, unsigned int iSize
		              ,std::string strFocus, std::string strNoFocus, int iTextOffsetX, int iTextOffsetY, int iLinkedWindow);

	~CGUIControlButton();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnKey(int iKey);

	void OnClick();

private:
	CGUIControlLabel m_label;
	CGUIControlImage m_ImgFocus;
	CGUIControlImage m_ImgNoFocus;

	DWORD m_dwColor;
	DWORD m_dwSelectedColor;
	int m_iLinkedWindow;
};

#endif //H_CGUICONTROLBUTTON