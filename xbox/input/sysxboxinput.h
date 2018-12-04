#ifndef H_SYSXBOXINPUT
#define H_SYSXBOXINPUT

#ifdef __cplusplus
extern "C" 
{
#endif
		 
#include "..\..\plugins\PadSSSPSX\pin_PadSSSPSX.h"

//
// XBox Gmaepad Buttons
//
#define K_XBOX_A			0
#define K_XBOX_B			1
#define K_XBOX_X			2
#define K_XBOX_Y			3
#define K_XBOX_BACK			4
#define K_XBOX_START		5
#define K_XBOX_BLACK		6
#define K_XBOX_WHITE		7
#define K_XBOX_DPAD_UP		8
#define K_XBOX_DPAD_DOWN	9
#define K_XBOX_DPAD_LEFT	10
#define K_XBOX_DPAD_RIGHT	11
#define K_XBOX_LTRIG		12
#define K_XBOX_RTRIG		13
#define K_XBOX_QCOMBO		14
#define K_XBOX_LTHUMB		15
#define K_XBOX_RTHUMB		16
#define K_XBOX_NONE			99

int CheckQuit();
int GetGUIXBoxInput(int* iKey);
unsigned int ReadXBoxButtons(int iPad);
void ReadXBoxThumbSticks(unsigned int iPort, unsigned char *lx, unsigned char *ly, unsigned char *rx, unsigned char *ry);
void SetXBoxRumble(unsigned char port, unsigned int val);
void SetXBoxConfig(PinConfig config);

#ifdef __cplusplus
}
#endif

#endif //H_SYSXBOXINPUT