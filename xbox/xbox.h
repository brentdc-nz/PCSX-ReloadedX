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

#ifndef __XBOX_H
#define __XBOX_H

#define XBOX_VERSION "Pre-Beta v0.6"

// Commands
#define ID_FILE_RUNBIOS 001
#define ID_FILE_RUN_ISO 002
#define ID_EMULATOR_RUN 003

extern char PcsxrDir[256];

#ifdef __cplusplus
extern "C"
{
#endif

int RunCommand(int iCommand, const char* isofile);
void SysRunGUI();

#ifdef __cplusplus
}
#endif

#endif //__XBOX_H