#ifndef H_CGUIWINDOW_DIALOG_CDINFO
#define H_CGUIWINDOW_DIALOG_CDINFO

#include "..\guidialog.h"

class CGUIDialogCDInfo : public CGUIDialog
{
public:
	CGUIDialogCDInfo(int iDialogID, std::string strXMLFile);
	~CGUIDialogCDInfo();

	virtual bool OnKey(int iKey);
};

#endif //H_CGUIWINDOW_DIALOG_CDINFO