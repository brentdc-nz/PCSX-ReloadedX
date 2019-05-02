#ifndef H_CGUIWINDOW_DIALOG_IMAGELOADFAIL
#define H_CGUIWINDOW_DIALOG_IMAGELOADFAIL

#include "..\guidialog.h"

class CGUIDialogImageLoadFail : public CGUIDialog
{
public:
	CGUIDialogImageLoadFail(int iDialogID, std::string strXMLFile);
	~CGUIDialogImageLoadFail();

	virtual bool OnKey(int iKey);
};

#endif //H_CGUIWINDOW_DIALOG_IMAGELOADFAIL