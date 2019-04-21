#include "guiwindowglobalconfigs.h"
#include "..\..\..\libpcsxcore\system.h"
#include "..\guiwindowkeys.h"
#include "..\xboxgui.h"
#include "..\..\input\sysxboxinput.h"
#include "..\guiconfigs.h"
#include "..\controls\guicontrolcheckbox.h"
#include "..\controls\guicontrolspin.h"

// Controls
#define CONTROL_START_CONTROL           1 // Used for the start coordinates
#define CONTROL_DEFAULT_CHECKBOX        2
#define CONTROL_DEFAULT_SPIN            3
#define CONTROL_OPTIONS_START           4

CGUIWindowGlobalConfigs::CGUIWindowGlobalConfigs(int iWindowID, std::string strXMLFile) 
: CGUIWindow(iWindowID, strXMLFile)
{
}

CGUIWindowGlobalConfigs::~CGUIWindowGlobalConfigs()
{
}

bool CGUIWindowGlobalConfigs::AllocateResources()
{
	return CGUIWindow::AllocateResources();
}

bool CGUIWindowGlobalConfigs::OnMessage(CGUIMessage message)
{
	switch(message.GetMessage())
	{
		case GUI_MSG_WINDOW_INIT:
		{
			SetupConfigControls();
			break;
		}

		case GUI_MSG_WINDOW_DEINIT:
		{
		//	SaveConfigs();
			RemoveConfigControls();
			break;
		}

		case GUI_MSG_UPDATED:
		{
			// Check if our skin has changed
			CConfig* pConfig = g_XboxConfigs.GetConfig("gui.skin");

			CGUIControlSpinString* pSkinCtrl = (CGUIControlSpinString*)GetControl(pConfig->GetGUIControlID());
			if(pSkinCtrl)
			{
				if(pSkinCtrl->GetSelectedValue() != g_XboxConfigs.GetString(pConfig->GetConfigName().c_str()))
					g_XboxGUI.DelaySkinLoad();
			}

			SaveConfigs(); //TODO: Save on window close rather than click
			return true;
		}
	}

	return CGUIWindow::OnMessage(message);
}

void CGUIWindowGlobalConfigs::FreeResources()
{
	CGUIWindow::FreeResources();
}

void CGUIWindowGlobalConfigs::RemoveConfigControls()
{
	// Clear out the configs we inserted
	for(int i = 0; i < (int)m_vecOptionControls.size(); i++)
	{
		CGUIControl* pCtrl = GetControl(m_vecOptionControls[i]);
		RemoveControl(m_vecOptionControls[i]);
		if(pCtrl)
		{
			pCtrl->FreeResources();
			delete pCtrl;
		}
	}
	m_vecOptionControls.clear();
}

bool CGUIWindowGlobalConfigs::SetupConfigControls() // If our configs go over a one page I need to rework this!
{
	// Loop through the configs

	// Get a control to use as out start position
	CGUIControlLabel* pCtrlStart = (CGUIControlLabel*)GetControl(CONTROL_START_CONTROL);
	if(pCtrlStart)
	{
		pCtrlStart->SetVisible(false);
		pCtrlStart->SetFocus(false);
	}

	CGUIControlCheckbox* pCtrlDefaultCheckbox = (CGUIControlCheckbox*)GetControl(CONTROL_DEFAULT_CHECKBOX);
	if(pCtrlDefaultCheckbox)
	{
		pCtrlDefaultCheckbox->SetVisible(false);
		pCtrlDefaultCheckbox->SetFocus(false);
	}

	CGUIControlSpin* pCtrlDefaultSpin = (CGUIControlSpin*)GetControl(CONTROL_DEFAULT_SPIN);
	if(pCtrlDefaultSpin)
	{
		pCtrlDefaultSpin->SetVisible(false);
		pCtrlDefaultSpin->SetFocus(false);
	}

	const CXboxConfigs::CfgMap& mapConfigs = g_XboxConfigs.GetConfigs();
	typedef CXboxConfigs::CfgMap::const_iterator mapIter;

	int iID = CONTROL_OPTIONS_START;
	int iPosY = pCtrlStart->GetPosY();

	CGUIControl* pNewCtrl = NULL;

	for (mapIter it = mapConfigs.begin(); it != mapConfigs.end(); it++)
	{
		CConfig* pConfig = (*it).second;

		if(pConfig)
		{
			if(pConfig->GetConfigType() != GLOBAL_CFG)
				continue;

			if(pConfig->GetDataType() == CFG_BOOL)
			{
				CConfigBool* pConfigBool = (CConfigBool*)(*it).second;

				pNewCtrl = new CGUIControlCheckbox(iID, GetID(), pCtrlDefaultCheckbox->GetPosX(), iPosY, pCtrlDefaultCheckbox->GetWidth(), pCtrlDefaultCheckbox->GetHeight()
					                               , pCtrlDefaultCheckbox->GetFont(), pConfigBool->GetDisplayText(), pCtrlDefaultCheckbox->GetColor(), pCtrlDefaultCheckbox->GetSize()
												   , pCtrlDefaultCheckbox->GetFocusImage(), pCtrlDefaultCheckbox->GetImageXOffset(), pCtrlDefaultCheckbox->GetTexture() 
												   , pCtrlDefaultCheckbox->GetTextureSlected(), pCtrlDefaultCheckbox->GetImageWidth(), pCtrlDefaultCheckbox->GetImageHeight());
				if(pNewCtrl)
				{
					InsertControl(pNewCtrl);								
					pNewCtrl->Selected(g_XboxConfigs.GetBool(pConfigBool->GetConfigName().c_str())); //TODO: Change to sending a message
				}
			}

			if(pConfig->GetDataType() == CFG_INT)
			{
				CConfigInt* pConfigInt = (CConfigInt*)(*it).second;

				pNewCtrl = new CGUIControlSpinInt(iID, GetID(), pCtrlDefaultCheckbox->GetPosX(), iPosY, pCtrlDefaultSpin->GetWidth(), pCtrlDefaultSpin->GetHeight(), pCtrlDefaultSpin->GetFont()
					                              , pCtrlDefaultSpin->GetFontSize(), pConfig->GetDisplayText(), pCtrlDefaultSpin->GetColor(), pCtrlDefaultSpin->GetFocusImage(), pCtrlDefaultSpin->GetSpinUp()
											      , pCtrlDefaultSpin->GetSpinUpFocus(), pCtrlDefaultSpin->GetSpinDown(), pCtrlDefaultSpin->GetSpinDownFocus(), pCtrlDefaultSpin->GetSpinWidth()
											      , pCtrlDefaultSpin->GetSpinHeight());

				if(pNewCtrl)
				{
					InsertControl(pNewCtrl);
			
					const CConfigInt::OptionsMap& mapOptions = pConfigInt->GetOptions();
					typedef CConfigInt::OptionsMap::const_iterator mapOptionsIter;

					for (mapOptionsIter it = mapOptions.begin(); it != mapOptions.end(); it++)
					{
						CGUIMessage msg(GUI_MSG_ADD_ITEM, GetID(), iID, (*it).first, (*it).second);
						g_XboxGUI.SendMessage(msg);
					}

					CGUIMessage msg(GUI_MSG_SET_ITEM, GetID(), iID, g_XboxConfigs.GetInt(pConfig->GetConfigName().c_str()));
					g_XboxGUI.SendMessage(msg);
				}
			}

			if(pConfig->GetDataType() == CFG_STRING)
			{
				CConfigString* pConfigString = (CConfigString*)(*it).second;

				pNewCtrl = new CGUIControlSpinString(iID, GetID(), pCtrlDefaultCheckbox->GetPosX(), iPosY, pCtrlDefaultSpin->GetWidth(), pCtrlDefaultSpin->GetHeight(), pCtrlDefaultSpin->GetFont()
					                                , pCtrlDefaultSpin->GetFontSize(), pConfig->GetDisplayText(), pCtrlDefaultSpin->GetColor(), pCtrlDefaultSpin->GetFocusImage(), pCtrlDefaultSpin->GetSpinUp()
											        , pCtrlDefaultSpin->GetSpinUpFocus(), pCtrlDefaultSpin->GetSpinDown(), pCtrlDefaultSpin->GetSpinDownFocus(), pCtrlDefaultSpin->GetSpinWidth()
											        , pCtrlDefaultSpin->GetSpinHeight());

				if(pNewCtrl)
				{
					InsertControl(pNewCtrl);
					
					const CConfigString::OptionsMap& mapOptions = pConfigString->GetOptions();
					typedef CConfigString::OptionsMap::const_iterator mapOptionsIter;

					for (mapOptionsIter it = mapOptions.begin(); it != mapOptions.end(); it++)
					{
						CGUIMessage msg(GUI_MSG_ADD_ITEM, GetID(), iID, (*it).second, (*it).first);
						g_XboxGUI.SendMessage(msg);
					}
			
					CGUIMessage msg(GUI_MSG_SET_ITEM, GetID(), iID, g_XboxConfigs.GetString(pConfig->GetConfigName().c_str()));
					g_XboxGUI.SendMessage(msg);
				}
			}
			
			// TODO: CFG_FLOAT

			pConfig->SetGUIControlID(iID);
			m_vecOptionControls.push_back(iID);
			pNewCtrl->SetUp(iID-1);
			pNewCtrl->SetDown(iID+1);
		}
		iID++;
		iPosY += pCtrlStart->GetHeight(); // Use the start controls height for line spacing
	}

	// FIXME: HACK - Let's just do this for the start and end controls for now
	pNewCtrl->SetDown(CONTROL_OPTIONS_START);
	
	pCtrlDefaultCheckbox = (CGUIControlCheckbox*)GetControl(CONTROL_OPTIONS_START);
	if(pCtrlDefaultCheckbox)
		pCtrlDefaultCheckbox->SetUp(iID-1);

	return true;
}

void CGUIWindowGlobalConfigs::SaveConfigs() //TODO: Change to saving on closing the window
{
	const CXboxConfigs::CfgMap& mapConfigs = g_XboxConfigs.GetConfigs();
	typedef CXboxConfigs::CfgMap::const_iterator mapIter;

	for (mapIter it = mapConfigs.begin(); it != mapConfigs.end(); it++)
	{
		CConfig* pConfig = (*it).second;

		if(pConfig)
		{
			if(pConfig->GetConfigType() != GLOBAL_CFG)
				continue;

			 //TODO: Change to sending a message to get the value for these!

			if(pConfig->GetDataType() == CFG_BOOL)
			{
				CGUIControlCheckbox* pCtrl = (CGUIControlCheckbox*)GetControl(pConfig->GetGUIControlID());
				g_XboxConfigs.SetBool((*it).first.c_str(), pCtrl->IsSelcted());
			}

			if(pConfig->GetDataType() == CFG_INT)
			{
				CGUIControlSpinInt* pCtrl = (CGUIControlSpinInt*)GetControl(pConfig->GetGUIControlID());
				g_XboxConfigs.SetInt((*it).first.c_str(), pCtrl->GetSelectedValue());
			}

			if(pConfig->GetDataType() == CFG_STRING)
			{
				CGUIControlSpinString* pCtrl = (CGUIControlSpinString*)GetControl(pConfig->GetGUIControlID());
				g_XboxConfigs.SetString((*it).first.c_str(), pCtrl->GetSelectedValue());
			}
		}
	}

	g_XboxConfigs.SaveGlobalConfigs();
}