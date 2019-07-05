#include "guidialogcdinfo.h"
#include "..\..\input\sysxboxinput.h"
#include "..\..\xbox.h"

CGUIDialogCDInfo::CGUIDialogCDInfo(int iDialogID, std::string strXMLFile)
    : CGUIDialog(iDialogID, strXMLFile)
{
}

CGUIDialogCDInfo::~CGUIDialogCDInfo()
{
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