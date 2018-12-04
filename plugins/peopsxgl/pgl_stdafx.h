/***************************************************************************
                          stdafx.h  -  description
                             -------------------
    begin                : Sun Mar 08 2009
    copyright            : (C) 1999-2009 by Pete Bernert
    web                  : www.pbernert.com   
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version. See also the license.txt file for *
 *   additional informations.                                              *
 *                                                                         *
 ***************************************************************************/

#ifndef H_PGL_STDAFX
#define H_PGL_STDAFX

#ifdef _XBOX
#include <xtl.h>
#include "..\..\xbox\fakeglx\fakeglx09.h"
#include "..\..\xbox\fakeglx\fakeglx01.h"
#endif

#ifdef _WINDOWS

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <mmsystem.h>
#include "resource.h"

#pragma warning (disable:4244)
#pragma warning (disable:4761)

#include <GL/gl.h>

#elif defined(_MACGL)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <OpenGL/gl.h>
#include <math.h> 

#define CALLBACK /* */

#elif _LINUX

#define __X11_C_
#define __inline inline

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <math.h> 
#include <X11/cursorfont.h> 

#define CALLBACK /* */

#endif

#include "pgl_gl_ext.h"

#define SHADETEXBIT(x) ((x>>24) & 0x1)
#define SEMITRANSBIT(x) ((x>>25) & 0x1)

#ifndef _WINDOWS
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT GL_BGRA
#endif
#define GL_COLOR_INDEX8_EXT 0x80E5
#endif

#endif //H_PGL_STDAFX