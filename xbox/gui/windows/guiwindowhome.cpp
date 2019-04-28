#include "..\..\xbox.h"
#include "..\xboxgui.h"
#include "..\guiwindowkeys.h"
#include "..\..\..\libpcsxcore\system.h"
#include "guiwindowhome.h"

// Controls
#define CONTROL_BUTTON_BOOTPSXCD   2
#define CONTROL_BUTTON_SHUTDOWN    4
#define CONTROL_BUTTON_QUITDASH    5
#define CONTROL_LABEL_VERSION      6

CGUIWindowHome::CGUIWindowHome(int iWindowID, std::string strXMLFile) 
: CGUIWindow(iWindowID, strXMLFile)
{
}

CGUIWindowHome::~CGUIWindowHome()
{
}

bool CGUIWindowHome::OnMessage(CGUIMessage message)
{
	switch(message.GetMessage())
	{
		case GUI_MSG_WINDOW_INIT:
		{
			// Set the version number
			CGUIMessage msg(GUI_MSG_SET_LABEL, GetID(), CONTROL_LABEL_VERSION, XBOX_VERSION);
			g_XboxGUI.SendMessage(msg);
			break;
		}

		case GUI_MSG_CLICKED:
		{
			if(message.GetControlID() == CONTROL_BUTTON_QUITDASH)
			{
				SysClose(FALSE);
				break;
			}

			if(message.GetControlID() == CONTROL_BUTTON_SHUTDOWN)
			{
				SysClose(TRUE);
				break;
			}	

			if(message.GetControlID() == CONTROL_BUTTON_BOOTPSXCD)
			{
				g_XboxGUI.ActivateWindow(DIALOG_CDINFO);
				break;
			}				
		}
	}

	return CGUIWindow::OnMessage(message);
}