#ifndef H_CGUIWINDOWMANAGER
#define H_CGUIWINDOWMANAGER

#include "guiwindow.h"

#include <map>
#include <stack>
#include <vector>

class CGUIWindowManager
{
public:
	CGUIWindowManager();
	~CGUIWindowManager();

	bool Initialize();
	void AddWindow(CGUIWindow* pWindow);
	void RouteToWindow(CGUIWindow* pDialog);
	void RemoveDialog(int iID);
	void DeInitialize();
	void Delete(int iID);
	void Remove(int iID);
	void ClearWindowHistory();
	void ActivateWindow(int iWindowID);
	void PreviousWindow();
	CGUIWindow* GetWindow(int id) const;
	int GetActiveWindow() const;
	bool OnKey(int iKey);
	bool OnMessage(CGUIMessage message);
	void Render();
	void RenderDialogs();

	bool IsInitialized() { return m_bInitialized; };

private:
	void AddToWindowHistory(int iNewWindowID);
	
	bool m_bInitialized;

	std::vector <CGUIWindow*> m_activeDialogs;
	std::stack<int> m_windowHistory;
	typedef std::map<int, CGUIWindow*> WindowMap;
	WindowMap m_mapWindows;
};

#endif //H_CGUIWINDOWMANAGER