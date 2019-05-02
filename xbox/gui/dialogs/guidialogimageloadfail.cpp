#include "guidialogimageloadfail.h"
#include "..\..\input\sysxboxinput.h"

CGUIDialogImageLoadFail::CGUIDialogImageLoadFail(int iDialogID, std::string strXMLFile)
    : CGUIDialog(iDialogID, strXMLFile)
{
}

CGUIDialogImageLoadFail::~CGUIDialogImageLoadFail()
{
}

bool CGUIDialogImageLoadFail::OnKey(int iKey)
{
	if(iKey == K_XBOX_A)
	{
		Close();
		return true;
	}

	return CGUIDialog::OnKey(iKey);
}