/*  Pcsx - Pc Psx Emulator
 *  Copyright (C) 1999-2003  Pcsx Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "..\xbox\xbox.h"
#include "..\libpcsxcore\psxcommon.h"
#include "..\libpcsxcore\plugins.h"

#include "input\sysxboxinput.h"
#include "gui\xboxgui.h"
#include "gui\guiwindowkeys.h"
#include "gui\guiconfigs.h"

// For shutting down the Xbox
VOID WINAPI HalWriteSMBusValue(BYTE, BYTE, BOOL, BYTE);

int iRunning = 0;
int iPSXGameLoaded = 0;
int iStartWindow = WINDOW_INVALID;
char PcsxrDir[MAXPATHLEN];

int SysInit()
{
	if (EmuInit() == -1)
		return -1;

#ifdef EMU_LOG
	emuLog = fopen("D:\\emuLog.txt","w");
	setvbuf(emuLog, NULL,  _IONBF, 0);
#endif

	LoadMcds(Config.Mcd1, Config.Mcd2);

	if (Config.Debug) StartDebugger();

	return 0;
}

void SysReset()
{
	EmuReset();
}

void SysClose(BOOL bShutdown)
{
	LD_LAUNCH_DASHBOARD LaunchData =
	{ XLD_LAUNCH_DASHBOARD_MAIN_MENU };

	if (iRunning)
		ClosePlugins(TRUE);

	EmuShutdown();
	ReleasePlugins();

	if(IsXboxGUIActive())
		CloseXboxGUI();

	XboxConfigs_Clear();
	
	if(bShutdown)
		HalWriteSMBusValue(/*SMBDEV_PIC16L*/0x20, /*PIC16L_CMD_POWER*/0x02, 0, /*POWER_SUBCMD_POWER_OFF*/0x80);
	else
		XLaunchNewImage(NULL, (LAUNCH_DATA*)&LaunchData);
}

void SysPrintf(const char *fmt, ...)
{
#ifdef _DEBUG
	va_list list;
	char tmp[512];

	va_start(list,fmt);
	vsprintf(tmp,fmt,list);
	va_end(list);

	OutputDebugString(tmp);
#endif
}

void SysMessage(const char *fmt, ...)
{
#ifdef _DEBUG
	va_list list;
	char tmp[512];

	va_start(list,fmt);
	vsprintf(tmp,fmt,list);
	va_end(list);

	OutputDebugString(tmp);
#endif
}

void SysUpdate()
{
	if(CheckQuit())
		SysClose(FALSE);
}

int RunCommand(int iCommand, const char* strIsoFile)
{
	switch(iCommand)
	{
		case ID_FILE_RUNBIOS: // FIXME
			CloseXboxGUI();

			if (strcmp(Config.Bios, "HLE") == 0)
			{
				SysMessage(_("Running BIOS is not supported with Internal HLE Bios."));
				return TRUE;
			}
			
			SetIsoFile(NULL);
			LoadPlugins();

			if (OpenPlugins(/*hWnd*/) == -1)
			{
				ClosePlugins(FALSE);
				return TRUE;
			}

			SysReset();
			CdromId[0] = '\0';
			CdromLabel[0] = '\0';
			iRunning = 1;
			return TRUE;	

		case ID_EMULATOR_RUN:
			if(iPSXGameLoaded)
			{
				CloseXboxGUI();
				OpenPlugins(/*hWnd*/);
				iRunning = 1;
//				CheatSearchBackupMemory(); // TODO
				psxCpu->Execute();
				return TRUE;
			}
			return FALSE;

		case ID_FILE_RUN_CD:
			CloseXboxGUI();
			iPSXGameLoaded = TRUE;
			SetIsoFile(NULL);
			LoadPlugins();

			if (OpenPlugins(/*hWnd*/) == -1)
			{
				ClosePlugins(FALSE);
				iStartWindow = DIALOG_CDBOOTFAIL;
				SysRunGUI();
				return TRUE;
			}

			if (CheckCdrom() == -1)
			{
				ClosePlugins(FALSE);
				iStartWindow = DIALOG_CDBOOTFAIL;
				SysRunGUI();
				SysMessage(_("The CD does not appear to be a valid Playstation CD"));
				return TRUE;
			}
					
			// Auto-detect: region first, then rcnt reset
			SysReset();

			if (LoadCdrom() == -1)
			{
				ClosePlugins(FALSE);
				iStartWindow = DIALOG_CDBOOTFAIL;
				SysRunGUI();
				SysMessage(_("Could not load CD-ROM!"));
				return TRUE;
			}
			
			iRunning = 1;
			psxCpu->Execute();
			return TRUE;
	
		case ID_FILE_RUN_ISO:
			CloseXboxGUI();
			iPSXGameLoaded = TRUE;
			SetIsoFile(strIsoFile);

			LoadPlugins();
			if (OpenPlugins() == -1)
			{
				ClosePlugins(FALSE);
				iStartWindow = DIALOG_IMAGELOADFAIL;
				SysRunGUI();
				return TRUE;
			}
			if (CheckCdrom() == -1)
			{
				ClosePlugins(FALSE);
				SysMessage(_("The CD does not appear to be a valid Playstation CD"));
				iStartWindow = DIALOG_IMAGELOADFAIL;
				SysRunGUI();				
				return TRUE;
			}

			// Auto-detect: region first, then rcnt reset
			SysReset();

			if(!XboxConfigs_GetBool("core.showbiosscreen"))
			{
				if (LoadCdrom() == -1)
				{
					ClosePlugins(FALSE);
					SysMessage(_("Could not load CD-ROM!"));
					iStartWindow = DIALOG_IMAGELOADFAIL;
					SysRunGUI();
					return TRUE;
				}
			}

			iRunning = 1;
			psxCpu->Execute();
			return TRUE;

		default:
			return FALSE;
	}

	return FALSE;
}

void SysRunGUI()
{
	if(iRunning)
	{
		ClosePlugins(FALSE);
		iRunning = 0;
	}

	InitializeXboxGUI();

	while(1)
	{
		RunXboxGUI();
		SysUpdate();

		if(iStartWindow != WINDOW_INVALID)
		{
			XboxGUIActivateWindow(iStartWindow);
			iStartWindow = WINDOW_INVALID;
		}
	}
}

int main()
{
	MEMORYSTATUS lpBuffer;
	int iScrWidth, iScrHeight;
	iRunning = 0;
	iPSXGameLoaded = 0;

#ifndef _XBOX
	GetCurrentDirectory(256, PcsxrDir);
#else
	strcpy( PcsxrDir, "D:\\") ;
#endif
	
#ifndef _XBOX
	strcpy(cfgfile, "config.cfg");
#endif
	memset(&Config, 0, sizeof(PcsxConfig));

	strcpy(Config.Net, "Disabled");

	/*assert(*/XboxConfigs_Load()/*)*/;

	if(LoadCoreConfig(&Config) == -1)
	{
		// Uh oh, no config file found, use some defaults
		Config.PsxAuto = 1; 

#ifndef _HARDLINKED
		strcpy(Config.PluginsDir, "plugins\\");
#endif
		strcpy(Config.BiosDir,    "bios\\");
		strcpy(Config.Bios,    "scph1001.bin");

#ifdef ENABLE_SIO1API
		set_default_plugin(Sio1ConfS.plist[0], Config.Sio1);
#endif
		strcpy(Config.Mcd1, "memcards\\Mcd001.mcr");
		strcpy(Config.Mcd2, "memcards\\Mcd002.mcr");

#ifndef _XBOX // We use our config manager on Xbox
		SaveCoreConfig();
#endif
	}

#ifdef _XBOX // 128mb modded Xboxes needs this to fucntion correctly
	lpBuffer.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&lpBuffer);

	if(lpBuffer.dwTotalPhys > 67108864) // Check if > 64mb
	{
		__asm
		{
			mov ecx, 0x2ff
			rdmsr
			mov al, 0x06
			wrmsr
		}
	}
#endif
	if (SysInit() == -1) return 1;

#ifdef _XBOX 
	if(!IsHDEnabled())
	{
		// No component cables so force to 480i
		SysMessage("Component cables not detected forcing to 480p.\n");
		XboxConfigs_SetInt("video.resolution", RESOLUTION_480P); // Becomes 480i with no component cables
	}

	XboxConfigs_GetScreenSize(&iScrWidth, &iScrHeight);
	D3D_SetMode(iScrWidth, iScrHeight, IsHDEnabled());

	// We need to do this here for the GUI
	wglCreateContext(NULL);
	wglMakeCurrent(NULL, NULL);
#endif

	SysRunGUI();
}