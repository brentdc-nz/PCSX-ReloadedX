#ifndef H_CGUIWINDOW_HOME
#define H_CGUIWINDOW_HOME

#include "..\guiwindow.h"

class CGUIWindowHome : public CGUIWindow
{
public:
	CGUIWindowHome(int iWindowID, std::string strXMLFile);
	virtual ~CGUIWindowHome();

	virtual bool OnMessage(CGUIMessage message);

private:

};

#endif //H_CGUIWINDOW_HOME
