#include "guicontrolcheckbox.h"
#include "..\xboxgui.h"
#include "..\..\input\sysxboxinput.h"

CGUIControlCheckbox::CGUIControlCheckbox(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, unsigned int iSize, 
		                                 std::string strImageFocus, int iImgXOffset, std::string strImage, std::string strImageSelected, int ImgWidth, int ImgHeight)
: CGUIControl(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight)
, m_label(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFont, strText, dwColor, iSize)
, m_imgFocused(iControlID, iWindowID, iPosX-10, iPosY, iWidth+10, iHeight, strImageFocus) // FIXME: Don't hardcode spaces at start and end
, m_imgStatus(iControlID, iWindowID, iPosX+iImgXOffset, iPosY, ImgWidth, ImgHeight, strImage)
, m_imgStatusOn(iControlID, iWindowID, iPosX+iImgXOffset, iPosY, ImgWidth, ImgHeight, strImageSelected)
{
	m_iImageOffsetX = iImgXOffset;
	m_bSelected = false;
}

CGUIControlCheckbox::~CGUIControlCheckbox()
{
}

bool CGUIControlCheckbox::AllocateResources()
{
	m_label.AllocateResources();
	m_imgFocused.AllocateResources();
	m_imgStatus.AllocateResources();
	m_imgStatusOn.AllocateResources();

	return CGUIControl::AllocateResources();
}

void CGUIControlCheckbox::FreeResources()
{
	m_label.FreeResources();
	m_imgFocused.FreeResources();
	m_imgStatus.FreeResources();
	m_imgStatusOn.FreeResources();

	CGUIControl::FreeResources();
}

void CGUIControlCheckbox::Render()
{
	if(!IsVisible()) return;

	m_label.Render();

	if(HasFocus())
		m_imgFocused.Render();

	if(m_bSelected)
		m_imgStatusOn.Render();
	else
		m_imgStatus.Render();

	CGUIControl::Render();
}

bool CGUIControlCheckbox::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		Selected(!m_bSelected); // Toggle it

		CGUIMessage msg(GUI_MSG_UPDATED, GetParentID(), GetID());
		g_XboxGUI.SendMessage(msg);

		return true;
	}

	return CGUIControl::OnKey(iKey);
}

void CGUIControlCheckbox::Selected(bool bSelected)
{
	m_bSelected = bSelected;
}

bool CGUIControlCheckbox::IsSelcted()
{
	return m_bSelected;
}