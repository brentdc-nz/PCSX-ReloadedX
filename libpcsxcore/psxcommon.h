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
* This file contains common definitions and includes for all parts of the 
* emulator core.
*/

#ifndef __PSXCOMMON_H__
#define __PSXCOMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _XBOX
#pragma warning (disable:4018)
#endif

#ifdef _XBOX
#include "..\libpcsxcore\config.h"
#include <xtl.h>
#endif

// System includes
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "..\xbox\glue\stdint.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include "..\xbox\zlib\zlib.h"

// Define types
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef intptr_t sptr;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uptr;

typedef uint8_t boolean;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#pragma warning (disable: 4244)

// Local includes
#include "system.h"
#include "debug.h"

#if defined (__LINUX__) || defined (__MACOSX__)
#define strnicmp strncasecmp
#endif
#define inline __inline
#undef _
//#define _(String) gettext(String)
#ifdef gettext_noop
#  define N_(String) gettext_noop (String)
#else
#  define N_(String) (String)
#endif
#define _(msgid) msgid

extern FILE *emuLog;
extern int Log;

void __Log(char *fmt, ...);

typedef struct {
	char Gpu[MAXPATHLEN];
	char Spu[MAXPATHLEN];
	char Cdr[MAXPATHLEN];
	char Pad1[MAXPATHLEN];
	char Pad2[MAXPATHLEN];
	char Net[MAXPATHLEN];
	char Sio1[MAXPATHLEN];
	char Mcd1[MAXPATHLEN];
	char Mcd2[MAXPATHLEN];
	char Bios[MAXPATHLEN];
	char BiosDir[MAXPATHLEN];
#ifndef _HARDLINKED
	char PluginsDir[MAXPATHLEN]
#endif
	char PatchesDir[MAXPATHLEN];
	char IsoImgDir[MAXPATHLEN];
	boolean Xa;
	boolean SioIrq;
	boolean Mdec;
	boolean PsxAuto;
	u8      Cdda;
	boolean HLE;
	boolean SlowBoot;
	boolean Debug;
	boolean PsxOut;
	boolean SpuIrq;
	boolean RCntFix;
	boolean UseNet;
	boolean VSyncWA;
	boolean NoMemcard;
	boolean Widescreen;
	boolean HideCursor;
	boolean SaveWindowPos;
	s32 WindowPos[2];
	u8 Cpu; // CPU_DYNAREC or CPU_INTERPRETER
	u8 PsxType; // PSX_TYPE_NTSC or PSX_TYPE_PAL
	u32 RewindCount;
	u32 RewindInterval;
	u8 HackFix;
#ifdef _WIN32
	char Lang[256];
#endif
} PcsxConfig;

extern PcsxConfig Config;
extern boolean NetOpened;

// It is safe if these overflow
extern u32 rewind_counter;
extern u8 vblank_count_hideafter;

#define gzfreeze(ptr, size) { \
	if (Mode == 1) gzwrite(f, ptr, size); \
	if (Mode == 0) gzread(f, ptr, size); \
}

// Make the timing events trigger faster as we are currently assuming everything
// takes one cycle, which is not the case on real hardware.
// FIXME: Count the proper cycle and get rid of this
#define BIAS	2
#define PSXCLK	33868800	/* 33.8688 MHz */

enum {
	PSX_TYPE_NTSC = 0,
	PSX_TYPE_PAL
}; // PSX Types

enum {
	CPU_DYNAREC = 0,
	CPU_INTERPRETER
}; // CPU Types

enum {
	CDDA_ENABLED_LE = 0,
	CDDA_DISABLED,
	CDDA_ENABLED_BE
}; // CDDA Types

int EmuInit();
void EmuReset();
void EmuShutdown();
void EmuUpdate();

#ifdef _XBOX //TODO: Move me!
int LoadCoreConfig(PcsxConfig *Conf);
void SaveCoreConfig();
#endif

#ifdef __cplusplus
}
#endif
#endif
