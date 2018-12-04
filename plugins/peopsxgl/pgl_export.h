// gpuopengl Plugin Exports

#ifndef H_PGL_EXPORTS
#define H_PGL_EXPORTS

#include "..\..\xbox\glue\stdint.h"

typedef struct GPUFREEZETAG
{
 uint32_t ulFreezeVersion;      // should be always 1 for now (set by main emu)
 uint32_t ulStatus;             // current gpu status
 uint32_t ulControl[256];       // latest control register values
 unsigned char psxVRam[1024*1024*2]; // current VRam image (full 2 MB for ZN)
} GPUFreeze_t;

long CALLBACK Plugin_GPUinit(void);
long CALLBACK Plugin_GPUopen(/*HWND hwndGPU*/void);
long CALLBACK Plugin_GPUconfigure(void);
long CALLBACK Plugin_GPUclose(int iDestroyContext);
uint32_t CALLBACK Plugin_GPUreadStatus(void);
void CALLBACK Plugin_GPUupdateLace(void);
void CALLBACK Plugin_GPUwriteStatus(uint32_t gdata);
void CALLBACK Plugin_GPUwriteData(uint32_t gdata);
void CALLBACK Plugin_GPUreadDataMem(uint32_t *pMem, int iSize);
void CALLBACK Plugin_GPUwriteDataMem(uint32_t *pMem, int iSize);
long CALLBACK Plugin_GPUdmaChain(uint32_t *baseAddrL, uint32_t addr);
void CALLBACK Plugin_GPUhSync( int val );
void CALLBACK Plugin_GPUvBlank( int val );
uint32_t CALLBACK Plugin_GPUreadData(void);
void Plugin_GPUshowScreenPic(unsigned char * pMem);
void CALLBACK Plugin_GPUaddVertex(short sx, short sy, long long fx, long long fy, long long fz);
long CALLBACK Plugin_GPUshutdown(void);
void CALLBACK Plugin_GPUkeypressed(int keycode);
long CALLBACK Plugin_GPUfreeze(uint32_t ulGetFreezeData, GPUFreeze_t * pF);
void CALLBACK Plugin_GPUcursor(int iPlayer,int x,int y);

#endif //H_PGL_EXPORTS