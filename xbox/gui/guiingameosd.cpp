#include "guiingameosd.h"
#include "guiconfigs.h"
#include "utils\stringutils.h"

#define DEBUG_FONT_PATH "D:\\assets\\fixedfont.ttf"

CGUIInGameOSD::CGUIInGameOSD()
{
	m_iPosX = 85;
	m_iPosY = 40;
	m_iFontSize = 40;
	m_pDebugFont = NULL;
	m_bShowFPS = false;
	m_bShowFreeMemory = false;
	m_bAllocated = false;
}

CGUIInGameOSD::~CGUIInGameOSD()
{
}

void CGUIInGameOSD::Initialize()
{
	if(m_bAllocated)
		return;

	m_pDebugFont = new CGUIFont("DebugFont", DEBUG_FONT_PATH, XFONT_BOLD);

	if(m_pDebugFont)
	{
		if(!m_pDebugFont->Load(true))
			return;
	}

	// Adjust the size and position for multisample
	switch(g_XboxConfigs.GetInt("video.antialiasing"))
	{
		case MULTISAMPLE_NONE:
			m_iPosX = 40;
			m_iPosY = 40;
			m_iFontSize = 27;
			break;

		case MULTISAMPLE_2:
			m_iPosX = 85;
			m_iPosY = 40;
			m_iFontSize = 40;
			break;

		case MULTISAMPLE_4:
			m_iPosX = 85;
			m_iPosY = 80;
			m_iFontSize = 62;
			break;
	};

	if(g_XboxConfigs.GetInt("video.resolution") == RESOLUTION_480P)
	{
		m_iPosX /= 2;
		m_iPosY /= 2;
		m_iFontSize /= 2;
	}

	m_bShowFPS = g_XboxConfigs.GetBool("video.showfps");
	m_bShowFreeMemory = g_XboxConfigs.GetBool("video.showfreememory");

	m_bAllocated = true;
}

void CGUIInGameOSD::Render(char* strFps)
{
	// Only Initialized if we have show fps/mem on
	if(!m_bAllocated)
		return;

	// FPS
	if(m_bShowFPS)
		m_pDebugFont->Render(m_iPosX, m_iPosY, m_iFontSize, 0xffffff, strFps);

	// Check Memory
	if(m_bShowFreeMemory)
	{
		GlobalMemoryStatus(&m_stat);
		std::string strRamAvail = "Free Mem = ";

		strRamAvail += CStringUtils::IntToString(m_stat.dwAvailPhys/(1024*1024)) + "MB";

		m_pDebugFont->Render(m_iPosX, m_iPosY*2, m_iFontSize, 0xffffff, strRamAvail.c_str());
	}
}

void CGUIInGameOSD::DeInitialize()
{
	if(m_pDebugFont)
	{
		m_pDebugFont->FreeResources();
		delete m_pDebugFont;
	}

	m_pDebugFont = NULL;
	m_bAllocated = false;
}