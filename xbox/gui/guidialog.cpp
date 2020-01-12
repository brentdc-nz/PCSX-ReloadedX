#include "guidialog.h"
#include "xboxgui.h"

CGUIDialog::CGUIDialog(int iDialogID, std::string strXMLFile)
: CGUIWindow(iDialogID, strXMLFile)
{
	m_bRunning = false;
}

CGUIDialog::~CGUIDialog()
{
}

bool CGUIDialog::OnMessage(CGUIMessage& message)
{
	switch (message.GetMessage())
	{
		case GUI_MSG_WINDOW_DEINIT:
		{
			CGUIWindow::OnMessage(message);

			// If we were running, make sure we remove ourselves from the window manager
			if (m_bRunning)
			{
				g_XboxGUI.GetWindowManager().RemoveDialog(GetID());
				m_bRunning = false;
			}
			return true;
		}

		case GUI_MSG_WINDOW_INIT:
		{
			CGUIWindow::OnMessage(message);
			return true;
		}
	}
	return CGUIWindow::OnMessage(message);
}

bool CGUIDialog::OnBack()
{
	Close();
	return true;
}

void CGUIDialog::Close()
{
	if (!m_bRunning) return;

	CGUIMessage msg(GUI_MSG_WINDOW_DEINIT, 0, 0);

	OnMessage(msg);
}

void CGUIDialog::DoModal(int iParentId, int iWindowID /*= WINDOW_INVALID */)
{
	// Set running before it's added to the window manager, else the auto-show code
	// could show it as well if we are in a different thread from
	// the main rendering thread (this should really be handled via
	// a thread message though IMO)
	m_bRunning = true;
	
	g_XboxGUI.GetWindowManager().RouteToWindow(this);

	// Active this window...
	CGUIMessage msg(GUI_MSG_WINDOW_INIT, WINDOW_INVALID, iWindowID);
	g_XboxGUI.GetWindowManager().OnMessage(msg);
}