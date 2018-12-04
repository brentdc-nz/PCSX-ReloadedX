#include "guiwindow.h"
#include "guifontmanager.h"
#include "guicontrolfactory.h"
#include "guiwindowkeys.h"
#include "xboxgui.h"
#include "..\input\sysxboxinput.h"

using namespace std;

CGUIWindow::CGUIWindow(int iWindowID, std::string strXMLFile)
{
	m_strXMLFile = strXMLFile;
	m_bAllocated = false;
	m_bLoaded = false;
	m_iID = iWindowID;
	m_iLastControlID = 0;
	m_iDefaultCOntrolID = 0;
}

CGUIWindow::~CGUIWindow()
{
}

void CGUIWindow::Initialize()
{
//	if(!g_XBoxGUI.GetWindowManager().IsInitialized())
//		return;

	LoadXML(m_strXMLFile);
}

bool CGUIWindow::LoadXML(std::string strXMLFile)
{
	if(m_bLoaded)
		return true; // No point loading if it's already there

	TiXmlDocument xmlDoc;

	std::string strPath(g_XBoxGUI.GetMediaDir());
	strPath += strXMLFile;

	if(!xmlDoc.LoadFile(strPath.c_str()))
	{
		SetID(WINDOW_INVALID);
		return false;
	}

	m_iDefaultCOntrolID = 0;

	TiXmlElement* pRootElement = xmlDoc.RootElement();

	if (strcmpi(pRootElement->Value(), "window"))
		return false;

	TiXmlElement *pChild = pRootElement->FirstChildElement();
	while (pChild)
	{
		string strValue = pChild->Value();
		if (strValue == "defaultcontrol" && pChild->FirstChild())
		{
			const char *always = pChild->Attribute("always");
			
			m_iDefaultCOntrolID = atoi(pChild->FirstChild()->Value());
		}
		else if (strValue == "controls")
		{
			TiXmlElement *pControl = pChild->FirstChildElement();
			while (pControl)
			{
				if (strcmpi(pControl->Value(), "control") == 0)
				{
					LoadControl(pControl);
				}
				pControl = pControl->NextSiblingElement();
			}
		}

		pChild = pChild->NextSiblingElement();
	}

	m_bLoaded = true;

	return true;
}

void CGUIWindow::LoadControl(TiXmlElement* pControl)
{
	CGUIControlFactory factory;

	CGUIControl* pGUIControl = factory.Create(GetID(), pControl);

	InsertControl(pGUIControl);
}

bool CGUIWindow::AllocateResources()
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		pControl->AllocateResources();
	}

	m_bAllocated = true;

	return true;
}

void CGUIWindow::FreeResources(bool bClearControls/* = false*/)
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		int id = pControl->GetID();

		if(pControl)
			pControl->FreeResources();
	}

	if(bClearControls)
		ClearAll();

	m_bAllocated = false;
}

void CGUIWindow::ClearAll()
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		delete pControl;
	}

	m_vecControls.clear();
	m_bLoaded = false;
}

bool CGUIWindow::OnKey(int iKey)
{
	CGUIControl* pControl = GetFocusedControl();
	
	if(pControl)
	{
		if(pControl->OnKey(iKey))
			return true;
	}
	else // No control has focus?
	{    // Set the default control to focus then
		
//		CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), m_iDefaultCOntrolID); //FIXME
//		g_XBoxGUI.SendMessage(msg);
	}

	// Default behaviour
	if(iKey == K_XBOX_B)
	{
		g_XBoxGUI.GetWindowManager().PreviousWindow();
		return true;
	}	

	return false;
}

bool CGUIWindow::OnMessage(CGUIMessage message)
{
	switch (message.GetMessage())
	{
		case GUI_MSG_WINDOW_INIT:
		{
			if(!m_bAllocated)
				AllocateResources();
			
			RestoreControlStates();
			return true;
		}

		case GUI_MSG_WINDOW_DEINIT:
		{
			FreeResources();
			SaveControlStates();
			return true;
		}
	}

	bool handled = false;

	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];
		if(pControl)
		{
			if(pControl->OnMessage(message))
			handled = true;
		}
	}

	return handled;
}

void CGUIWindow::Render()
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		pControl->Render();
	}
}

void CGUIWindow::SaveControlStates()
{
	CGUIControl* pControl = GetFocusedControl();
	if(pControl)
		m_iLastControlID = pControl->GetID();
}

void CGUIWindow::RestoreControlStates()
{
	int iFocusedControl = m_iLastControlID ? m_iLastControlID : m_iDefaultCOntrolID;

	CGUIMessage msg(GUI_MSG_SETFOCUS, GetID(), iFocusedControl);
	OnMessage(msg);
}

void CGUIWindow::InsertControl(CGUIControl* pControl)
{
	if(pControl)
		m_vecControls.push_back(pControl);
}

void CGUIWindow::RemoveControl(int iID)
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		if(pControl->GetID() == iID)
		{
			m_vecControls.erase(m_vecControls.begin()+i);
			return;
		}
	}
}

CGUIControl* CGUIWindow::GetControl(int iControlID)
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		if(pControl->GetID() == iControlID)
			return pControl;
	}

	return NULL;
}

CGUIControl* CGUIWindow::GetFocusedControl()
{
	for(int i = 0; i < (int)m_vecControls.size(); i++)
	{
		CGUIControl* pControl = NULL;
		pControl = m_vecControls[i];

		if(pControl->HasFocus())
			return pControl;
	}

	return NULL;
}