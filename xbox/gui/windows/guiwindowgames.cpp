#include "guiwindowgames.h"
#include "..\controls\guicontrollist.h"
#include "..\utils\stringutils.h"
#include "..\utils\fileutils.h"
#include "..\..\..\libpcsxcore\system.h"
#include "..\guiwindowkeys.h"
#include "..\xboxgui.h"
#include "..\..\xbox.h"
#include "..\guiconfigs.h"
#include "..\..\input\sysxboxinput.h"

using namespace std;

// Control(s) 
#define CONTROL_LIST_IMAGES           1

CGUIWindowGames::CGUIWindowGames(int iWindowID, std::string strXMLFile) 
: CGUIWindow(iWindowID, strXMLFile)
{
}

CGUIWindowGames::~CGUIWindowGames()
{
}

bool CGUIWindowGames::OnMessage(CGUIMessage message)
{
	switch(message.GetMessage())
	{
		case GUI_MSG_WINDOW_INIT:
		{
			// Populate the list with images
			if(!GetPSXImages())
				SysMessage("Unable to load CD Images.\n");
			break;
		}

		case GUI_MSG_CLICKED:
		{
			if(message.GetControlID() == CONTROL_LIST_IMAGES) // A PSX CD image has been clicked
			{
				CGUIControlList* pCtrlPSXCDList = (CGUIControlList*)GetControl(CONTROL_LIST_IMAGES);

				CGUIListItem* pItem = pCtrlPSXCDList->GetSelectedItem();

				if(pItem)
				{
					// If we don't have a config for this game open the config window
					if(!g_XboxConfigs.LoadGameConfigs(pItem->GetName()))
					{
						std::string strGameFilename = pItem->GetName(); // Store it now as we are decativating the window

						g_XboxConfigs.LoadGameConfigs("default"); // Load our defaults to use for new configs created

						g_XBoxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);

						CGUIMessage msg(GUI_MSG_FILENAME, GetID(), 0, strGameFilename);
						g_XBoxGUI.SendMessage(msg);
					}
					else
					{
						string strPath = CDIMAGE_PATH + pItem->GetName();
						RunCommand(ID_FILE_RUN_ISO, strPath.c_str());
					}

					return true;
				}
			}
		}
		break;
	}
	return CGUIWindow::OnMessage(message);
}

bool CGUIWindowGames::OnKey(int iKey)
{
	if(iKey == K_XBOX_Y)
	{
		std::string strFilename;

		CGUIControlList* pControlPSXCDImages = NULL;
		pControlPSXCDImages = (CGUIControlList*)GetControl(CONTROL_LIST_IMAGES);
	
		if(pControlPSXCDImages)
		{
			CGUIListItem* pItem = pControlPSXCDImages->GetSelectedItem();
			if(pItem)
				strFilename = pItem->GetName(); // Store it before we close the current window
		
			if(!g_XboxConfigs.LoadGameConfigs(pItem->GetName()))
				g_XboxConfigs.LoadGameConfigs("default");

		}

		g_XBoxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);

		CGUIMessage msg(GUI_MSG_FILENAME, GetID(), 0, strFilename);
		g_XBoxGUI.SendMessage(msg);

		return true;
	}

	return CGUIWindow::OnKey(iKey);
}

bool CGUIWindowGames::GetPSXImages()
{
	CGUIControlList* pControlPSXCDImages = NULL;
	pControlPSXCDImages = (CGUIControlList*)GetControl(CONTROL_LIST_IMAGES);

	if(!pControlPSXCDImages)
		return false;

	int iID = 0;

	std::vector<std::string> vecCDImages;
	CFileUtils::GetFilesInDirectory(CDIMAGE_PATH, vecCDImages, CFileUtils::ISO_FILE | CFileUtils::CUE_FILE);

	for(int i=0; i < (int)vecCDImages.size(); i++)
	{
		iID++;
		CGUIListItem* pNewItem = new CGUIListItem(vecCDImages[i]); // Released in CGUIListItem::Cleanup()	
		pControlPSXCDImages->AddItem(pNewItem);
	}

	return true;
}
