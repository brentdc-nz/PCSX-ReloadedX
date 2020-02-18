#include "guidialogcdinfo.h"
#include "..\..\input\sysxboxinput.h"
#include "..\..\xbox.h"
#include "..\..\glue\sys\xboxata.h"
#include "..\controls\guicontrollabel.h"

// Control(s)
#define CONTROL_LABEL_DVDDRVMODEL   01
#define CONTROL_LABEL_COMPATRESULT  02

CGUIDialogCDInfo::CGUIDialogCDInfo(int iDialogID, std::string strXMLFile)
    : CGUIDialog(iDialogID, strXMLFile)
{
}

CGUIDialogCDInfo::~CGUIDialogCDInfo()
{
}

bool CGUIDialogCDInfo::OnMessage(CGUIMessage message)
{
	switch(message.GetMessage())
	{
		case GUI_MSG_WINDOW_INIT:
		{
			std::string strDriveModel("");

			// Get the drive model label
			CGUIControlLabel* pCtrlDVDDrvLbl = NULL;
			pCtrlDVDDrvLbl = (CGUIControlLabel*)GetControl(CONTROL_LABEL_DVDDRVMODEL);

			// Now get the result label
			CGUIControlLabel* pCtrlCompatRslt = NULL;
			pCtrlCompatRslt = (CGUIControlLabel*)GetControl(CONTROL_LABEL_COMPATRESULT);

			// Check if the drive is compatible
			if(CheckDriveCompat(strDriveModel))
			{
				if(pCtrlCompatRslt)
				{
					pCtrlCompatRslt->SetLabel("Drive is IS compatible :-)");
					pCtrlCompatRslt->SetColor(D3DCOLOR_XRGB(0, 255, 0));
				}
			}
			else
			{
				if(pCtrlCompatRslt)
				{
					pCtrlCompatRslt->SetLabel("Drive is NOT compatible :-(");
					pCtrlCompatRslt->SetColor(D3DCOLOR_XRGB(255, 0, 0));
				}
			}

			if(pCtrlDVDDrvLbl)
				pCtrlDVDDrvLbl->SetLabel("DVD-Rom model: " + strDriveModel);

			break;
		}
	}

	return CGUIWindow::OnMessage(message);
}

bool CGUIDialogCDInfo::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{	
		Close();
		RunCommand(ID_CD_RUN, "");
		return true;
	}

	return CGUIDialog::OnKey(iKey);
}

// Checked if the Xbox has a DVD drive
// that can boot a PlayStation 1 CD
// only Samsung Rev B and Philips drives
// have been confirmed to work so far
bool CGUIDialogCDInfo::CheckDriveCompat(std::string &strDVDDrvModel)
{
	CXboxATA::ATA_COMMAND_OBJ hddcommand;
	ZeroMemory(&hddcommand, sizeof(CXboxATA::ATA_COMMAND_OBJ));

	hddcommand.DATA_BUFFSIZE = 0x200;
	hddcommand.IPReg.bCommandReg = ATA_IDENTIFY_PACKET_DEVICE;

	if(CXboxATA::SendATACommand(XBOX_DEVICE_DVDROM, &hddcommand, IDE_COMMAND_READ))
	{
		CHAR lpsDVDModel[256];
		ZeroMemory(&lpsDVDModel, sizeof(lpsDVDModel));
		CXboxATA::GetIDEModel(hddcommand.DATA_BUFFER, lpsDVDModel);

		std::string strDVDModel(lpsDVDModel);
		strDVDDrvModel = strDVDModel;

		// TODO: Check if comparing strings is our only option, 
		//       or can get a enum from the kernel?

		// Compatible drives

		// Philips
		if(!strDVDModel.compare(strDVDModel.length()-5, 5, "3235C"))
			return true;

		// Samsung Rev B
		if(!strDVDModel.compare(strDVDModel.length()-8, 8, "SDG-605B"))
			return true;

		// Neither of the working drives found :(
		return false;
	}

	strDVDDrvModel = "Failed to detect DVD Rom model";
	return false;
}