#ifndef H_CGUIDIALOG
#define H_CGUIDIALOG

#include "guiwindow.h"
#include "guiwindowkeys.h"

class CGUIDialog : public CGUIWindow
{
public:
	CGUIDialog(int iDialogID, std::string strXMLFile);
	~CGUIDialog();

	virtual bool OnMessage(CGUIMessage& message);
	virtual bool IsDialog() const { return true; };
	virtual bool IsRunning() const { return m_bRunning; };
	virtual bool OnBack();
	void Close();
	void DoModal(int iParentId, int iWindowID = WINDOW_INVALID);

protected:
	bool m_bRunning;
};

#endif //H_CGUIDIALOG