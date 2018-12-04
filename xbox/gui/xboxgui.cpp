#include "xboxgui.h"
#include "guiwindowkeys.h"
#include "..\..\libpcsxcore\system.h"
#include "..\input\sysxboxinput.h"
#include "guiconfigs.h"

// Windows
#include "windows\guiwindowhome.h"
#include "windows\guiwindowgames.h"
#include "windows\guiwindowgameconfigs.h"
#include "windows\guiwindowglobalconfigs.h"

CXBoxGUI g_XBoxGUI;

CXBoxGUI::CXBoxGUI()
{
	m_bGUIActive = false;
	m_bGUIStatesCaptured = false;
	m_bInGameStatesCaptured = false;
	m_dwSkinTime = 0;
	m_strMediaDir = "";
	m_surfPreview = NULL;
	m_iStartWindow = WINDOW_HOME;
}

CXBoxGUI::~CXBoxGUI()
{
}

bool CXBoxGUI::Initialize()
{
	if(!m_pD3DDevice)
	{
		SysMessage("Can't start the GUI, no D3D device.\n");
		return false;
	}

	// Change D3D device parameters here, valid only for the GUI
	m_PresentParams.MultiSampleType = D3DMULTISAMPLE_NONE; // AA Screws up fonts in the GUI!
	m_PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// Reset the device in order to use the changed params
	m_pD3DDevice->Reset(&m_PresentParams);

	if(!m_bGUIStatesCaptured)
	{
		m_bGUIStatesCaptured = true;
		m_pD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_dwGUIRenderStates);
	}
	else
	{
		m_pD3DDevice->GetBackBuffer(-1, D3DBACKBUFFER_TYPE_MONO, &m_surfPreview);

		m_pD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_dwInGameRenderStates);
		m_bInGameStatesCaptured = true;
	}

	// Create our windows
	m_GUIWindowManager.AddWindow(new CGUIWindowHome(WINDOW_HOME, "home.xml"));
	m_GUIWindowManager.AddWindow(new CGUIWindowGames(WINDOW_GAMES, "games.xml"));
	m_GUIWindowManager.AddWindow(new CGUIWindowGlobalConfigs(WINDOW_GLOBAL_CONFIGS, "globalconfigs.xml"));
	m_GUIWindowManager.AddWindow(new CGUIWindowGameConfigs(WINDOW_GAME_CONFIGS, "gameconfigs.xml"));

	LoadSkin(g_XboxConfigs.GetString("gui.skin"));

	m_GUIWindowManager.ActivateWindow(m_iStartWindow);
	
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3DDevice->ApplyStateBlock(m_dwGUIRenderStates);

	m_bGUIActive = true;

	return true;
}

void CXBoxGUI::Run()
{
	int iKey;

	if(GetGUIXBoxInput(&iKey))
		m_GUIWindowManager.OnKey(iKey);

	// Check if we need to load a new skin
	if(m_dwSkinTime && GetTickCount() >= m_dwSkinTime)
		ReloadSkin();

	Render();
}

void CXBoxGUI::Render()
{
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);	

	m_GUIWindowManager.Render();

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool CXBoxGUI::Close()
{
	UnloadSkin();

	if(m_bInGameStatesCaptured)
		m_pD3DDevice->ApplyStateBlock(m_dwInGameRenderStates);

	// Release the preview surface if we have one
	if(m_surfPreview)
	{
		m_surfPreview->Release();
		m_surfPreview = NULL;
	}

	m_bGUIActive = false;

	// Set AA modes for emulation
	switch(g_XboxConfigs.GetInt("video.antialiasing"))
	{
		case MULTISAMPLE_NONE:
			m_PresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
			break;

		case MULTISAMPLE_2:
			m_PresentParams.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES_MULTISAMPLE_LINEAR;
			break;

		case MULTISAMPLE_4:
			m_PresentParams.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES_MULTISAMPLE_LINEAR;
			break;

		default:
			m_PresentParams.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES_MULTISAMPLE_LINEAR;
			break;
	};

	// VSync ON:	   D3DPRESENT_INTERVAL_ONE
	// VSync OFF:	   D3DPRESENT_INTERVAL_IMMEDIATE 
	// Vsync Adaptive: D3DPRESENT_INTERVAL_ONE_OR_IMMEDIATE

	// The driver supports a presentation swap interval every screen refresh but it will flip 
	// immediately if the rendering has gone over a frame. It otherise behaves like D3DPRESENT_INTERVAL_ONE
	switch(g_XboxConfigs.GetInt("video.vsync"))
	{
		case VSYNC_OFF:
			m_PresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			break;

		case VSYNC_ON:
			m_PresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			break;

		case VSYNC_ADAPTIVE:
			m_PresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE_OR_IMMEDIATE;
			break;

		default:
			m_PresentParams.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			break;
	};

	m_PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;//COPY; //FIXME: Swap Back!

	// Reset the device in order to use the changed params
	m_pD3DDevice->Reset(&m_PresentParams);

	return true;
}

D3DSurface* CXBoxGUI::GetPreviewSurface() //TODO: Pass by reference and return a bool
{
	// Check if we have an in-game capture yet
	if(m_bInGameStatesCaptured)
		return m_surfPreview;

	return NULL;
}

void CXBoxGUI::SetStartWindow(int iStartWindow)
{
	// Validity is checked in CWindowManager
	m_iStartWindow = iStartWindow;
}


void CXBoxGUI::LoadSkin(std::string strSkinName)
{
	m_dwSkinTime = 0;

	m_strMediaDir = "D:\\skins\\" + strSkinName + "\\";

	UnloadSkin();

	m_GUITextureManager.Initialize();
	m_GUIFontManager.Initialize();
	m_GUIWindowManager.Initialize();
}

void CXBoxGUI::UnloadSkin()
{
	m_GUIWindowManager.DeInitialize();
	m_GUIFontManager.Cleanup();
	m_GUITextureManager.ReleaseAllTextures();
}

void CXBoxGUI::ReloadSkin()
{
	LoadSkin(g_XboxConfigs.GetString("gui.skin"));
	m_GUIWindowManager.ActivateWindow(WINDOW_GLOBAL_CONFIGS);
}

void CXBoxGUI::DelaySkinLoad()
{
	m_dwSkinTime = GetTickCount() + 2000;
}

//==================================================================================

int InitializeXBoxGUI()
{
	return g_XBoxGUI.Initialize();
}

void RunXBoxGUI()
{
	g_XBoxGUI.Run();
}

int CloseXBoxGUI()
{
	return g_XBoxGUI.Close();
}

void SetGUID3DDevice(LPDIRECT3DDEVICE8 pD3DDevice, D3DPRESENT_PARAMETERS PresentParams)
{
	g_XBoxGUI.SetD3DDevice(pD3DDevice, PresentParams);
}

int IsXBoxGUIActive()
{
	return g_XBoxGUI.IsGUIActive();
}

void SetXBoxGUIStartWindow(int iStartWindow)
{
	g_XBoxGUI.SetStartWindow(iStartWindow);
}