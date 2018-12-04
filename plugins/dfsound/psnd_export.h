// DFSound Plugin Exports

#ifndef H_PSND_EXPORTS
#define H_PSND_EXPORTS

#include "..\..\libpcsxcore\decode_xa.h"

unsigned short CALLBACK Plugin_SPUreadRegister(unsigned long reg);
void CALLBACK Plugin_SPUwriteRegister(unsigned long reg, unsigned short val);
void CALLBACK Plugin_SPUwriteDMA(unsigned short val);
void CALLBACK Plugin_SPUwriteDMAMem(unsigned short * pusPSXMem,int iSize);
unsigned short CALLBACK Plugin_SPUreadDMA(void);
void CALLBACK Plugin_SPUreadDMAMem(unsigned short * pusPSXMem,int iSize);
void CALLBACK Plugin_SPUasync(unsigned long cycle);
void CALLBACK Plugin_SPUregisterCallback(void (CALLBACK *callback)(void));
long CALLBACK Plugin_SPUopen(void);
long CALLBACK Plugin_SPUinit(void);
void CALLBACK Plugin_SPUplayCDDAchannel(short *pcm, int nbytes);
void CALLBACK Plugin_SPUplayADPCMchannel(xa_decode_t *xap);
long CALLBACK Plugin_SPUshutdown(void);
long CALLBACK Plugin_SPUclose(void);
void CALLBACK Plugin_GPUvisualVibration(uint32_t iSmall, uint32_t iBig);
long CALLBACK Plugin_SPUtest(void);

#endif //H_PSND_EXPORTS