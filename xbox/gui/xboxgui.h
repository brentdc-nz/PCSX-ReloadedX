#ifndef H_CXBOXGUI
#define H_CXBOXGUI

#include <xtl.h>
#include "..\fakeglx\fakeglx09.h"
#include "..\fakeglx\fakeglx01.h"

#ifdef __cplusplus

#include "guifontmanager.h"
#include "guiwindowmanager.h"
#include "guitexturemanager.h"
#include "guibgmusic.h"
#include "guiingameosd.h"

class CXBoxGUI
{
public:
	CXBoxGUI();
	~CXBoxGUI();

	bool Initialize();
	void Run();
	void Render();
	bool Close();

	bool IsGUIActive() { return m_bGUIActive; };

	void SetD3DDevice(LPDIRECT3DDEVICE8 pD3DDevice, D3DPRESENT_PARAMETERS PresentParams) { m_pD3DDevice = pD3DDevice; m_PresentParams = PresentParams; }
	LPDIRECT3DDEVICE8 GetD3DDevice() { return m_pD3DDevice; }
	bool bIsHDEnabled() { return XGetAVPack() == XC_AV_PACK_HDTV; }
	D3DPRESENT_PARAMETERS GetPresentParameters() const { return m_PresentParams; }

	D3DSurface* GetPreviewSurface();

	void SetStartWindow(int iStartWindow);
	void ActivateWindow(int iWindowID) { m_GUIWindowManager.ActivateWindow(iWindowID); };
	bool SendMessage(CGUIMessage message) { return m_GUIWindowManager.OnMessage(message); };

	/*const*/ CGUIFontManager& GetFontManager() /*const*/ { return m_GUIFontManager; };
	/*const*/ CGUITextureManager& GetTextureManager() /*const*/ { return m_GUITextureManager; };
	/*const*/ CGUIWindowManager& GetWindowManager() /*const*/ { return m_GUIWindowManager; };

	// Skin functions
	void LoadSkin(std::string strSkinName);
	void UnloadSkin();
	void ReloadSkin();
	void DelaySkinLoad();
	std::string GetMediaDir() { return m_strMediaDir; };

	CGUIInGameOSD m_InGameOSD;

private:
	void CheckGUIOptionChanges();

	CGUIFontManager		m_GUIFontManager;
	CGUIWindowManager	m_GUIWindowManager;
	CGUITextureManager  m_GUITextureManager;

	LPDIRECT3DDEVICE8 m_pD3DDevice;
	D3DPRESENT_PARAMETERS m_PresentParams;
	bool m_bGUIActive;

	CGUIBGMusic m_GUIBGMusic;

	DWORD m_dwGUIRenderStates;
	DWORD m_dwInGameRenderStates;
	bool m_bGUIStatesCaptured;
	bool m_bInGameStatesCaptured;

	LPDIRECT3DSURFACE8 m_surfPreview;
	int m_iStartWindow;

	std::string m_strMediaDir;
	DWORD m_dwSkinTime;

	int m_iCurrentResolution;
	DWORD m_dwResChangeTime;
};

extern CXBoxGUI g_XBoxGUI;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

int InitializeXBoxGUI();
void RunXBoxGUI();
int CloseXBoxGUI();
void SetGUID3DDevice(LPDIRECT3DDEVICE8 pD3DDevice, D3DPRESENT_PARAMETERS PresentParams);
int IsXBoxGUIActive();
int IsHDEnabled();
void SetXBoxGUIStartWindow(int iStartWindow);
void DrawXboxIngameOSD(char* strFps);

#ifdef __cplusplus
}
#endif

#endif //H_CXBOXGUI