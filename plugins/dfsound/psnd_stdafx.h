/***************************************************************************
                           StdAfx.h  -  description
                             -------------------
    begin                : Wed May 15 2002
    copyright            : (C) 2002 by Pete Bernert
    email                : BlackDove@addcom.de
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

#ifdef _XBOX
#include "xtl.h"
#include "..\..\libpcsxcore\decode_xa.h"
#endif

#ifndef INLINE
#define INLINE __inline
#endif

#pragma warning (disable:4996)
#if defined (__GNUC__) || defined (__clang__)
#define UNUSED_VARIABLE __attribute__((unused))
#else
#define UNUSED_VARIABLE
#endif
