#include <xtl.h>
#include "guiwindowhome.h"
#include "..\..\..\libpcsxcore\system.h"

// Control(s)
#define CONTROL_BUTTON_SHUTDOWN    4
#define CONTROL_BUTTON_QUITDASH    5

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
		}
	}

	return CGUIWindow::OnMessage(message);
}