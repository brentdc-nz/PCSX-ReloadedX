#include "guicontrolspin.h"
#include "..\xboxgui.h"
#include "..\..\input\sysxboxinput.h"
#include "..\utils\stringutils.h"

CGUIControlSpin::CGUIControlSpin(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus, std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown
		                         , std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight)
: CGUIControl(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight)
, m_label(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFont, strText, dwColor, iSize)
, m_labelValue(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFont, "", dwColor, iSize)
, m_imgFocused(iControlID, iWindowID, iPosX-10, iPosY, iWidth+10, iHeight, strImageFocus) // FIXME: Don't hardcode spaces at start and end
, m_ImgScrollUp(iControlID, iWindowID, iPosX, iPosY, iSpinWidth, iSpinHeight, strSpinUp)
, m_ImgScrollUpFocus(iControlID, iWindowID, iPosX, iPosY, iSpinWidth, iSpinHeight, strSpinUpFocus)
, m_ImgScrollDown(iControlID, iWindowID, iPosX, iPosY, iSpinWidth, iSpinHeight, strSpinDown)
, m_ImgScrollDownFocus(iControlID, iWindowID, iPosX, iPosY, iSpinWidth, iSpinHeight, strSpinDownFocus)
{
	m_iButtonSelect = SPIN_BUTTON_DOWN;
}

CGUIControlSpin::~CGUIControlSpin()
{
}

bool CGUIControlSpin::AllocateResources()
{
	m_label.AllocateResources();
	m_labelValue.AllocateResources();
	m_imgFocused.AllocateResources();
	m_ImgScrollUp.AllocateResources();
	m_ImgScrollUpFocus.AllocateResources();
	m_ImgScrollDown.AllocateResources();
	m_ImgScrollDownFocus.AllocateResources();

	m_labelValue.SetPosition(m_iPosX+m_iWidth-m_ImgScrollUp.GetWidth()*2-5, m_iPosY); // Pad by 5 for now
	m_labelValue.SetAlignment(XFONT_RIGHT);

	m_ImgScrollDown.SetPosition(m_iPosX+m_iWidth-m_ImgScrollUp.GetWidth()*2, m_iPosY);
	m_ImgScrollDownFocus.SetPosition(m_iPosX+m_iWidth-m_ImgScrollUp.GetWidth()*2, m_iPosY);
	m_ImgScrollUp.SetPosition(m_iPosX+m_iWidth-m_ImgScrollUp.GetWidth(), m_iPosY);
	m_ImgScrollUpFocus.SetPosition(m_iPosX+m_iWidth-m_ImgScrollUp.GetWidth(), m_iPosY);

	return CGUIControl::AllocateResources();
}

void CGUIControlSpin::FreeResources()
{
	m_label.FreeResources();
	m_labelValue.FreeResources();
	m_imgFocused.FreeResources();
	m_ImgScrollUp.FreeResources();
	m_ImgScrollUpFocus.FreeResources();
	m_ImgScrollDown.FreeResources();
	m_ImgScrollDownFocus.FreeResources();

	CGUIControl::FreeResources();
}

void CGUIControlSpin::Render()
{
	if(!IsVisible()) return;

	m_label.Render();
	m_labelValue.Render();

	if(HasFocus())
	{
		m_imgFocused.Render();
	
		if(m_iButtonSelect == SPIN_BUTTON_UP)
			m_ImgScrollUpFocus.Render();
		else
			m_ImgScrollUp.Render();

		if(m_iButtonSelect == SPIN_BUTTON_DOWN)
			m_ImgScrollDownFocus.Render();
		else
			m_ImgScrollDown.Render();
	}
	else
	{
		m_ImgScrollUp.Render();
		m_ImgScrollDown.Render();
	}

	CGUIControl::Render();
}

bool CGUIControlSpin::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		CGUIMessage msg(GUI_MSG_CLICKED, GetParentID(), GetID());
		g_XboxGUI.SendMessage(msg);

		return true;
	}

	return CGUIControl::OnKey(iKey);
}

void CGUIControlSpin::OnLeft()
{
	if (m_iButtonSelect == SPIN_BUTTON_UP)
		m_iButtonSelect = SPIN_BUTTON_DOWN;
	else
		CGUIControl::OnLeft();
}

void CGUIControlSpin::OnRight()
{
	if (m_iButtonSelect == SPIN_BUTTON_DOWN)
		m_iButtonSelect = SPIN_BUTTON_UP;
	else
		CGUIControl::OnRight();
}

//===========================================

CGUIControlSpinInt::CGUIControlSpinInt(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus, std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown
		                               ,std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight)
: CGUIControlSpin(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFont, iSize, strText, dwColor, strImageFocus, strSpinUp, strSpinUpFocus, strSpinDown
		          ,strSpinDownFocus, iSpinWidth, iSpinHeight)
{
	m_iValue = 0;
}

CGUIControlSpinInt::~CGUIControlSpinInt()
{
}

bool CGUIControlSpinInt::AllocateResources()
{
	return CGUIControlSpin::AllocateResources();
}

void CGUIControlSpinInt::FreeResources()
{
	for(int i = 0; i < (int)m_vecOptions.size(); i++)
	{
		COptionsInt* pOption = NULL;
		pOption = m_vecOptions[i];
		if(pOption)
			delete pOption;
	}

	m_vecOptions.clear();

	CGUIControlSpin::FreeResources();
}

void CGUIControlSpinInt::Render()
{
	if(m_vecOptions.size() > 0 && m_iValue <= (int)m_vecOptions.size())
	{
		COptionsInt* pOption = m_vecOptions[m_iValue];

		std::string strRange;
		strRange = " ("+CStringUtils::IntToString(m_iValue+1)+"/"+CStringUtils::IntToString((int)m_vecOptions.size())+")";

		m_labelValue.SetLabel(pOption->GetDisplayText() + strRange);
	}

	CGUIControlSpin::Render();
}

bool CGUIControlSpinInt::OnMessage(CGUIMessage message)
{
	if(message.GetControlID() == GetID())
	{	
		switch(message.GetMessage())
		{
			case GUI_MSG_ADD_ITEM:
				{ // Keep in this scope
					COptionsInt* pOption = NULL;
					pOption = new COptionsInt(message.GetIntParam(), message.GetStringParam1());

					if(pOption)
						m_vecOptions.push_back(pOption);

					return true;
				}

			case GUI_MSG_SET_ITEM:
				{
					for (int i = 0; i < (int)m_vecOptions.size(); i++)
					{
						COptionsInt* pOption = NULL;
						pOption = m_vecOptions[i];
						if(pOption)
						{
							if(pOption->GetValue() == message.GetIntParam())
								m_iValue = i;
						}
					}
					return true;
				}
		}
	}

	return CGUIControlSpin::OnMessage(message);
}

bool CGUIControlSpinInt::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		if(m_iButtonSelect == SPIN_BUTTON_DOWN)
		{
			if(m_iValue > 0)
				m_iValue--;				
		}

		if(m_iButtonSelect == SPIN_BUTTON_UP)
		{
			if(m_iValue < (int)m_vecOptions.size()-1)
				m_iValue++;
		}
	}

	CGUIMessage msg(GUI_MSG_UPDATED, GetParentID(), GetID());
	g_XboxGUI.SendMessage(msg);

	return CGUIControlSpin::OnKey(iKey);
}

int CGUIControlSpinInt::GetSelectedValue() // TODO: Move to using a message
{
	COptionsInt* pOption = NULL;
	pOption = m_vecOptions[m_iValue];

	if(pOption)
		return pOption->GetValue();

	return 0;
}

//===========================================


CGUIControlSpinString::CGUIControlSpinString(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus, std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown
		                               ,std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight)
: CGUIControlSpin(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight, strFont, iSize, strText, dwColor, strImageFocus, strSpinUp, strSpinUpFocus, strSpinDown
		          ,strSpinDownFocus, iSpinWidth, iSpinHeight)
{
	m_iValue = 0;
}

CGUIControlSpinString::~CGUIControlSpinString()
{
}

bool CGUIControlSpinString::AllocateResources()
{
	return CGUIControlSpin::AllocateResources();
}

void CGUIControlSpinString::FreeResources()
{
	for(int i = 0; i < (int)m_vecOptions.size(); i++)
	{
		COptionsString* pOption = NULL;
		pOption = m_vecOptions[i];
		if(pOption)
			delete pOption;
	}

	m_vecOptions.clear();

	CGUIControlSpin::FreeResources();
}

void CGUIControlSpinString::Render()
{
	if(m_vecOptions.size() > 0 && m_iValue <= (int)m_vecOptions.size())
	{
		COptionsString* pOption = m_vecOptions[m_iValue];

		std::string strRange;
		strRange = " ("+CStringUtils::IntToString(m_iValue+1)+"/"+CStringUtils::IntToString((int)m_vecOptions.size())+")";

		m_labelValue.SetLabel(pOption->GetDisplayText() + strRange);
	}

	CGUIControlSpin::Render();
}

bool CGUIControlSpinString::OnMessage(CGUIMessage message)
{
	if(message.GetControlID() == GetID())
	{	
		switch(message.GetMessage())
		{
			case GUI_MSG_ADD_ITEM:
				{ // Keep in this scope
					COptionsString* pOption = NULL;
					pOption = new COptionsString(message.GetStringParam1(), message.GetStringParam2());

					if(pOption)
						m_vecOptions.push_back(pOption);

					return true;
				}

			case GUI_MSG_SET_ITEM:
				{
					for (int i = 0; i < (int)m_vecOptions.size(); i++)
					{
						COptionsString* pOption = NULL;
						pOption = m_vecOptions[i];

						if(pOption)
						{
							if(pOption->GetValue() == message.GetStringParam1())
								m_iValue = i;
						}
					}
					return true;
				}
		}
	}

	return CGUIControlSpin::OnMessage(message);
}

bool CGUIControlSpinString::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		if(m_iButtonSelect == SPIN_BUTTON_DOWN)
		{
			if(m_iValue > 0)
				m_iValue--;				
		}

		if(m_iButtonSelect == SPIN_BUTTON_UP)
		{
			if(m_iValue < (int)m_vecOptions.size()-1)
				m_iValue++;
		}
	}

	CGUIMessage msg(GUI_MSG_UPDATED, GetParentID(), GetID());
	g_XboxGUI.SendMessage(msg);

	return CGUIControlSpin::OnKey(iKey);
}

std::string CGUIControlSpinString::GetSelectedValue() // TODO: Move to using a message
{
	COptionsString* pOption = NULL;
	pOption = m_vecOptions[m_iValue];

	if(pOption)
		return pOption->GetValue();

	return "";
}