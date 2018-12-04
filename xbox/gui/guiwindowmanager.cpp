#include "guiwindowmanager.h"
#include "guiwindowkeys.h"
#include "guimessage.h"

using namespace std;

CGUIWindowManager::CGUIWindowManager()
{
	m_bInitialized = false;
}

CGUIWindowManager::~CGUIWindowManager()
{
}

bool CGUIWindowManager::Initialize()
{
	for (WindowMap::iterator it = m_mapWindows.begin(); it != m_mapWindows.end(); it++)
	{
		CGUIWindow *pWindow = (*it).second;

		pWindow->FreeResources(true);
		pWindow->Initialize();
	}

	m_bInitialized = true;
	return true;
}

void CGUIWindowManager::AddWindow(CGUIWindow* pWindow)
{
	if (!pWindow)
		return;

	m_mapWindows.insert(pair<int, CGUIWindow*>(pWindow->GetID(), pWindow));
}

void CGUIWindowManager::DeInitialize()
{
	for (WindowMap::iterator it = m_mapWindows.begin(); it != m_mapWindows.end(); it++)
	{
		CGUIWindow* pWindow = (*it).second;
		CGUIMessage msg(GUI_MSG_WINDOW_DEINIT, 0, pWindow->GetID(), 0);
		pWindow->OnMessage(msg);
		pWindow->FreeResources(true);
	}

	m_bInitialized = false;
}

void CGUIWindowManager::ClearWindowHistory()
{
	while (m_windowHistory.size())
		m_windowHistory.pop();
}

CGUIWindow* CGUIWindowManager::GetWindow(int id) const
{
	if (id == WINDOW_INVALID)
		return NULL;

	WindowMap::const_iterator it = m_mapWindows.find(id);

	if (it != m_mapWindows.end())
		return (*it).second;

	return NULL;
}

int CGUIWindowManager::GetActiveWindow() const
{
	if (!m_windowHistory.empty())
		return m_windowHistory.top();

	return WINDOW_INVALID;
}

void CGUIWindowManager::ActivateWindow(int iWindowID)
{
	// First check existence of the window we wish to activate
	CGUIWindow *pNewWindow = GetWindow(iWindowID);
	if (!pNewWindow)
	{ 
		// Nothing to see here - move along
		return;
	}

	// Deactivate any window
	int currentWindow = GetActiveWindow();
	CGUIWindow *pWindow = GetWindow(currentWindow);
	if (pWindow)
	{
		CGUIMessage msg(GUI_MSG_WINDOW_DEINIT, currentWindow, iWindowID);
		pWindow->OnMessage(msg);
	}
 
	// Add window to the history list (we must do this before we activate it,
	// as all messages done in WINDOW_INIT will want to be sent to the new
	// topmost window).  If we are swapping windows, we pop the old window
	// off the history stack
	if (m_windowHistory.size())
		m_windowHistory.pop();
	
	AddToWindowHistory(iWindowID);

	// Send the init message
	CGUIMessage msg(GUI_MSG_WINDOW_INIT, currentWindow, iWindowID);
	pNewWindow->OnMessage(msg);
}

void CGUIWindowManager::PreviousWindow()
{
	int currentWindow = GetActiveWindow();
	CGUIWindow *pCurrentWindow = GetWindow(currentWindow);
	if (!pCurrentWindow)
		return; // No windows or window history yet

	// Get the previous window in our stack
	if (m_windowHistory.size() < 2)
	{
		// No previous window history yet - check if we should just activate home
		if (GetActiveWindow() != WINDOW_INVALID && GetActiveWindow() != WINDOW_HOME)
		{
		//	ClearWindowHistory();
			ActivateWindow(WINDOW_HOME);
		}
		return;
	}

	m_windowHistory.pop();
	int previousWindow = GetActiveWindow();
	m_windowHistory.push(currentWindow);

	CGUIWindow *pNewWindow = GetWindow(previousWindow);
	if (!pNewWindow)
	{
		ClearWindowHistory();
		ActivateWindow(WINDOW_HOME);
		return;
	}

	// Deinitialize our window
	CGUIMessage msg(GUI_MSG_WINDOW_DEINIT, 0, 0);
	pCurrentWindow->OnMessage(msg);

	// Remove the current window off our window stack
	m_windowHistory.pop();

	// Ok, initialize the new window
	CGUIMessage msg2(GUI_MSG_WINDOW_INIT, WINDOW_INVALID, GetActiveWindow());
	pNewWindow->OnMessage(msg2);
}

bool CGUIWindowManager::OnKey(int iKey)
{
	CGUIWindow* pWindow = GetWindow(GetActiveWindow());
	
	if (pWindow)
		return pWindow->OnKey(iKey);

	return false;
}

bool CGUIWindowManager::OnMessage(CGUIMessage message)
{
	CGUIWindow* pWindow = GetWindow(GetActiveWindow());

	if(pWindow)
       return pWindow->OnMessage(message);
    
	return false;
}

void CGUIWindowManager::Render()
{
	CGUIWindow* pWindow = GetWindow(GetActiveWindow());
	
	if (pWindow)
	{
//		pWindow->ClearBackground(); //TODO
		pWindow->Render();
	}
}

void CGUIWindowManager::AddToWindowHistory(int iNewWindowID)
{
	// Check the window stack to see if this window is in our history,
	// and if so, pop all the other windows off the stack so that we
	// always have a predictable "Back" behaviour for each window
	stack<int> historySave = m_windowHistory;
	while (historySave.size())
	{
		if (historySave.top() == iNewWindowID)
			break;

		historySave.pop();
	}
	if (!historySave.empty())	
		m_windowHistory = historySave; // Found window in history
	else	
		m_windowHistory.push(iNewWindowID); // Didn't find window in history, add it to the stack
}