#ifndef H_CGUIWINDOW_GAMES
#define H_CGUIWINDOW_GAMES

#include "..\guiwindow.h"

class CGUIWindowGames : public CGUIWindow
{
public:
	CGUIWindowGames(int iWindowID, std::string strXMLFile);
	virtual ~CGUIWindowGames();

	virtual bool OnMessage(CGUIMessage message);
	virtual bool OnKey(int iKey);

private:
	bool GetPSXImages();
};

#endif //H_CGUIWINDOW_GAMES
