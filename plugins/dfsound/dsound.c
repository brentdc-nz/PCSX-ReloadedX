/***************************************************************************
                          dsound.c  -  description
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

//*************************************************************************//
// History of changes:
//
// 2003/01/12 - Pete
// - added recording funcs
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#include "psnd_stdafx.h"

#define _IN_DSOUND

#include "psnd_externals.h"

#ifdef _XBOX //_WINDOWS

#include <dsound.h>

#ifndef _XBOX
#include "record.h"
#endif

////////////////////////////////////////////////////////////////////////
// dsound globals
////////////////////////////////////////////////////////////////////////

LPDIRECTSOUND lpDS;
LPDIRECTSOUNDBUFFER lpDSB = NULL;
DSBUFFERDESC        dsbdesc;
#ifndef _XBOX
LPDIRECTSOUNDBUFFER lpDSBP = NULL;
DSBUFFERDESC        dsbd;
DSCAPS              dscaps;
DSBCAPS             dsbcaps;
#endif

unsigned long LastWrite=0xffffffff;
unsigned long LastPlay=0;

////////////////////////////////////////////////////////////////////////
// SETUP SOUND
////////////////////////////////////////////////////////////////////////

void SetupSound(void)
{
 HRESULT dsval;
 WAVEFORMATEX pcmwf;
#ifdef _XBOX
 DSMIXBINVOLUMEPAIR dsmbvp[8] = {
 {DSMIXBIN_FRONT_LEFT, DSBVOLUME_MAX},
 {DSMIXBIN_FRONT_RIGHT, DSBVOLUME_MAX},
 {DSMIXBIN_FRONT_CENTER, DSBVOLUME_MAX},
 {DSMIXBIN_FRONT_CENTER, DSBVOLUME_MAX},
 {DSMIXBIN_BACK_LEFT, DSBVOLUME_MAX},
 {DSMIXBIN_BACK_RIGHT, DSBVOLUME_MAX},
 {DSMIXBIN_LOW_FREQUENCY, DSBVOLUME_MAX},
 {DSMIXBIN_LOW_FREQUENCY, DSBVOLUME_MAX}};

 DSMIXBINS dsmb;
#endif

 dsval = DirectSoundCreate(NULL,&lpDS,NULL);
 if(dsval!=DS_OK) 
  {
#ifndef _XBOX
   MessageBox(hWMain,"DirectSoundCreate!","Error",MB_OK);
#endif
   return;
  }

#ifndef _XBOX
 if(DS_OK!=IDirectSound_SetCooperativeLevel(lpDS,hWMain, DSSCL_PRIORITY))
  {
   if(DS_OK!=IDirectSound_SetCooperativeLevel(lpDS,hWMain, DSSCL_NORMAL))
    {
     MessageBox(hWMain,"SetCooperativeLevel!","Error",MB_OK);
     return;
    }
  }

 memset(&dsbd,0,sizeof(DSBUFFERDESC));
 dsbd.dwSize = 20; // NT4 hack! sizeof(dsbd);
 dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;                 
 dsbd.dwBufferBytes = 0; 
 dsbd.lpwfxFormat = NULL;

 dsval=IDirectSound_CreateSoundBuffer(lpDS,&dsbd,&lpDSBP,NULL);
 if(dsval!=DS_OK) 
  {
   MessageBox(hWMain, "CreateSoundBuffer (Primary)", "Error",MB_OK);
   return;
  }
#endif //_XBOX

 memset(&pcmwf, 0, sizeof(WAVEFORMATEX));
 pcmwf.wFormatTag = WAVE_FORMAT_PCM;

 if(iDisStereo) {pcmwf.nChannels = 1; pcmwf.nBlockAlign = 2;}
 else           {pcmwf.nChannels = 2; pcmwf.nBlockAlign = 4;}

 pcmwf.nSamplesPerSec = 44100;
 
 pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
 pcmwf.wBitsPerSample = 16;

#ifndef _XBOX
 dsval=IDirectSoundBuffer_SetFormat(lpDSBP,&pcmwf);
 if(dsval!=DS_OK) 
  {
   MessageBox(hWMain, "SetFormat!", "Error",MB_OK);
   return;
  }
#endif //_XBOX

#ifndef _XBOX
 dscaps.dwSize = sizeof(DSCAPS);
 dsbcaps.dwSize = sizeof(DSBCAPS);
 IDirectSound_GetCaps(lpDS,&dscaps);
 IDirectSoundBuffer_GetCaps(lpDSBP,&dsbcaps);
#endif //_XBOX

 memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
 dsbdesc.dwSize = sizeof(DSBUFFERDESC);
#ifndef _XBOX
 dsbdesc.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_STICKYFOCUS | DSBCAPS_GETCURRENTPOSITION2;
#else
 dsbdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY;
#endif
 dsbdesc.dwBufferBytes = SOUNDSIZE;
 dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf;

 dsval=IDirectSound_CreateSoundBuffer(lpDS,&dsbdesc,&lpDSB,NULL);
 if(dsval!=DS_OK) 
  {
#ifndef _XBOX
   MessageBox(hWMain,"CreateSoundBuffer (Secondary)", "Error",MB_OK);
#endif
   return;
  }

#ifndef _DEBUG
 // Make sure we start always at the beginning
 LastWrite=0xffffffff;
 LastPlay=0;
 IDirectSoundBuffer_SetCurrentPosition(lpDSB, LastWrite);
#endif

#ifndef _XBOX
 dsval=IDirectSoundBuffer_Play(lpDSBP,0,0,DSBPLAY_LOOPING);
 if(dsval!=DS_OK) 
  {
   MessageBox(hWMain,"Play (Primary)","Error",MB_OK);
   return;
  }
#endif //_XBOX

#ifdef _XBOX
 dsmb.dwMixBinCount = 8;
 dsmb.lpMixBinVolumePairs = dsmbvp;
 
 IDirectSoundBuffer_SetHeadroom(lpDSB, DSBHEADROOM_MIN);
 IDirectSoundBuffer_SetMixBins(lpDSB, &dsmb);

 IDirectSoundBuffer_SetVolume(lpDSB, DSBVOLUME_MAX);
#endif //_XBOX

 dsval=IDirectSoundBuffer_Play(lpDSB,0,0,DSBPLAY_LOOPING);
 if(dsval!=DS_OK) 
  {
#ifndef _XBOX
   MessageBox(hWMain,"Play (Secondary)","Error",MB_OK);
#endif
   return;
  }
}

////////////////////////////////////////////////////////////////////////
// REMOVE SOUND
////////////////////////////////////////////////////////////////////////

void RemoveSound(void)
{ 
 int iRes;

#ifndef _XBOX // We don't need recording on XBox
 if(iDoRecord) RecordStop();
#endif

 if(lpDSB!=NULL) 
  {
   IDirectSoundBuffer_Stop(lpDSB);
   iRes=IDirectSoundBuffer_Release(lpDSB);
   // FF says such a loop is bad... Demo says it's good... Pete doesn't care
   while(iRes!=0) iRes=IDirectSoundBuffer_Release(lpDSB);
   lpDSB=NULL;
  }                                                     

#ifndef _XBOX
 if(lpDSBP!=NULL) 
  {
   IDirectSoundBuffer_Stop(lpDSBP);
   iRes=IDirectSoundBuffer_Release(lpDSBP);
   // FF says such a loop is bad... Demo says it's good... Pete doesn't care
   while(iRes!=0) iRes=IDirectSoundBuffer_Release(lpDSBP);
   lpDSBP=NULL;
  }
#endif

 if(lpDS!=NULL) 
  {
   iRes=IDirectSound_Release(lpDS);
   // FF says such a loop is bad... Demo says it's good... Pete doesn't care
   while(iRes!=0) iRes=IDirectSound_Release(lpDS);
   lpDS=NULL;
  }
}

////////////////////////////////////////////////////////////////////////
// GET BYTES BUFFERED
////////////////////////////////////////////////////////////////////////

unsigned long SoundGetBytesBuffered(void)
{
 unsigned long cplay,cwrite;

 if(LastWrite==0xffffffff) return 0;

 IDirectSoundBuffer_GetCurrentPosition(lpDSB,&cplay,&cwrite);

 if(cplay>SOUNDSIZE) return SOUNDSIZE;

 if(cplay<LastWrite) return LastWrite-cplay;
 return (SOUNDSIZE-cplay)+LastWrite;
}

////////////////////////////////////////////////////////////////////////
// FEED SOUND DATA
////////////////////////////////////////////////////////////////////////

void SoundFeedStreamData(unsigned char* pSound,long lBytes)
{
 LPVOID lpvPtr1, lpvPtr2;
 unsigned long dwBytes1,dwBytes2; 
 unsigned long *lpSS, *lpSD;
 unsigned long dw,cplay,cwrite;
 HRESULT hr;
#ifndef _XBOX
 unsigned long status;
#endif

 if (lpDSB == NULL)
	 return;

#ifndef _XBOX // We don't need recording on XBox
 if(iDoRecord) RecordBuffer(pSound,lBytes);
#endif

#ifndef _XBOX
 IDirectSoundBuffer_GetStatus(lpDSB,&status);
 if(status&DSBSTATUS_BUFFERLOST)
  {
   if(IDirectSoundBuffer_Restore(lpDSB)!=DS_OK) return;
   IDirectSoundBuffer_Play(lpDSB,0,0,DSBPLAY_LOOPING);
  }
#endif

 IDirectSoundBuffer_GetCurrentPosition(lpDSB,&cplay,&cwrite);

 if(LastWrite==0xffffffff) LastWrite=cwrite;

/*
// mmm... security... not needed, I think
 if(LastWrite<cplay)
  {
   if((cplay-LastWrite)<=(unsigned long)lBytes)
    {
     LastWrite=0xffffffff;
     return;
    }
  }
 else
  {
   if(LastWrite<cwrite)
    {
     LastWrite=0xffffffff;
     return;
    }
  }
*/

 hr=IDirectSoundBuffer_Lock(lpDSB,LastWrite,lBytes,
                &lpvPtr1, &dwBytes1, 
                &lpvPtr2, &dwBytes2,
                0);

 if(hr!=DS_OK) {LastWrite=0xffffffff;return;}

 lpSD=(unsigned long *)lpvPtr1;
 dw=dwBytes1>>2;

 lpSS=(unsigned long *)pSound;
 while(dw) {*lpSD++=*lpSS++;dw--;}

 if(lpvPtr2)
  {
   lpSD=(unsigned long *)lpvPtr2;
   dw=dwBytes2>>2;
   while(dw) {*lpSD++=*lpSS++;dw--;}
  }

 IDirectSoundBuffer_Unlock(lpDSB,lpvPtr1,dwBytes1,lpvPtr2,dwBytes2);

 LastWrite+=lBytes;
 if(LastWrite>=SOUNDSIZE) LastWrite-=SOUNDSIZE;
 LastPlay=cplay;
}

#endif