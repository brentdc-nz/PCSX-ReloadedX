#include "guicontrollabel.h"
#include "..\xboxgui.h"

using namespace std;

CGUIControlLabel::CGUIControlLabel(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, unsigned int iSize) 
: CGUIControl(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight)
{
	m_strText = strText;
	m_strFont = strFont;
	m_dwColor = dwColor;
	m_pFont = NULL;
	m_iSize = iSize;
	m_iAlignment = XFONT_LEFT;
}

CGUIControlLabel::~CGUIControlLabel()
{
}

bool CGUIControlLabel::AllocateResources()
{
	m_pFont = g_XboxGUI.GetFontManager().GetFont(m_strFont);

	if(!m_pFont)
		return false;

	return CGUIControl::AllocateResources();
}

void CGUIControlLabel::FreeResources()
{
	CGUIControl::FreeResources();
}

void CGUIControlLabel::Render()
{
	if(!IsVisible()) return;

	if(m_pFont)
		m_pFont->Render(m_iPosX, m_iPosY, m_iSize, m_dwColor, m_strText, m_iAlignment);

	CGUIControl::Render();
}

bool CGUIControlLabel::OnMessage(CGUIMessage message)
{
	if(message.GetControlID() == GetID())
	{	
		switch(message.GetMessage())
		{
			case GUI_MSG_SET_LABEL:
			{ 
				SetLabel(message.GetStringParam1());
				return true;
			}
		}
	}

	return CGUIControl::OnMessage(message);
}

void CGUIControlLabel::Render(DWORD dwColor)
{
	if(m_pFont)
		m_pFont->Render(m_iPosX, m_iPosY, m_iSize, dwColor, m_strText);

	CGUIControl::Render();
}