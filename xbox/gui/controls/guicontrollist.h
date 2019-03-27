#ifndef H_CGUICONTROLLIST
#define H_CGUICONTROLLIST

#include "..\guicontrol.h"
#include "..\guifont.h"
#include <string>
#include <vector>

class CGUIListItem
{
public:
	CGUIListItem(std::string strName, bool bIsDirectory = false);
	~CGUIListItem();

	std::string GetName() { return m_strName; };

private:
	std::string m_strName;
	bool m_bIsDirectory;
};

class CGUIControlList : public CGUIControl
{
public:
	CGUIControlList(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, int iLineSpacing, std::string strFont, DWORD dwColor, unsigned int iSize, DWORD dwSelectedColor);
	~CGUIControlList();

	virtual void Render();
	virtual bool OnKey(int iKey);

	bool AddItem(CGUIListItem* pItem);
	CGUIListItem* GetSelectedItem();
	int GetSize();
	virtual void FreeResources();

private:
    int m_iCursor;
	int m_iOffset;
	int m_iLineSpacing;
	CGUIFont* m_pFont;
	unsigned int m_iFontSize;
	DWORD m_dwColor;
	DWORD m_dwSelectedColor;
	std::vector <CGUIListItem*>m_vecItems;
};

#endif //H_CGUICONTROLLIST