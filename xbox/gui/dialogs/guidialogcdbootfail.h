#ifndef H_CGUIWINDOW_DIALOG_CDBOOTFAIL
#define H_CGUIWINDOW_DIALOG_CDBOOTFAIL

#include "..\guidialog.h"

class CGUIDialogCDBootFail : public CGUIDialog
{
public:
	CGUIDialogCDBootFail(int iDialogID, std::string strXMLFile);
	~CGUIDialogCDBootFail();

	virtual bool OnKey(int iKey);
};

#endif //H_CGUIWINDOW_DIALOG_CDBOOTFAIL