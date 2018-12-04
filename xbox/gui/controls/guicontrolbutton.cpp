#include "guicontrolbutton.h"
#include "..\guiwindowkeys.h"
#include "..\xboxgui.h"
#include "..\..\input\sysxboxinput.h"

CGUIControlButton::	CGUIControlButton(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, DWORD dwSelctedColor, unsigned int iSize
		                              ,std::string strFocus, std::string strNoFocus, int iTextOffsetX, int iTextOffsetY, int iLinkedWindow)
: CGUIControl(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight)
, m_label(iControlID, iWindowID, iPosX+iTextOffsetX, iPosY+iTextOffsetY, iWidth, iHeight, strFont, strText, dwColor, iSize)
, m_ImgFocus(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFocus)
, m_ImgNoFocus(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strNoFocus)
{
	m_dwColor = dwColor;
	m_dwSelectedColor = dwSelctedColor;
	m_iLinkedWindow = iLinkedWindow;
}

CGUIControlButton::~CGUIControlButton()
{
}

bool CGUIControlButton::AllocateResources()
{
	m_label.AllocateResources();
	m_ImgFocus.AllocateResources();
	m_ImgNoFocus.AllocateResources();

	return CGUIControl::AllocateResources();
}

void CGUIControlButton::FreeResources()
{
	m_label.FreeResources();
	m_ImgFocus.FreeResources();
	m_ImgNoFocus.FreeResources();

	CGUIControl::FreeResources();
}

void CGUIControlButton::Render()
{
	if(m_bHasFocus)
	{	
		m_ImgFocus.Render();
		m_label.Render(m_dwSelectedColor);
	}
	else
	{
		m_ImgNoFocus.Render();
		m_label.Render(m_dwColor);
	}

	CGUIControl::Render();
}

bool CGUIControlButton::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		OnClick();
		return true;
	}

	return CGUIControl::OnKey(iKey);
}

void CGUIControlButton::OnClick()
{
	if(m_iLinkedWindow != WINDOW_INVALID)
		g_XBoxGUI.ActivateWindow(m_iLinkedWindow);
	else
	{
		CGUIMessage msg(GUI_MSG_CLICKED, GetParentID(), GetID());
		g_XBoxGUI.SendMessage(msg);
	}
}