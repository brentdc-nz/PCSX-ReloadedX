// cdrPeops Plugin Exports

#ifndef H_PCD_EXPORTS
#define H_PCD_EXPORTS

#ifdef _HARDLINKED
#include "..\..\xbox\glue\stdint.h"
#include "..\..\libpcsxcore\psemu_plugin_defs.h"
#endif

long CALLBACK Plugin_CDRinit(void);
long CALLBACK Plugin_CDRshutdown(void);
long CALLBACK Plugin_CDRopen(void);
long CALLBACK Plugin_CDRclose(void);
long CALLBACK Plugin_CDRreadTrack(unsigned char *time);
unsigned char* CALLBACK Plugin_CDRgetBuffer(void);
long CALLBACK Plugin_CDRgetTN(unsigned char *buffer);
long CALLBACK Plugin_CDRgetTD(unsigned char track, unsigned char *buffer);
unsigned char* CALLBACK Plugin_CDRgetBufferSub(void);
long CALLBACK Plugin_CDRplay(unsigned char* sector);
long CALLBACK Plugin_CDRstop(void);
long CALLBACK Plugin_CDRgetStatus(struct CdrStat *stat);

#endif //H_PCD_EXPORTS