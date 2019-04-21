#include "guiwindowgameconfigs.h"
#include "..\guiconfigs.h"
#include "..\xboxgui.h"
#include "..\..\input\sysxboxinput.h"
#include "..\guiwindowkeys.h"
#include "..\..\xbox.h"
#include "..\controls\guicontrolcheckbox.h"
#include "..\controls\guicontrolspin.h"

// Controls
#define CONTROL_START_CONTROL           1 // Used for the start coordinates
#define CONTROL_DEFAULT_CHECKBOX        2
#define CONTROL_DEFAULT_SPIN            3
#define CONTROL_OPTIONS_START           4
#define CONTROL_TITLE_LABEL             100 // Set to 100 to leave room for configs
#define CONTROL_BUTTON_VIDEO            101
#define CONTROL_BUTTON_SOUND            102
#define CONTROL_BUTTON_CORE             103
#define CONTROL_BUTTON_CONTROLS         104

CGUIWindowGameConfigs::CGUIWindowGameConfigs(int iWindowID, std::string strXMLFile) 
: CGUIWindow(iWindowID, strXMLFile)
{
	m_strActiveGame = "";
	m_strActiveGamePath = "";
	m_ConfigCat = CFG_VIDEO;
}

CGUIWindowGameConfigs::~CGUIWindowGameConfigs()
{
}

bool CGUIWindowGameConfigs::OnKey(int iKey)
{
	if(iKey == K_XBOX_B)
	{
		SaveConfigs();
		g_XboxConfigs.SaveGameConfigs(m_strActiveGame);

		RunCommand(ID_FILE_RUN_ISO, m_strActiveGamePath.c_str());
		return true;
	}

	return CGUIWindow::OnKey(iKey);
}

bool CGUIWindowGameConfigs::OnMessage(CGUIMessage message)
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
			RemoveConfigControls();
			break;
		}

		case GUI_MSG_CLICKED:
		{
			if(message.GetControlID() == CONTROL_BUTTON_VIDEO)
			{
				SaveConfigs();
				m_ConfigCat = CFG_VIDEO;
				g_XboxConfigs.SaveGameConfigs(m_strActiveGame);

				g_XboxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);
				return true;
			}

			if(message.GetControlID() == CONTROL_BUTTON_SOUND)
			{
				SaveConfigs();
				m_ConfigCat = CFG_SOUND;
				g_XboxConfigs.SaveGameConfigs(m_strActiveGame);

				g_XboxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);
				return true;
			}

			if(message.GetControlID() == CONTROL_BUTTON_CORE)
			{
				SaveConfigs();
				m_ConfigCat = CFG_CORE;
				g_XboxConfigs.SaveGameConfigs(m_strActiveGame);

				g_XboxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);
				return true;
			}

			if(message.GetControlID() == CONTROL_BUTTON_CONTROLS)
			{
				SaveConfigs();
				m_ConfigCat = CFG_CONTROLS;
				g_XboxConfigs.SaveGameConfigs(m_strActiveGame);

				g_XboxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);
				return true;
			}
			break;
		}

		case GUI_MSG_FILENAME:
		{
			m_strActiveGame = message.GetStringParam1();
			m_strActiveGamePath = message.GetStringParam2();

			CGUIControlLabel* pCtrl = (CGUIControlLabel*)GetControl(CONTROL_TITLE_LABEL);
			if(pCtrl)
				pCtrl->SetLabel(m_strActiveGame + " Config");

			return true;
		}		
	}

	return CGUIWindow::OnMessage(message);
}

bool CGUIWindowGameConfigs::SetupConfigControls() // If our configs go over a one page I need to rework this!
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

	const CXboxConfigs::CfgMap mapConfigs = g_XboxConfigs.GetConfigs(m_ConfigCat);
	typedef CXboxConfigs::CfgMap::const_iterator mapIter;

	int iID = CONTROL_OPTIONS_START;
	int iPosY = pCtrlStart->GetPosY();

	CGUIControl* pNewCtrl = NULL;

	for (mapIter it = mapConfigs.begin(); it != mapConfigs.end(); it++)
	{
		CConfig* pConfig = (*it).second;

		if(pConfig)
		{
			if(pConfig->GetConfigType() != GAME_CFG)
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
			pNewCtrl->SetLeft(CONTROL_BUTTON_VIDEO);
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

void CGUIWindowGameConfigs::RemoveConfigControls()
{
	// Clear out the options we inserted
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

void CGUIWindowGameConfigs::SaveConfigs()
{
	const CXboxConfigs::CfgMap mapConfigs = g_XboxConfigs.GetConfigs(m_ConfigCat);
	typedef CXboxConfigs::CfgMap::const_iterator mapIter;

	for (mapIter it = mapConfigs.begin(); it != mapConfigs.end(); it++)
	{
		CConfig* pConfig = (*it).second;

		if(pConfig)
		{
			if(pConfig->GetConfigType() != GAME_CFG)
				continue;

			// TODO: Change to sending a message to get the values for these!

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
}