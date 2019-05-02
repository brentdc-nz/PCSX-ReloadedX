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

// Dialogs
#include "dialogs\guidialogimageloadfail.h"
#include "dialogs\guidialogcdbootfail.h"
#include "dialogs\guidialogcdinfo.h"

CXboxGUI g_XboxGUI;

CXboxGUI::CXboxGUI()
{
	m_bGUIActive = false;
	m_bGUIStatesCaptured = false;
	m_bInGameStatesCaptured = false;
	m_dwSkinChangeTime = 0;
	m_dwResChangeTime = 0;
	m_strMediaDir = "";
	m_surfPreview = NULL;
	m_iStartWindow = WINDOW_HOME;
}

CXboxGUI::~CXboxGUI()
{
}

bool CXboxGUI::Initialize()
{
	if(!m_pD3DDevice)
	{
		SysMessage("Can't start the GUI, no D3D device.\n");
		return false;
	}

	m_InGameOSD.DeInitialize(); // Release any ingame OSD stuff

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

	// Dialogs
	m_GUIWindowManager.AddWindow(new CGUIDialogCDInfo(DIALOG_CDINFO, "dialogcdinfo.xml"));
	m_GUIWindowManager.AddWindow(new CGUIDialogImageLoadFail(DIALOG_IMAGELOADFAIL, "dialogimageloadfail.xml"));
	m_GUIWindowManager.AddWindow(new CGUIDialogCDBootFail(DIALOG_CDBOOTFAIL, "dialogcdbootfail.xml"));

	LoadSkin(g_XboxConfigs.GetString("gui.skin"));

	m_GUIWindowManager.ActivateWindow(m_iStartWindow);
	
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->ApplyStateBlock(m_dwGUIRenderStates);

	m_iCurrentResolution = g_XboxConfigs.GetInt("video.resolution");

	m_bGUIActive = true;

	return true;
}

void CXboxGUI::Run()
{
	int iKey;

	if(GetGUIXBoxInput(&iKey))
		m_GUIWindowManager.OnKey(iKey);

	CheckGUIOptionChanges();

	Render();
}

void CXboxGUI::CheckGUIOptionChanges()
{
	// Check if we should be playing background music
	if(g_XboxConfigs.GetBool("sound.guibgmusic") && !m_GUIBGMusic.IsPlaying())
		m_GUIBGMusic.Start();
	else if(!g_XboxConfigs.GetBool("sound.guibgmusic") && m_GUIBGMusic.IsPlaying())
		m_GUIBGMusic.Stop();

	// Check if we need to load a new skin
	if(m_dwSkinChangeTime && GetTickCount() >= m_dwSkinChangeTime)
		ReloadSkin();

	// Check if the resolution has changed
	if(g_XboxConfigs.GetInt("video.resolution") != m_iCurrentResolution)
	{
		m_dwResChangeTime = GetTickCount() + 2000;
		m_iCurrentResolution = g_XboxConfigs.GetInt("video.resolution");
	}

//	WIP: Rather than the quick and dirty method of reloading the xbe 
//       reset the device with the new resolution and reload the skin
	if(m_dwResChangeTime && GetTickCount() >= m_dwResChangeTime)
#if 1
		XLaunchNewImage("D:\\default.xbe", NULL); // TODO: Clean up properly before reloading the xbe
#endif
#if 0 // Reset the device with the new resolution and reload skin (WIP not working properly yet)
	{
		int iScrWidth, iScrHeight;

		g_XboxConfigs.GetScreenSize(&iScrWidth, &iScrHeight);

		m_PresentParams.BackBufferWidth = iScrWidth;
		m_PresentParams.BackBufferHeight = iScrHeight;

		m_pD3DDevice->Reset(&m_PresentParams);

//		D3D_PostResetRestore();

		ReloadSkin();

		m_dwResChangeTime = 0;
	}
#endif
}

void CXboxGUI::Render()
{
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);	

	m_GUIWindowManager.Render();

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool CXboxGUI::Close()
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

	// Release the background music if enabled
	m_GUIBGMusic.Release();

	// Delete all the old windows and dialogs

	// Windows
	m_GUIWindowManager.Delete(WINDOW_HOME);
	m_GUIWindowManager.Delete(WINDOW_GAMES);
	m_GUIWindowManager.Delete(WINDOW_GLOBAL_CONFIGS);
	m_GUIWindowManager.Delete(WINDOW_GAME_CONFIGS);

	// Dialogs
	m_GUIWindowManager.Delete(DIALOG_CDBOOTFAIL);
	m_GUIWindowManager.Delete(DIALOG_CDINFO);

	if(g_XboxConfigs.GetBool("video.showfps") || g_XboxConfigs.GetBool("video.showfreememory"))
		m_InGameOSD.Initialize();

	return true;
}

D3DSurface* CXboxGUI::GetPreviewSurface() //TODO: Pass by reference and return a bool
{
	// Check if we have an in-game capture yet
	if(m_bInGameStatesCaptured)
		return m_surfPreview;

	return NULL;
}

void CXboxGUI::SetStartWindow(int iStartWindow)
{
	// Validity is checked in CWindowManager
	m_iStartWindow = iStartWindow;
}

void CXboxGUI::LoadSkin(std::string strSkinName)
{
	m_dwSkinChangeTime = 0;
	
	std::string strSkinRes;

	// We force this config back to 480P in main() if component
	// cables are not detected so there is no need to check here	
	switch(g_XboxConfigs.GetInt("video.resolution"))
	{
		case RESOLUTION_1080I: // Not used, here for testing
		strSkinRes = "1080i";
		break;		
		
		case RESOLUTION_720P:
		strSkinRes = "720p";
		break;
	
		case RESOLUTION_480P:
		strSkinRes = "480p";
		break;
		
		default:
		strSkinRes = "480p";
		break;		
	}
	
	m_strMediaDir = "D:\\skins\\" + strSkinName + "\\" + strSkinRes + "\\";

	UnloadSkin();

	m_GUITextureManager.Initialize();
	m_GUIFontManager.Initialize();
	m_GUIWindowManager.Initialize();
}

void CXboxGUI::UnloadSkin()
{
	m_GUIWindowManager.DeInitialize();
	m_GUIFontManager.Cleanup();
	m_GUITextureManager.ReleaseAllTextures();
}

void CXboxGUI::ReloadSkin()
{
	LoadSkin(g_XboxConfigs.GetString("gui.skin"));
	m_GUIWindowManager.ActivateWindow(WINDOW_GLOBAL_CONFIGS);
}

void CXboxGUI::DelaySkinLoad()
{
	m_dwSkinChangeTime = GetTickCount() + 2000;
}

//==================================================================================

int InitializeXboxGUI()
{
	return g_XboxGUI.Initialize();
}

void RunXboxGUI()
{
	g_XboxGUI.Run();
}

void XboxGUIActivateWindow(int iWindowID)
{
	g_XboxGUI.ActivateWindow(iWindowID);
}

int CloseXboxGUI()
{
	return g_XboxGUI.Close();
}

void SetGUID3DDevice(LPDIRECT3DDEVICE8 pD3DDevice, D3DPRESENT_PARAMETERS PresentParams)
{
	g_XboxGUI.SetD3DDevice(pD3DDevice, PresentParams);
}

int IsHDEnabled()
{
	return g_XboxGUI.bIsHDEnabled();
}

int IsXboxGUIActive()
{
	return g_XboxGUI.IsGUIActive();
}

void SetXboxGUIStartWindow(int iStartWindow)
{
	g_XboxGUI.SetStartWindow(iStartWindow);
}

void DrawXboxIngameOSD(char* strFps)
{
	g_XboxGUI.m_InGameOSD.Render(strFps);
}