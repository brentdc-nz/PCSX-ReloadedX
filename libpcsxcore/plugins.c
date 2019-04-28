/***************************************************************************
 *   Copyright (C) 2007 Ryan Schultz, PCSX-df Team, PCSX team              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

/*
* Plugin library callback/access functions.
*/

#include "plugins.h"
#include "cdriso.h"

static char IsoFile[MAXPATHLEN] = "";
static s64 cdOpenCaseTime = 0;

#ifdef _HARDLINKED
BOOL bPluginsOpen = FALSE;
#endif

GPUupdateLace         GPU_updateLace;
GPUinit               GPU_init;
GPUshutdown           GPU_shutdown;
GPUconfigure          GPU_configure;
GPUtest               GPU_test;
GPUabout              GPU_about;
GPUopen               GPU_open;
GPUclose              GPU_close;
GPUreadStatus         GPU_readStatus;
GPUreadData           GPU_readData;
GPUreadDataMem        GPU_readDataMem;
GPUwriteStatus        GPU_writeStatus;
GPUwriteData          GPU_writeData;
GPUwriteDataMem       GPU_writeDataMem;
GPUdmaChain           GPU_dmaChain;
GPUkeypressed         GPU_keypressed;
GPUdisplayText        GPU_displayText;
GPUmakeSnapshot       GPU_makeSnapshot;
GPUfreeze             GPU_freeze;
GPUgetScreenPic       GPU_getScreenPic;
GPUshowScreenPic      GPU_showScreenPic;
GPUclearDynarec       GPU_clearDynarec;
GPUhSync              GPU_hSync;
GPUvBlank             GPU_vBlank;
GPUvisualVibration    GPU_visualVibration;
GPUcursor             GPU_cursor;
GPUaddVertex          GPU_addVertex;

CDRinit               CDR_init;
CDRshutdown           CDR_shutdown;
CDRopen               CDR_open;
CDRclose              CDR_close;
CDRtest               CDR_test;
CDRgetTN              CDR_getTN;
CDRgetTD              CDR_getTD;
CDRreadTrack          CDR_readTrack;
CDRgetBuffer          CDR_getBuffer;
CDRplay               CDR_play;
CDRstop               CDR_stop;
CDRgetStatus          CDR_getStatus;
CDRgetDriveLetter     CDR_getDriveLetter;
CDRgetBufferSub       CDR_getBufferSub;
CDRconfigure          CDR_configure;
CDRabout              CDR_about;
CDRsetfilename        CDR_setfilename;
CDRreadCDDA           CDR_readCDDA;
CDRgetTE              CDR_getTE;

SPUconfigure          SPU_configure;
SPUabout              SPU_about;
SPUinit               SPU_init;
SPUshutdown           SPU_shutdown;
SPUtest               SPU_test;
SPUopen               SPU_open;
SPUclose              SPU_close;
SPUplaySample         SPU_playSample;
SPUwriteRegister      SPU_writeRegister;
SPUreadRegister       SPU_readRegister;
SPUwriteDMA           SPU_writeDMA;
SPUreadDMA            SPU_readDMA;
SPUwriteDMAMem        SPU_writeDMAMem;
SPUreadDMAMem         SPU_readDMAMem;
SPUplayADPCMchannel   SPU_playADPCMchannel;
SPUfreeze             SPU_freeze;
SPUregisterCallback   SPU_registerCallback;
SPUasync              SPU_async;
SPUplayCDDAchannel    SPU_playCDDAchannel;

PADconfigure          PAD1_configure;
PADabout              PAD1_about;
PADinit               PAD1_init;
PADshutdown           PAD1_shutdown;
PADtest               PAD1_test;
PADopen               PAD1_open;
PADclose              PAD1_close;
PADquery              PAD1_query;
PADreadPort1          PAD1_readPort1;
PADkeypressed         PAD1_keypressed;
PADstartPoll          PAD1_startPoll;
PADpoll               PAD1_poll;
PADsetSensitive       PAD1_setSensitive;
PADregisterVibration  PAD1_registerVibration;
PADregisterCursor     PAD1_registerCursor;

PADconfigure          PAD2_configure;
PADabout              PAD2_about;
PADinit               PAD2_init;
PADshutdown           PAD2_shutdown;
PADtest               PAD2_test;
PADopen               PAD2_open;
PADclose              PAD2_close;
PADquery              PAD2_query;
PADreadPort2          PAD2_readPort2;
PADkeypressed         PAD2_keypressed;
PADstartPoll          PAD2_startPoll;
PADpoll               PAD2_poll;
PADsetSensitive       PAD2_setSensitive;
PADregisterVibration  PAD2_registerVibration;
PADregisterCursor     PAD2_registerCursor;

NETinit               NET_init;
NETshutdown           NET_shutdown;
NETopen               NET_open;
NETclose              NET_close;
NETtest               NET_test;
NETconfigure          NET_configure;
NETabout              NET_about;
NETpause              NET_pause;
NETresume             NET_resume;
NETqueryPlayer        NET_queryPlayer;
NETsendData           NET_sendData;
NETrecvData           NET_recvData;
NETsendPadData        NET_sendPadData;
NETrecvPadData        NET_recvPadData;
NETsetInfo            NET_setInfo;
NETkeypressed         NET_keypressed;

#ifdef ENABLE_SIO1API

SIO1init              SIO1_init;
SIO1shutdown          SIO1_shutdown;
SIO1open              SIO1_open;
SIO1close             SIO1_close;
SIO1test              SIO1_test;
SIO1configure         SIO1_configure;
SIO1about             SIO1_about;
SIO1pause             SIO1_pause;
SIO1resume            SIO1_resume;
SIO1keypressed        SIO1_keypressed;
SIO1writeData8        SIO1_writeData8;
SIO1writeData16       SIO1_writeData16;
SIO1writeData32       SIO1_writeData32;
SIO1writeStat16       SIO1_writeStat16;
SIO1writeStat32       SIO1_writeStat32;
SIO1writeMode16       SIO1_writeMode16;
SIO1writeMode32       SIO1_writeMode32;
SIO1writeCtrl16       SIO1_writeCtrl16;
SIO1writeCtrl32       SIO1_writeCtrl32;
SIO1writeBaud16       SIO1_writeBaud16;
SIO1writeBaud32       SIO1_writeBaud32;
SIO1readData8         SIO1_readData8;
SIO1readData16        SIO1_readData16;
SIO1readData32        SIO1_readData32;
SIO1readStat16        SIO1_readStat16;
SIO1readStat32        SIO1_readStat32;
SIO1readMode16        SIO1_readMode16;
SIO1readMode32        SIO1_readMode32;
SIO1readCtrl16        SIO1_readCtrl16;
SIO1readCtrl32        SIO1_readCtrl32;
SIO1readBaud16        SIO1_readBaud16;
SIO1readBaud32        SIO1_readBaud32;
SIO1update            SIO1_update;
SIO1registerCallback  SIO1_registerCallback;

#endif

#ifndef _HARDLINKED
static const char *err;

#define CheckErr(func) { \
	err = SysLibError(); \
	if (err != NULL) { SysMessage(_("Error loading %s: %s"), func, err); return -1; } \
}

#define LoadSym(dest, src, name, checkerr) { \
	dest = (src)SysLoadSym(drv, name); \
	if (checkerr) { CheckErr(name); } else SysLibError(); \
}

void *hGPUDriver = NULL;
#endif //_HARDLINKED

void CALLBACK GPU__displayText(char *pText) {
	SysPrintf("%s\n", pText);
}

long CALLBACK GPU__configure(void) { return 0; }
long CALLBACK GPU__test(void) { return 0; }
void CALLBACK GPU__about(void) {}
void CALLBACK GPU__makeSnapshot(void) {}
void CALLBACK GPU__keypressed(int key) {}
long CALLBACK GPU__getScreenPic(unsigned char *pMem) { return -1; }
long CALLBACK GPU__showScreenPic(unsigned char *pMem) { return -1; }
void CALLBACK GPU__clearDynarec(void (CALLBACK *callback)(void)) {}
void CALLBACK GPU__hSync(int val) {}
void CALLBACK GPU__vBlank(int val) {}
void CALLBACK GPU__visualVibration(unsigned long iSmall, unsigned long iBig) {}
void CALLBACK GPU__cursor(int player, int x, int y) {}
void CALLBACK GPU__addVertex(short sx,short sy,s64 fx,s64 fy,s64 fz) {}

#ifndef _HARDLINKED
#define LoadGpuSym1(dest, name) \
	LoadSym(GPU_##dest, GPU##dest, name, TRUE);

#define LoadGpuSym0(dest, name) \
	LoadSym(GPU_##dest, GPU##dest, name, FALSE); \
	if (GPU_##dest == NULL) GPU_##dest = (GPU##dest) GPU__##dest;

#define LoadGpuSymN(dest, name) \
	LoadSym(GPU_##dest, GPU##dest, name, FALSE);
#endif //_HARDLINKED

static int LoadGPUplugin(const char *GPUdll) {
#ifndef _HARDLINKED
	void *drv;

	hGPUDriver = SysLoadLibrary(GPUdll);
	if (hGPUDriver == NULL) {
		GPU_configure = NULL;
		SysMessage (_("Could not load GPU plugin %s!\n%s"), GPUdll, SysLibError());
		return -1;
	}
	drv = hGPUDriver;
	LoadGpuSym1(init, "GPUinit");
	LoadGpuSym1(shutdown, "GPUshutdown");
	LoadGpuSym1(open, "GPUopen");
	LoadGpuSym1(close, "GPUclose");
	LoadGpuSym1(readData, "GPUreadData");
	LoadGpuSym1(readDataMem, "GPUreadDataMem");
	LoadGpuSym1(readStatus, "GPUreadStatus");
	LoadGpuSym1(writeData, "GPUwriteData");
	LoadGpuSym1(writeDataMem, "GPUwriteDataMem");
	LoadGpuSym1(writeStatus, "GPUwriteStatus");
	LoadGpuSym1(dmaChain, "GPUdmaChain");
	LoadGpuSym1(updateLace, "GPUupdateLace");
	LoadGpuSym0(keypressed, "GPUkeypressed");
	LoadGpuSym0(displayText, "GPUdisplayText");
	LoadGpuSym0(makeSnapshot, "GPUmakeSnapshot");
	LoadGpuSym1(freeze, "GPUfreeze");
	LoadGpuSym0(getScreenPic, "GPUgetScreenPic");
	LoadGpuSym0(showScreenPic, "GPUshowScreenPic");
	LoadGpuSym0(clearDynarec, "GPUclearDynarec");
    LoadGpuSym0(hSync, "GPUhSync");
    LoadGpuSym0(vBlank, "GPUvBlank");
    LoadGpuSym0(visualVibration, "GPUvisualVibration");
    LoadGpuSym0(cursor, "GPUcursor");
	LoadGpuSym0(addVertex, "GPUaddVertex");
	LoadGpuSym0(configure, "GPUconfigure");
	LoadGpuSym0(test, "GPUtest");
	LoadGpuSym0(about, "GPUabout");
#else
	GPU_init = &Plugin_GPUinit;
 	GPU_shutdown = &Plugin_GPUshutdown;
	GPU_open = &Plugin_GPUopen;
	GPU_close = &Plugin_GPUclose;
	GPU_readData = &Plugin_GPUreadData;
	GPU_readDataMem = &Plugin_GPUreadDataMem;
	GPU_readStatus = &Plugin_GPUreadStatus;
	GPU_writeData = &Plugin_GPUwriteData;
	GPU_writeDataMem = &Plugin_GPUwriteDataMem;
	GPU_writeStatus = &Plugin_GPUwriteStatus;
	GPU_dmaChain = &Plugin_GPUdmaChain;
	GPU_updateLace = &Plugin_GPUupdateLace;
	GPU_keypressed = &Plugin_GPUkeypressed;
	GPU_freeze = &Plugin_GPUfreeze;
	GPU_hSync = &Plugin_GPUhSync;
	GPU_vBlank = &Plugin_GPUvBlank;
	GPU_visualVibration = &Plugin_GPUvisualVibration;
	GPU_cursor = &Plugin_GPUcursor;
	GPU_addVertex = &Plugin_GPUaddVertex;
	GPU_configure = &Plugin_GPUconfigure;

	// Dummy unrequired functions
	GPU_displayText = &GPU__displayText;
	GPU_makeSnapshot = &GPU__makeSnapshot;
	GPU_getScreenPic = &GPU__getScreenPic;
	GPU_showScreenPic = &GPU__showScreenPic;
	GPU_clearDynarec = &GPU__clearDynarec;
	GPU_test = &GPU__test;
	GPU_about = &GPU__about;
#endif //_HARDLINKED

	return 0;
}

#ifndef _HARDLINKED
void *hCDRDriver = NULL;
#endif

long CALLBACK CDR__play(unsigned char *sector) { return 0; }
long CALLBACK CDR__stop(void) { return 0; }

long CALLBACK CDR__getStatus(struct CdrStat *stat) {
	if (cdOpenCaseTime < 0 || cdOpenCaseTime > (s64)time(NULL))
		stat->Status = 0x10;
	else
		stat->Status = 0;

	return 0;
}

char* CALLBACK CDR__getDriveLetter(void) { return NULL; }
long CALLBACK CDR__configure(void) { return 0; }
long CALLBACK CDR__test(void) { return 0; }
void CALLBACK CDR__about(void) {}
long CALLBACK CDR__setfilename(char*filename) { return 0; }

#ifndef _HARDLINKED
#define LoadCdrSym1(dest, name) \
	LoadSym(CDR_##dest, CDR##dest, name, TRUE);

#define LoadCdrSym0(dest, name) \
	LoadSym(CDR_##dest, CDR##dest, name, FALSE); \
	if (CDR_##dest == NULL) CDR_##dest = (CDR##dest) CDR__##dest;

#define LoadCdrSymN(dest, name) \
	LoadSym(CDR_##dest, CDR##dest, name, FALSE);
#endif //_HARDLINKED

static int LoadCDRplugin(const char *CDRdll) {
	if (CDRdll == NULL) {
		cdrIsoInit();
		return 0;
	}
#ifndef _HARDLINKED
	void *drv;

	hCDRDriver = SysLoadLibrary(CDRdll);
	if (hCDRDriver == NULL) {
		CDR_configure = NULL;
		SysMessage (_("Could not load CD-ROM plugin %s!\n%s"), CDRdll, SysLibError());
		return -1;
	}
	drv = hCDRDriver;
	LoadCdrSym1(init, "CDRinit");
	LoadCdrSym1(shutdown, "CDRshutdown");
	LoadCdrSym1(open, "CDRopen");
	LoadCdrSym1(close, "CDRclose");
	LoadCdrSym1(getTN, "CDRgetTN");
	LoadCdrSym1(getTD, "CDRgetTD");
	LoadCdrSym1(readTrack, "CDRreadTrack");
	LoadCdrSym1(getBuffer, "CDRgetBuffer");
	LoadCdrSym1(getBufferSub, "CDRgetBufferSub");
	LoadCdrSym0(play, "CDRplay");
	LoadCdrSym0(stop, "CDRstop");
	LoadCdrSym0(getStatus, "CDRgetStatus");
	LoadCdrSym0(getDriveLetter, "CDRgetDriveLetter");
	LoadCdrSym0(configure, "CDRconfigure");
	LoadCdrSym0(test, "CDRtest");
	LoadCdrSym0(about, "CDRabout");
	LoadCdrSym0(setfilename, "CDRsetfilename");
	LoadCdrSymN(readCDDA, "CDRreadCDDA");
	LoadCdrSymN(getTE, "CDRgetTE");
#else
	CDR_init = &Plugin_CDRinit;
	CDR_shutdown = &Plugin_CDRshutdown;
	CDR_open = &Plugin_CDRopen;
	CDR_close = &Plugin_CDRclose;
	CDR_getTN = &Plugin_CDRgetTN;
	CDR_getTD = &Plugin_CDRgetTD;
	CDR_readTrack = &Plugin_CDRreadTrack;
	CDR_getBuffer = &Plugin_CDRgetBuffer;
	CDR_getBufferSub = &Plugin_CDRgetBufferSub;
	CDR_play = &Plugin_CDRplay;
	CDR_stop = &Plugin_CDRstop;
	CDR_getStatus = &Plugin_CDRgetStatus;

	// Dummy unrequired functions
	CDR_getDriveLetter = CDR__getDriveLetter;
	CDR_configure = CDR__configure;
	CDR_test = CDR__test;
	CDR_about = CDR__about;
	CDR_setfilename = CDR__setfilename;
#endif //_HARDLINKED

	return 0;
}

long CALLBACK SPU__configure(void) { return 0; }
void CALLBACK SPU__about(void) {}
long CALLBACK SPU__test(void) { return 0; }

#ifndef _HARDLINKED

void *hSPUDriver = NULL;

#define LoadSpuSym1(dest, name) \
	LoadSym(SPU_##dest, SPU##dest, name, TRUE);

#define LoadSpuSym0(dest, name) \
	LoadSym(SPU_##dest, SPU##dest, name, FALSE); \
	if (SPU_##dest == NULL) SPU_##dest = (SPU##dest) SPU__##dest;

#define LoadSpuSymN(dest, name) \
	LoadSym(SPU_##dest, SPU##dest, name, FALSE);
#endif //_HARDLINKED

static int LoadSPUplugin(const char *SPUdll) {
#ifndef _HARDLINKED
	void *drv;

	hSPUDriver = SysLoadLibrary(SPUdll);
	if (hSPUDriver == NULL) {
		SPU_configure = NULL;
		SysMessage (_("Could not load SPU plugin %s!\n%s"), SPUdll, SysLibError());
		return -1;
	}
	drv = hSPUDriver;
	LoadSpuSym1(init, "SPUinit");
	LoadSpuSym1(shutdown, "SPUshutdown");
	LoadSpuSym1(open, "SPUopen");
	LoadSpuSym1(close, "SPUclose");
	LoadSpuSym0(configure, "SPUconfigure");
	LoadSpuSym0(about, "SPUabout");
	LoadSpuSym0(test, "SPUtest");
	LoadSpuSym1(writeRegister, "SPUwriteRegister");
	LoadSpuSym1(readRegister, "SPUreadRegister");
	LoadSpuSym1(writeDMA, "SPUwriteDMA");
	LoadSpuSym1(readDMA, "SPUreadDMA");
	LoadSpuSym1(writeDMAMem, "SPUwriteDMAMem");
	LoadSpuSym1(readDMAMem, "SPUreadDMAMem");
	LoadSpuSym1(playADPCMchannel, "SPUplayADPCMchannel");
	LoadSpuSym1(freeze, "SPUfreeze");
	LoadSpuSym1(registerCallback, "SPUregisterCallback");
	LoadSpuSymN(async, "SPUasync");
	LoadSpuSymN(playCDDAchannel, "SPUplayCDDAchannel");
#else
	SPU_init = &Plugin_SPUinit;
	SPU_shutdown = &Plugin_SPUshutdown;
	SPU_open = &Plugin_SPUopen;
	SPU_close = &Plugin_SPUclose;
	SPU_writeRegister = &Plugin_SPUwriteRegister;
	SPU_readRegister = &Plugin_SPUreadRegister;
	SPU_readDMA = &Plugin_SPUreadDMA;
	SPU_writeDMA = &Plugin_SPUwriteDMA;
	SPU_readDMAMem = &Plugin_SPUreadDMAMem;
	SPU_writeDMAMem = &Plugin_SPUwriteDMAMem;
	SPU_playADPCMchannel = &Plugin_SPUplayADPCMchannel;
	SPU_freeze = NULL; //TODO: Needed for save states
	SPU_registerCallback = &Plugin_SPUregisterCallback;
	SPU_async = &Plugin_SPUasync;
	SPU_playCDDAchannel = &Plugin_SPUplayCDDAchannel;

	// Dummy unrequired functions
	SPU_configure = &SPU__configure;
	SPU_about = &SPU__about;
	SPU_about = &SPU__test;
#endif //_HARDLINKED

	return 0;
}

#ifndef _HARDLINKED
void *hPAD1Driver = NULL;
void *hPAD2Driver = NULL;
#endif

static unsigned char buf[256];
unsigned char stdpar[10] = { 0x00, 0x41, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
unsigned char mousepar[8] = { 0x00, 0x12, 0x5a, 0xff, 0xff, 0xff, 0xff };
unsigned char analogpar[9] = { 0x00, 0xff, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static int bufcount, bufc;

PadDataS padd1, padd2;

unsigned char _PADstartPoll(PadDataS *pad) {
    bufc = 0;

    switch (pad->controllerType) {
        case PSE_PAD_TYPE_MOUSE:
            mousepar[3] = pad->buttonStatus & 0xff;
            mousepar[4] = pad->buttonStatus >> 8;
            mousepar[5] = pad->moveX;
            mousepar[6] = pad->moveY;

            memcpy(buf, mousepar, 7);
            bufcount = 6;
            break;
        case PSE_PAD_TYPE_NEGCON: // npc101/npc104(slph00001/slph00069)
            analogpar[1] = 0x23;
            analogpar[3] = pad->buttonStatus & 0xff;
            analogpar[4] = pad->buttonStatus >> 8;
            analogpar[5] = pad->rightJoyX;
            analogpar[6] = pad->rightJoyY;
            analogpar[7] = pad->leftJoyX;
            analogpar[8] = pad->leftJoyY;

            memcpy(buf, analogpar, 9);
            bufcount = 8;
            break;
        case PSE_PAD_TYPE_ANALOGPAD: // scph1150
            analogpar[1] = 0x73;
            analogpar[3] = pad->buttonStatus & 0xff;
            analogpar[4] = pad->buttonStatus >> 8;
            analogpar[5] = pad->rightJoyX;
            analogpar[6] = pad->rightJoyY;
            analogpar[7] = pad->leftJoyX;
            analogpar[8] = pad->leftJoyY;

            memcpy(buf, analogpar, 9);
            bufcount = 8;
            break;
        case PSE_PAD_TYPE_ANALOGJOY: // scph1110
            analogpar[1] = 0x53;
            analogpar[3] = pad->buttonStatus & 0xff;
            analogpar[4] = pad->buttonStatus >> 8;
            analogpar[5] = pad->rightJoyX;
            analogpar[6] = pad->rightJoyY;
            analogpar[7] = pad->leftJoyX;
            analogpar[8] = pad->leftJoyY;

            memcpy(buf, analogpar, 9);
            bufcount = 8;
            break;
        case PSE_PAD_TYPE_STANDARD:
        default:
            stdpar[3] = pad->buttonStatus & 0xff;
            stdpar[4] = pad->buttonStatus >> 8;

            memcpy(buf, stdpar, 5);
            bufcount = 4;
    }

    return buf[bufc++];
}

unsigned char _PADpoll(unsigned char value) {
    if (bufc > bufcount) return 0;
    return buf[bufc++];
}

unsigned char CALLBACK PAD1__startPoll(int pad) {
    PadDataS padd;

    PAD1_readPort1(&padd);

    return _PADstartPoll(&padd);
}

unsigned char CALLBACK PAD1__poll(unsigned char value) {
    return _PADpoll(value);
}

long CALLBACK PAD1__configure(void) { return 0; }
void CALLBACK PAD1__about(void) {}
long CALLBACK PAD1__test(void) { return 0; }
long CALLBACK PAD1__query(void) { return 3; }
long CALLBACK PAD1__keypressed() { return 0; }
void CALLBACK PAD1__registerVibration(void (CALLBACK *callback)(unsigned long, unsigned long)) {}
void CALLBACK PAD1__registerCursor(void (CALLBACK *callback)(int, int, int)) {}

#ifndef _HARDLINKED
#define LoadPad1Sym1(dest, name) \
	LoadSym(PAD1_##dest, PAD##dest, name, TRUE);

#define LoadPad1SymN(dest, name) \
	LoadSym(PAD1_##dest, PAD##dest, name, FALSE);

#define LoadPad1Sym0(dest, name) \
	LoadSym(PAD1_##dest, PAD##dest, name, FALSE); \
	if (PAD1_##dest == NULL) PAD1_##dest = (PAD##dest) PAD1__##dest;
#endif

static int LoadPAD1plugin(const char *PAD1dll) {
#ifndef _HARDLINKED
	void *drv;

	hPAD1Driver = SysLoadLibrary(PAD1dll);
	if (hPAD1Driver == NULL) {
		PAD1_configure = NULL;
		SysMessage (_("Could not load Controller 1 plugin %s!\n%s"), PAD1dll, SysLibError());
		return -1;
	}
	drv = hPAD1Driver;
	LoadPad1Sym1(init, "PADinit");
	LoadPad1Sym1(shutdown, "PADshutdown");
	LoadPad1Sym1(open, "PADopen");
	LoadPad1Sym1(close, "PADclose");
	LoadPad1Sym0(query, "PADquery");
	LoadPad1Sym1(readPort1, "PADreadPort1");
	LoadPad1Sym0(configure, "PADconfigure");
	LoadPad1Sym0(test, "PADtest");
	LoadPad1Sym0(about, "PADabout");
	LoadPad1Sym0(keypressed, "PADkeypressed");
	LoadPad1Sym0(startPoll, "PADstartPoll");
	LoadPad1Sym0(poll, "PADpoll");
	LoadPad1SymN(setSensitive, "PADsetSensitive");
    LoadPad1Sym0(registerVibration, "PADregisterVibration");
    LoadPad1Sym0(registerCursor, "PADregisterCursor");
#else
	PAD1_test = &PAD1__test; 
	PAD1_configure = &PAD1__configure;
	PAD1_about = &PAD1__about;
	PAD1_query = &PAD1__query;
	PAD1_registerCursor = & PAD1__registerCursor;

	PAD1_init = &Plugin_PADinit;
	PAD1_shutdown = &Plugin_PADshutdown;
	PAD1_keypressed = &Plugin_PADkeypressed1;

	PAD1_readPort1 = &Plugin_PADreadPort1;
	PAD1_startPoll = &Plugin_PADstartPoll;
	PAD1_poll = &Plugin_PADpoll;
	PAD1_open = &Plugin_PADopen;
	PAD1_close = &Plugin_PADclose;
	PAD1_registerVibration = &Plugin_PADregisterVibration;
#endif //_HARDLINKED

	return 0;
}

unsigned char CALLBACK PAD2__startPoll(int pad) {
	PadDataS padd;

	PAD2_readPort2(&padd);

	return _PADstartPoll(&padd);
}

unsigned char CALLBACK PAD2__poll(unsigned char value) {
	return _PADpoll(value);
}

long CALLBACK PAD2__configure(void) { return 0; }
void CALLBACK PAD2__about(void) {}
long CALLBACK PAD2__test(void) { return 0; }
long CALLBACK PAD2__query(void) { return PSE_PAD_USE_PORT1 | PSE_PAD_USE_PORT2; }
long CALLBACK PAD2__keypressed() { return 0; }
void CALLBACK PAD2__registerVibration(void (CALLBACK *callback)(unsigned long, unsigned long)) {}
void CALLBACK PAD2__registerCursor(void (CALLBACK *callback)(int, int, int)) {}

#ifndef _HARDLINKED
#define LoadPad2Sym1(dest, name) \
	LoadSym(PAD2_##dest, PAD##dest, name, TRUE);

#define LoadPad2Sym0(dest, name) \
	LoadSym(PAD2_##dest, PAD##dest, name, FALSE); \
	if (PAD2_##dest == NULL) PAD2_##dest = (PAD##dest) PAD2__##dest;

#define LoadPad2SymN(dest, name) \
	LoadSym(PAD2_##dest, PAD##dest, name, FALSE);
#endif

static int LoadPAD2plugin(const char *PAD2dll) {
#ifndef _HARDLINKED
	void *drv;

	hPAD2Driver = SysLoadLibrary(PAD2dll);
	if (hPAD2Driver == NULL) {
		PAD2_configure = NULL;
		SysMessage (_("Could not load Controller 2 plugin %s!\n%s"), PAD2dll, SysLibError());
		return -1;
	}
	drv = hPAD2Driver;
	LoadPad2Sym1(init, "PADinit");
	LoadPad2Sym1(shutdown, "PADshutdown");
	LoadPad2Sym1(open, "PADopen");
	LoadPad2Sym1(close, "PADclose");
	LoadPad2Sym0(query, "PADquery");
	LoadPad2Sym1(readPort2, "PADreadPort2");
	LoadPad2Sym0(configure, "PADconfigure");
	LoadPad2Sym0(test, "PADtest");
	LoadPad2Sym0(about, "PADabout");
	LoadPad2Sym0(keypressed, "PADkeypressed");
	LoadPad2Sym0(startPoll, "PADstartPoll");
	LoadPad2Sym0(poll, "PADpoll");
	LoadPad2SymN(setSensitive, "PADsetSensitive");
    LoadPad2Sym0(registerVibration, "PADregisterVibration");
    LoadPad2Sym0(registerCursor, "PADregisterCursor");
#else
	PAD2_test = &PAD2__test; 
	PAD2_configure = &PAD2__configure;
	PAD2_about = &PAD2__about;
	PAD2_query = &PAD2__query;
	PAD2_registerCursor = & PAD2__registerCursor;

	PAD2_init = &Plugin_PADinit;
	PAD2_shutdown = &Plugin_PADshutdown;
	PAD2_keypressed = &Plugin_PADkeypressed2;

	PAD2_readPort2 = &Plugin_PADreadPort2;
	PAD2_startPoll = &Plugin_PADstartPoll;
	PAD2_poll = &Plugin_PADpoll;
	PAD2_open = &Plugin_PADopen;
	PAD2_close = &Plugin_PADclose;
	PAD2_registerVibration = &Plugin_PADregisterVibration;
#endif //_HARDLINKED

	return 0;
}

#ifndef _HARDLINKED
void *hNETDriver = NULL;

void CALLBACK NET__setInfo(netInfo *info) {}
void CALLBACK NET__keypressed(int key) {}
long CALLBACK NET__configure(void) { return 0; }
long CALLBACK NET__test(void) { return 0; }
void CALLBACK NET__about(void) {}

#define LoadNetSym1(dest, name) \
	LoadSym(NET_##dest, NET##dest, name, TRUE);

#define LoadNetSymN(dest, name) \
	LoadSym(NET_##dest, NET##dest, name, FALSE);

#define LoadNetSym0(dest, name) \
	LoadSym(NET_##dest, NET##dest, name, FALSE); \
	if (NET_##dest == NULL) NET_##dest = (NET##dest) NET__##dest;
#endif

static int LoadNETplugin(const char *NETdll) {
#ifndef _HARDLINKED
	void *drv;

	hNETDriver = SysLoadLibrary(NETdll);
	if (hNETDriver == NULL) {
		SysMessage (_("Could not load NetPlay plugin %s!\n%s"), NETdll, SysLibError());
		return -1;
	}
	drv = hNETDriver;
	LoadNetSym1(init, "NETinit");
	LoadNetSym1(shutdown, "NETshutdown");
	LoadNetSym1(open, "NETopen");
	LoadNetSym1(close, "NETclose");
	LoadNetSymN(sendData, "NETsendData");
	LoadNetSymN(recvData, "NETrecvData");
	LoadNetSym1(sendPadData, "NETsendPadData");
	LoadNetSym1(recvPadData, "NETrecvPadData");
	LoadNetSym1(queryPlayer, "NETqueryPlayer");
	LoadNetSym1(pause, "NETpause");
	LoadNetSym1(resume, "NETresume");
	LoadNetSym0(setInfo, "NETsetInfo");
	LoadNetSym0(keypressed, "NETkeypressed");
	LoadNetSym0(configure, "NETconfigure");
	LoadNetSym0(test, "NETtest");
	LoadNetSym0(about, "NETabout");
#else 
	
	//TODO - Does anyone use netplay??

#endif //_HARDLINKED

	return 0;
}

#ifdef ENABLE_SIO1API

void *hSIO1Driver = NULL;

long CALLBACK SIO1__init(void) { return 0; }
long CALLBACK SIO1__shutdown(void) { return 0; }
long CALLBACK SIO1__open(void) { return 0; }
long CALLBACK SIO1__close(void) { return 0; }
long CALLBACK SIO1__configure(void) { return 0; }
long CALLBACK SIO1__test(void) { return 0; }
void CALLBACK SIO1__about(void) {}
void CALLBACK SIO1__pause(void) {}
void CALLBACK SIO1__resume(void) {}
long CALLBACK SIO1__keypressed(int key) { return 0; }
void CALLBACK SIO1__writeData8(u8 val) {}
void CALLBACK SIO1__writeData16(u16 val) {}
void CALLBACK SIO1__writeData32(u32 val) {}
void CALLBACK SIO1__writeStat16(u16 val) {}
void CALLBACK SIO1__writeStat32(u32 val) {}
void CALLBACK SIO1__writeMode16(u16 val) {}
void CALLBACK SIO1__writeMode32(u32 val) {}
void CALLBACK SIO1__writeCtrl16(u16 val) {}
void CALLBACK SIO1__writeCtrl32(u32 val) {}
void CALLBACK SIO1__writeBaud16(u16 val) {}
void CALLBACK SIO1__writeBaud32(u32 val) {}
u8 CALLBACK SIO1__readData8(void) { return 0; }
u16 CALLBACK SIO1__readData16(void) { return 0; }
u32 CALLBACK SIO1__readData32(void) { return 0; }
u16 CALLBACK SIO1__readStat16(void) { return 0; }
u32 CALLBACK SIO1__readStat32(void) { return 0; }
u16 CALLBACK SIO1__readMode16(void) { return 0; }
u32 CALLBACK SIO1__readMode32(void) { return 0; }
u16 CALLBACK SIO1__readCtrl16(void) { return 0; }
u32 CALLBACK SIO1__readCtrl32(void) { return 0; }
u16 CALLBACK SIO1__readBaud16(void) { return 0; }
u32 CALLBACK SIO1__readBaud32(void) { return 0; }
void CALLBACK SIO1__update(uint32_t t) {};
void CALLBACK SIO1__registerCallback(void (CALLBACK *callback)(void)) {};

#define LoadSio1Sym1(dest, name) \
    LoadSym(SIO1_##dest, SIO1##dest, name, TRUE);

#define LoadSio1SymN(dest, name) \
    LoadSym(SIO1_##dest, SIO1##dest, name, FALSE);

#define LoadSio1Sym0(dest, name) \
    LoadSym(SIO1_##dest, SIO1##dest, name, FALSE); \
    if (SIO1_##dest == NULL) SIO1_##dest = (SIO1##dest) SIO1__##dest;

static int LoadSIO1plugin(const char *SIO1dll) {
    void *drv;

    hSIO1Driver = SysLoadLibrary(SIO1dll);
    if (hSIO1Driver == NULL) {
		SysMessage (_("Could not load SIO1 plugin %s!\n%s"), SIO1dll, SysLibError());
		return -1;
    }
    drv = hSIO1Driver;

    LoadSio1Sym0(init, "SIO1init");
    LoadSio1Sym0(shutdown, "SIO1shutdown");
    LoadSio1Sym0(open, "SIO1open");
    LoadSio1Sym0(close, "SIO1close");
    LoadSio1Sym0(pause, "SIO1pause");
    LoadSio1Sym0(resume, "SIO1resume");
    LoadSio1Sym0(keypressed, "SIO1keypressed");
    LoadSio1Sym0(configure, "SIO1configure");
    LoadSio1Sym0(test, "SIO1test");
    LoadSio1Sym0(about, "SIO1about");
    LoadSio1Sym0(writeData8, "SIO1writeData8");
    LoadSio1Sym0(writeData16, "SIO1writeData16");
    LoadSio1Sym0(writeData32, "SIO1writeData32");
    LoadSio1Sym0(writeStat16, "SIO1writeStat16");
    LoadSio1Sym0(writeStat32, "SIO1writeStat32");
    LoadSio1Sym0(writeMode16, "SIO1writeMode16");
    LoadSio1Sym0(writeMode32, "SIO1writeMode32");
    LoadSio1Sym0(writeCtrl16, "SIO1writeCtrl16");
    LoadSio1Sym0(writeCtrl32, "SIO1writeCtrl32");
    LoadSio1Sym0(writeBaud16, "SIO1writeBaud16");
    LoadSio1Sym0(writeBaud32, "SIO1writeBaud32");
	LoadSio1Sym0(readData8, "SIO1readData8");
    LoadSio1Sym0(readData16, "SIO1readData16");
    LoadSio1Sym0(readData32, "SIO1readData32");
    LoadSio1Sym0(readStat16, "SIO1readStat16");
    LoadSio1Sym0(readStat32, "SIO1readStat32");
    LoadSio1Sym0(readMode16, "SIO1readMode16");
    LoadSio1Sym0(readMode32, "SIO1readMode32");
    LoadSio1Sym0(readCtrl16, "SIO1readCtrl16");
    LoadSio1Sym0(readCtrl32, "SIO1readCtrl32");
    LoadSio1Sym0(readBaud16, "SIO1readBaud16");
    LoadSio1Sym0(readBaud32, "SIO1readBaud32");
	LoadSio1Sym0(update, "SIO1update");
    LoadSio1Sym0(registerCallback, "SIO1registerCallback");

    return 0;
}

#endif

void CALLBACK clearDynarec(void) {
	psxCpu->Reset();
}

int LoadPlugins() {
	long ret;
#ifndef _HARDLINKED
	char Plugin[MAXPATHLEN];
#endif

	ReleasePlugins();

	if (UsingIso()) {
		LoadCDRplugin(NULL);
	} else {
#ifndef _HARDLINKED
		sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Cdr);\
		if (LoadCDRplugin(Plugin) == -1) return -1;
#else
		if (LoadCDRplugin("StaticCDPlugin") == -1) return -1;
#endif
	}

#ifndef _HARDLINKED
	sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Gpu);
	if (LoadGPUplugin(Plugin) == -1) return -1;
#else
	if (LoadGPUplugin("") == -1) return -1;
#endif

#ifndef _HARDLINKED
	sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Spu);
	if (LoadSPUplugin(Plugin) == -1) return -1;
#else
	if (LoadSPUplugin("") == -1) return -1;
#endif

#ifndef _HARDLINKED
	sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Pad1);
	if (LoadPAD1plugin(Plugin) == -1) return -1;
#else
	if (LoadPAD1plugin("") == -1) return -1;
#endif

#ifndef _HARDLINKED
	sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Pad2);
	if (LoadPAD2plugin(Plugin) == -1) return -1;
#else
	if (LoadPAD2plugin("") == -1) return -1;
#endif

	if (strcmp("Disabled", Config.Net) == 0 || strcmp("", Config.Net) == 0)
		Config.UseNet = FALSE;
	else {
		Config.UseNet = TRUE;
#ifndef _HARDLINKED
		sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Net);
		if (LoadNETplugin(Plugin) == -1) Config.UseNet = FALSE;
#else
		if (LoadNETplugin("") == -1) Config.UseNet = FALSE;
#endif
	}

#ifdef ENABLE_SIO1API
	sprintf(Plugin, "%s/%s", Config.PluginsDir, Config.Sio1);
	if (LoadSIO1plugin(Plugin) == -1) return -1;
#endif

	ret = CDR_init();
	if (ret < 0) { SysMessage (_("Error initializing CD-ROM plugin: %d"), ret); return -1; }
	ret = GPU_init();
	if (ret < 0) { SysMessage (_("Error initializing GPU plugin: %d"), ret); return -1; }
	ret = SPU_init();
	if (ret < 0) { SysMessage (_("Error initializing SPU plugin: %d"), ret); return -1; }
	ret = PAD1_init(1);
	if (ret < 0) { SysMessage (_("Error initializing Controller 1 plugin: %d"), ret); return -1; }
	ret = PAD2_init(2);
	if (ret < 0) { SysMessage (_("Error initializing Controller 2 plugin: %d"), ret); return -1; }

	if (Config.UseNet) {
		ret = NET_init();
		if (ret < 0) { SysMessage (_("Error initializing NetPlay plugin: %d"), ret); return -1; }
	}

#ifdef ENABLE_SIO1API
	ret = SIO1_init();
	if (ret < 0) { SysMessage (_("Error initializing SIO1 plugin: %d"), ret); return -1; }
#endif

	SysPrintf("%s", _("Plugins loaded.\n"));

#ifdef _HARDLINKED
	bPluginsOpen = TRUE;
#endif

	return 0;
}

void ReleasePlugins() {
	if (Config.UseNet) {
		long ret = NET_close();
		if (ret < 0) Config.UseNet = FALSE;
	}
	NetOpened = FALSE;

#ifndef _HARDLINKED
	if (hCDRDriver != NULL || cdrIsoActive()) CDR_shutdown();
	if (hGPUDriver != NULL) GPU_shutdown();
	if (hSPUDriver != NULL) SPU_shutdown();
	if (hPAD1Driver != NULL) PAD1_shutdown();
	if (hPAD2Driver != NULL) PAD2_shutdown();
#else
	if(bPluginsOpen)
	{
		if (cdrIsoActive())
			CDR_shutdown();

		GPU_shutdown();
		SPU_shutdown();
		PAD1_shutdown();
		PAD2_shutdown();

		bPluginsOpen = FALSE;
	}
#endif

	if (Config.UseNet/* && hNETDriver != NULL*/) NET_shutdown();

#ifndef _HARDLINKED
	if (hCDRDriver != NULL) SysCloseLibrary(hCDRDriver); hCDRDriver = NULL;
	if (hGPUDriver != NULL) SysCloseLibrary(hGPUDriver); hGPUDriver = NULL;
	if (hSPUDriver != NULL) SysCloseLibrary(hSPUDriver); hSPUDriver = NULL;
	if (hPAD1Driver != NULL) SysCloseLibrary(hPAD1Driver); hPAD1Driver = NULL;
	if (hPAD2Driver != NULL) SysCloseLibrary(hPAD2Driver); hPAD2Driver = NULL;

	if (Config.UseNet && hNETDriver != NULL) {
		SysCloseLibrary(hNETDriver); hNETDriver = NULL;
	}
#endif //_HARDLINKED

#ifdef ENABLE_SIO1API
	if (hSIO1Driver != NULL) {
		SIO1_shutdown();
		SysCloseLibrary(hSIO1Driver);
		hSIO1Driver = NULL;
	}
#endif
}

void SetIsoFile(const char *filename) {
	if (filename == NULL) {
		IsoFile[0] = '\0';
		return;
	}
	strncpy(IsoFile, filename, MAXPATHLEN);
}

const char *GetIsoFile(void) {
	return IsoFile;
}

boolean UsingIso(void) {
	return (IsoFile[0] != '\0' /*|| Config.Cdr[0] == '\0'*/);
}

void SetCdOpenCaseTime(s64 time) {
	cdOpenCaseTime = time;
}

//=================================================================================

int _OpenPlugins()
{
	long ret;

	GPU_clearDynarec(clearDynarec);

	ret = CDR_open();
	if (ret < 0) { SysMessage("%s", _("Error Opening CDR Plugin")); return -1; }	
	ret = SPU_open();
	if (ret < 0) { SysMessage("%s", _("Error Opening SPU Plugin")); return -1; }
	SPU_registerCallback(SPUirq);
	ret = GPU_open(/*hWnd*/);
	if (ret < 0) { SysMessage("%s", "Error Opening GPU Plugin"); return -1; }
	ret = PAD1_open(/*&gpuDisp*/);
	if (ret < 0) { SysMessage("%s", _("Error Opening PAD1 Plugin")); return -1; }
    PAD1_registerVibration(GPU_visualVibration);
    PAD1_registerCursor(GPU_cursor);
	ret = PAD2_open(/*&gpuDisp*/);
 	if (ret < 0) { SysMessage("%s", _("Error Opening PAD2 Plugin")); return -1; }
    PAD2_registerVibration(GPU_visualVibration);
    PAD2_registerCursor(GPU_cursor);
#ifdef ENABLE_SIO1API
	ret = SIO1_open(&gpuDisp);
	if (ret < 0) { SysMessage("%s", _("Error opening SIO1 plugin!")); return -1; }
	SIO1_registerCallback(SIO1irq);
#endif

	// TODO : Does anyone use netplay?	

/*	if (Config.UseNet && !NetOpened) {
		netInfo info;
		char path[MAXPATHLEN];

		strcpy(info.EmuName, "PCSXR " PACKAGE_VERSION);
		strncpy(info.CdromID, CdromId, 9);
		strncpy(info.CdromLabel, CdromLabel, 11);
		info.psxMem = psxM;
		info.GPU_showScreenPic = GPU_showScreenPic;
		info.GPU_displayText = GPU_displayText;
		info.GPU_showScreenPic = GPU_showScreenPic;
		info.PAD_setSensitive = PAD1_setSensitive;
		sprintf(path, "%s%s", Config.BiosDir, Config.Bios);
		strcpy(info.BIOSpath, path);
		strcpy(info.MCD1path, Config.Mcd1);
		strcpy(info.MCD2path, Config.Mcd2);
		sprintf(path, "%s%s", Config.PluginsDir, Config.Gpu);
		strcpy(info.GPUpath, path);
		sprintf(path, "%s%s", Config.PluginsDir, Config.Spu);
		strcpy(info.SPUpath, path);
		sprintf(path, "%s%s", Config.PluginsDir, Config.Cdr);
		strcpy(info.CDRpath, path);
		NET_setInfo(&info);

		ret = NET_open(&gpuDisp);
		if (ret < 0) {
			if (ret == -2) {
				// -2 is returned when something in the info
				// changed and needs to be synced
				char *ptr;

				PARSEPATH(Config.Bios, info.BIOSpath);
				PARSEPATH(Config.Gpu,  info.GPUpath);
				PARSEPATH(Config.Spu,  info.SPUpath);
				PARSEPATH(Config.Cdr,  info.CDRpath);

				strcpy(Config.Mcd1, info.MCD1path);
				strcpy(Config.Mcd2, info.MCD2path);
				return -2;
			} else {
				Config.UseNet = FALSE;
			}
		} else {
			if (NET_queryPlayer() == 1) {
				if (SendPcsxInfo() == -1) Config.UseNet = FALSE;
			} else {
				if (*/RecvPcsxInfo();/* == -1) Config.UseNet = FALSE;
			}
		}
		NetOpened = TRUE;
	} else if (Config.UseNet) {
		NET_resume();
	}
*/
	return 0;
}

int OpenPlugins()
{
	int ret = 1;

	while ((ret = _OpenPlugins()) == -2)
	{
		ReleasePlugins();
		LoadMcds(Config.Mcd1, Config.Mcd2);
		if (LoadPlugins() == -1) return -1;
	}
	return ret;
}

void ClosePlugins(int iDestroyContext)
{
	long ret;

	ret = CDR_close();
	if (ret < 0) { SysMessage("%s", _("Error Closing CDR Plugin")); return; }
	ret = SPU_close();
	if (ret < 0) { SysMessage("%s", _("Error Closing SPU Plugin")); return; }
	ret = PAD1_close();
	if (ret < 0) { SysMessage("%s", _("Error Closing PAD1 Plugin")); return; }
	ret = PAD2_close();
	if (ret < 0) { SysMessage("%s", _("Error Closing PAD2 Plugin")); return; }
	ret = GPU_close(iDestroyContext);
	if (ret < 0) { SysMessage("%s", _("Error Closing GPU Plugin")); return; }
#ifdef ENABLE_SIO1API
	ret = SIO1_close();
	if (ret < 0) { SysMessage("%s", _("Error closing SIO1 plugin!")); return; }
#endif

	if (Config.UseNet) {
		NET_pause();
	}
}