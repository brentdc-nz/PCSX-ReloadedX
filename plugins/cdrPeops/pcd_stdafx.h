//  pcd_stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#ifdef _XBOX
#include <xtl.h>
#else
#include <windows.h>
#endif

#ifdef _HARDLINKED
extern void SysMessage(const char *fmt, ...);
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <process.h>
#ifndef _XBOX
#include <commdlg.h>
#include <windowsx.h>
#endif
#include <stddef.h>
#include "pcd_wnaspi32.h"
#include "pcd_scsidefs.h"

#include "pcd_defines.h"
#include "pcd_cdda.h"
#ifndef _HARDLINKED
#include "pcd_cdr.h"
#include "pcd_cfg.h"
#endif
#include "pcd_generic.h"
#include "pcd_ioctrl.h"
#include "pcd_ppf.h"
#include "pcd_read.h"
#include "pcd_scsi.h"
#include "pcd_sub.h"
#include "pcd_toc.h"
