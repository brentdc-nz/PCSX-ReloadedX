#include "guidialogcdbootfail.h"
#include "..\..\input\sysxboxinput.h"

CGUIDialogCDBootFail::CGUIDialogCDBootFail(int iDialogID, std::string strXMLFile)
    : CGUIDialog(iDialogID, strXMLFile)
{
}

CGUIDialogCDBootFail::~CGUIDialogCDBootFail()
{
}

bool CGUIDialogCDBootFail::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		Close();
		return true;
	}

	return CGUIDialog::OnKey(iKey);
}