#ifndef H_CGUIWINDOW_GAME_CONFIGS
#define H_CGUIWINDOW_GAME_CONFIGS

#include "..\guiwindow.h"
#include "..\guimessage.h"

class CGUIWindowGameConfigs : public CGUIWindow
{
public:
	CGUIWindowGameConfigs(int iWindowID, std::string strXMLFile);
	virtual ~CGUIWindowGameConfigs();

	virtual bool OnKey(int iKey);
	virtual bool OnMessage(CGUIMessage message);

private:
	bool SetupConfigControls();
	void RemoveConfigControls();
	void SaveConfigs();
	std::vector <int>m_vecOptionControls;
	std::string m_strActiveGame;
	std::string m_strActiveGamePath;
	int m_ConfigCat;
};

#endif //H_CGUIWINDOW_GAME_CONFIGS
