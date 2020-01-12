#ifndef H_CGUIWINDOW_DIALOG_CDINFO
#define H_CGUIWINDOW_DIALOG_CDINFO

#include "..\guidialog.h"

class CGUIDialogCDInfo : public CGUIDialog
{
public:
	CGUIDialogCDInfo(int iDialogID, std::string strXMLFile);
	~CGUIDialogCDInfo();

	virtual bool OnMessage(CGUIMessage message);
	virtual bool OnKey(int iKey);

private:
	bool CheckDriveCompat(std::string& strDVDDrvModel);
};

#endif //H_CGUIWINDOW_DIALOG_CDINFO