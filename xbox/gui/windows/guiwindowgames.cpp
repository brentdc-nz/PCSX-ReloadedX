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
			if(!GetDirectory(CDIMAGE_PATH, true))
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
					if(pItem->IsDirectory())
					{
						std::string strDirName = pItem->GetName();
						std::string strFullPath = pItem->GetFullPath();

						pCtrlPSXCDList->Clear(); // Clear out our current items

						if(strDirName == "\\..") // HACK: Need a better way to identify
						{
							if(strFullPath == "D:\\psxcds\\") // Check if it's the root dir
								GetDirectory(CDIMAGE_PATH, true);
							else
								GetDirectory(strFullPath);
						}
						else
							GetDirectory(strFullPath);
					}
					else
					{
						// If we don't have a config for this game open the config window
						if(!g_XboxConfigs.LoadGameConfigs(pItem->GetName()))
						{
							// Store the strings now as we are decativating the window
							std::string strGameFilename = pItem->GetName();
							std::string strGameFullPath = pItem->GetFullPath();

							g_XboxConfigs.LoadGameConfigs("default"); // Load our defaults to use for new configs created

							g_XBoxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);

							CGUIMessage msg(GUI_MSG_FILENAME, GetID(), 0, strGameFilename, strGameFullPath);
							g_XBoxGUI.SendMessage(msg);
						}
						else
						{
							string strPath = pItem->GetFullPath();
							RunCommand(ID_FILE_RUN_ISO, strPath.c_str());
						}
						return true;
					}
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
		std::string strFilename, strGameFullPath;

		CGUIControlList* pControlPSXCDImages = NULL;
		pControlPSXCDImages = (CGUIControlList*)GetControl(CONTROL_LIST_IMAGES);
	
		if(pControlPSXCDImages)
		{
			CGUIListItem* pItem = pControlPSXCDImages->GetSelectedItem();
			if(pItem)
			{
				// Store the strings before we close the current window
				strFilename = pItem->GetName();
				strGameFullPath = pItem->GetFullPath();
			}
		
			if(!g_XboxConfigs.LoadGameConfigs(pItem->GetName()))
				g_XboxConfigs.LoadGameConfigs("default");

		}

		g_XBoxGUI.ActivateWindow(WINDOW_GAME_CONFIGS);

		CGUIMessage msg(GUI_MSG_FILENAME, GetID(), 0, strFilename, strGameFullPath);
		g_XBoxGUI.SendMessage(msg);

		return true;
	}

	return CGUIWindow::OnKey(iKey);
}

bool CGUIWindowGames::GetDirectory(std::string strPath, bool bIsRoot)
{
	CGUIControlList* pControlPSXCDImages = NULL;
	pControlPSXCDImages = (CGUIControlList*)GetControl(CONTROL_LIST_IMAGES);

	if(!pControlPSXCDImages)
		return false;

	std::vector<PairStrBool> vecCDImages;
	CFileUtils::GetFilesInDirectory(strPath, vecCDImages, CFileUtils::DIRECTORY | CFileUtils::ISO_FILE | CFileUtils::CUE_FILE | CFileUtils::BIN_FILE | CFileUtils::IMG_FILE);

	if(!bIsRoot) // If directory add a entry to return to the parent
	{
		// Get the parent directory then add it in
		std::string strParentDir = strPath.substr(0, strPath.find_last_of("\\/"));
		pControlPSXCDImages->AddItem(new CGUIListItem("\\..", strParentDir, true));
	}

	for(int i=0; i < (int)vecCDImages.size(); i++) // These are released in CGUIListItem::Cleanup()
	{
		CGUIListItem* pNewItem = NULL;

		if(vecCDImages[i].second)
			pNewItem = new CGUIListItem("(DIR) "+vecCDImages[i].first, strPath+"\\"+vecCDImages[i].first, vecCDImages[i].second);
		else
			pNewItem = new CGUIListItem(vecCDImages[i].first, strPath+"\\"+vecCDImages[i].first, vecCDImages[i].second);

		if(pNewItem)
			pControlPSXCDImages->AddItem(pNewItem);
	}

	return true;
}