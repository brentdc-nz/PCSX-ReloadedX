#include "guicontrol.h"
#include "guimessage.h"
#include "xboxgui.h"
#include "..\input\sysxboxinput.h"

CGUIControl::CGUIControl(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight)
{
	m_iID = iControlID;
	m_iParentID = iWindowID;
	m_iPosX = iPosX;
	m_iPosY = iPosY;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_bSelected = false;;
	m_bHasFocus = false;
	m_bVisible = true;
	m_bResoucesAlocated = false;
	m_iControlUp = m_iControlDown = m_iControlLeft = m_iControlRight = UNKNOWN_ID;
}

CGUIControl::~CGUIControl()
{
}

bool CGUIControl::AllocateResources()
{
	return true;
}

void CGUIControl::FreeResources()
{
}

void CGUIControl::Render()
{
}

bool CGUIControl::OnKey(int iKey)
{
	switch(iKey)
	{
		case K_XBOX_DPAD_UP:
		OnUp();
		return true;
		break;

		case K_XBOX_DPAD_DOWN:
		OnDown();
		return true;
		break;

		case K_XBOX_DPAD_LEFT:
		OnLeft();
		return true;
		break;

		case K_XBOX_DPAD_RIGHT:
		OnRight();
		return true;
		break;
	}

	return false;
}

bool CGUIControl::OnMessage(CGUIMessage message)
{
	if(message.GetControlID() == GetID())
	{	
		switch(message.GetMessage())
		{
			case GUI_MSG_SETFOCUS:
			SetFocus(true);
			return true;
			break;

			default:
			return false;
			break;
		}
	}

	// Not processed
	return false;
}

// Movement (can be overridden in derived classes)

void CGUIControl::SetMovement(int iUp, int iDown, int iLeft, int iRight)
{
	m_iControlUp = iUp;
	m_iControlDown = iDown;
	m_iControlLeft = iLeft;
	m_iControlRight = iRight;
}

void CGUIControl::OnUp()
{
	if(HasFocus() && (m_iID != m_iControlUp) && (m_iControlUp != UNKNOWN_ID))
	{
		SetFocus(false);
		CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), m_iControlUp);
		g_XboxGUI.SendMessage(msg);
	}
}

void CGUIControl::OnDown()
{
	if(HasFocus() && (m_iID != m_iControlDown) && (m_iControlDown != UNKNOWN_ID))
	{
		SetFocus(false);
		CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), m_iControlDown);
		g_XboxGUI.SendMessage(msg);
	}
}

void CGUIControl::OnLeft()
{
	if(HasFocus() && (m_iID != m_iControlLeft) && (m_iControlLeft != UNKNOWN_ID))
	{
		SetFocus(false);
		CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), m_iControlLeft);
		g_XboxGUI.SendMessage(msg);
	}
}

void CGUIControl::OnRight()
{
	if(HasFocus() && (m_iID != m_iControlRight) && (m_iControlRight != UNKNOWN_ID))
	{
		SetFocus(false);
		CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), m_iControlRight);
		g_XboxGUI.SendMessage(msg);
	}
}