#ifndef H_CGUICONTROLLABEL
#define H_CGUICONTROLLABEL

#include "..\guicontrol.h"
#include "..\guimessage.h"
#include "..\guifont.h"
#include <string>

class CGUIControlLabel : public CGUIControl
{
public:
	CGUIControlLabel(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, unsigned int iSize = 32);
	~CGUIControlLabel();
	
	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual bool OnMessage(CGUIMessage message);
	virtual void Render();

	void Render(DWORD dwColor);
	void SetLabel(std::string strText) { m_strText = strText; };
	void SetAlignment(int iAlignment) { m_iAlignment = iAlignment; };
	std::string GetFont() { return m_strFont; };
	DWORD GetColor() { return m_dwColor; };
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; };
	unsigned int GetSize() { return m_iSize; };

private:
	std::string m_strText;
	std::string m_strFont;
	DWORD m_dwColor;
	CGUIFont* m_pFont;
	unsigned int m_iSize;
	int m_iAlignment;
};

#endif //H_CGUICONTROLLABEL