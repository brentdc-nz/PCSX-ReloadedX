#ifndef H_CGUICONTROL
#define H_CGUICONTROL

#include "guimessage.h"

#define UNKNOWN_ID 0

class CGUIControl
{
public:
	CGUIControl(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight);
	~CGUIControl();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnKey(int iKey);
	virtual bool OnMessage(CGUIMessage message);

	int GetID() { return m_iID; };
	void SetID(int iID) { m_iID = iID; };
	int GetParentID() { return m_iParentID; };
	void SetFocus(bool bFocus) { m_bHasFocus = bFocus; };
	bool HasFocus() { return m_bHasFocus; };

	void SetMovement(int iUp, int iDown, int iLeft, int iRight);
	void SetUp(int iUp){ m_iControlUp = iUp; };
	void SetDown(int iDown){ m_iControlDown = iDown; };
	void SetLeft(int iLeft){ m_iControlLeft = iLeft; };
	void SetRight(int iRight){ m_iControlRight = iRight; };
	void SetNavigation(int iUp, int iDown, int iLeft, int iRight) { m_iControlUp = iUp; m_iControlDown = iDown; m_iControlLeft = iLeft; m_iControlRight = iRight; };

	int GetPosX(){ return m_iPosX; }; 
	int GetPosY(){ return m_iPosY; }; 
	int GetWidth(){ return m_iWidth; };
	int GetHeight(){ return m_iHeight; };

	virtual void Selected(bool bSelected){ m_bSelected = bSelected; };
	virtual bool IsSelcted(){ return m_bSelected; };

	bool IsVisible() { return m_bVisible; };
	void SetVisible(bool bVisible) { m_bVisible = bVisible; };
	void SetPosition(int iPosX, int iPosY) { m_iPosX = iPosX; m_iPosY = iPosY; };

protected:
	virtual void OnUp();
	virtual void OnDown();
	virtual void OnLeft();
	virtual void OnRight();

	int m_iID;
	int m_iParentID;
	int m_iPosX;
	int m_iPosY;
	int m_iWidth;
	int m_iHeight;
	bool m_bSelected;
	bool m_bHasFocus;
	bool m_bVisible;
	bool m_bResoucesAlocated;
	int m_iControlUp;
	int m_iControlDown;
	int m_iControlLeft;
	int m_iControlRight;
};

#endif //H_CGUICONTROL
