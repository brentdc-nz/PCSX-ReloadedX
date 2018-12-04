#ifndef H_CGUIWINDOW_GLOBALCONFIGS
#define H_CGUIWINDOW_GLOBALCONFIGS

#include "..\guiwindow.h"
#include <vector>

class CGUIWindowGlobalConfigs : public CGUIWindow
{
public:
	CGUIWindowGlobalConfigs(int iWindowID, std::string strXMLFile);
	virtual ~CGUIWindowGlobalConfigs();

	virtual bool AllocateResources();
	virtual bool OnMessage(CGUIMessage message);
	virtual void FreeResources();

private:
	std::vector <int>m_vecOptionControls;
	void RemoveConfigControls();
	bool SetupConfigControls();
	void SaveConfigs();
};

#endif //H_CGUIWINDOW_GLOBALCONFIGS
