#ifndef H_PIN_EXPORT
#define H_PIN_EXPORT

#include "..\..\xbox\glue\stdint.h"
#include "..\..\libpcsxcore\psemu_plugin_defs.h"

long CALLBACK Plugin_PADinit(long flags);
void CALLBACK Plugin_PADshutdown(void);
long CALLBACK Plugin_PADreadPort1(PadDataS* pads);
long CALLBACK Plugin_PADreadPort2(PadDataS* pads);
unsigned char CALLBACK Plugin_PADstartPoll(int pad);
unsigned char CALLBACK Plugin_PADpoll(unsigned char value);
s32 CALLBACK Plugin_PADopen (/*HWND hWnd*/void);
long CALLBACK Plugin_PADclose (void);
long CALLBACK Plugin_PADkeypressed1(void);
long CALLBACK Plugin_PADkeypressed2(void);
void CALLBACK Plugin_PADregisterVibration(void (CALLBACK *callback)(uint32_t, uint32_t));

#endif //H_PIN_EXPORT