#include <xtl.h>
#include "xbinput.h"
#include "sysxboxinput.h"
#include "..\xbox.h"
#include "..\gui\xboxgui.h"
#include "..\gui\guiwindowkeys.h"

// Deadzone for the gamepad inputs
const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );

#define ANALOG_SENSITIVITY 80

class cSysInputXbox
{
public:
	cSysInputXbox();
	~cSysInputXbox();
	
	bool CheckQuit();
	void UpdateGamepads();
	bool GetGUIInput(int* iKey);
	DWORD ReadButtons(int iPad);
	void ReadThumbSticks(unsigned int iPort, unsigned char *lx, unsigned char *ly, unsigned char *rx, unsigned char *ry);
	void SetRumble(unsigned char port, unsigned int val);
	void SetConfig(PinConfig config);

private:
	XDEVICE_PREALLOC_TYPE*	m_InputDeviceTypes;
	DWORD					m_dwNumInputDeviceTypes;
	XBGAMEPAD*				m_Gamepad;
	bool					m_bRumbleEnabled;
	bool					m_bQuitComboPressed;
	
	int						m_iKeyPressed;
	int						m_iPreviousKeyState;

	DWORD					m_dwSwitchTime;

	PinConfig				m_GamepadConfig;
};

//======================================

cSysInputXbox g_SysInputXbox;

cSysInputXbox::cSysInputXbox()
{
	XInitDevices( m_dwNumInputDeviceTypes, m_InputDeviceTypes );
	XBInput_CreateGamepads( &m_Gamepad );

	m_iPreviousKeyState = K_XBOX_NONE;

	m_dwSwitchTime = GetTickCount();

	m_bRumbleEnabled = false;//true; //FIXME: Rumble works but can get stuck on
	m_bQuitComboPressed = false;
}

cSysInputXbox::~cSysInputXbox()
{
}

bool cSysInputXbox::CheckQuit()
{
	return m_bQuitComboPressed;
}

void cSysInputXbox::UpdateGamepads()
{
	// Read the input for all connected gamepads
	XBInput_GetInput( m_Gamepad );
    
	INT nThumbLX = 0;
	INT nThumbLY = 0;
	INT nThumbRX = 0;
	INT nThumbRY = 0;

	for( DWORD i=0; i<4; i++ )
	{
		if( m_Gamepad[i].hDevice )
		{
			// Only account for thumbstick info beyond the deadzone
			if( m_Gamepad[i].sThumbLX > XINPUT_DEADZONE ||
				m_Gamepad[i].sThumbLX < -XINPUT_DEADZONE )
				nThumbLX += m_Gamepad[i].sThumbLX;
			if( m_Gamepad[i].sThumbLY > XINPUT_DEADZONE ||
				m_Gamepad[i].sThumbLY < -XINPUT_DEADZONE )
				nThumbLY += m_Gamepad[i].sThumbLY;
			if( m_Gamepad[i].sThumbRX > XINPUT_DEADZONE ||
				m_Gamepad[i].sThumbRX < -XINPUT_DEADZONE )
				nThumbRX += m_Gamepad[i].sThumbRX;
			if( m_Gamepad[i].sThumbRY > XINPUT_DEADZONE ||
				m_Gamepad[i].sThumbRY < -XINPUT_DEADZONE )
				nThumbRY += m_Gamepad[i].sThumbRY;
		}
	}

	// Button combo to return to the Xbox dashboard
	if(m_Gamepad[0].bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 0)
	{
		if(m_Gamepad[0].bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 0)
		{
			if(m_Gamepad[0].bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK])
			{
				m_bQuitComboPressed = true;
				return;
			}
		}
	}

	m_iKeyPressed = K_XBOX_NONE;

	// Return to the GUI
	if( m_Gamepad[0].wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		m_iKeyPressed = K_XBOX_RTHUMB;

		if(m_iPreviousKeyState != K_XBOX_RTHUMB)
		{
			// We can't switch too fast
			if(GetTickCount() > m_dwSwitchTime + 2500) // TODO: Use key state rather than time!
			{
				m_dwSwitchTime = GetTickCount();

				if(IsXBoxGUIActive())
					RunCommand(ID_EMULATOR_RUN, "");
				else
				{
//					SetXBoxGUIStartWindow(WINDOW_INGAME_CONFIG); // WIP
					SysRunGUI();
				}
			}
		}
	}
}

bool cSysInputXbox::GetGUIInput(int* iKey)
{
	if(m_Gamepad[0].wButtons & XINPUT_GAMEPAD_DPAD_UP)
		m_iKeyPressed = K_XBOX_DPAD_UP;

	if(m_Gamepad[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		m_iKeyPressed = K_XBOX_DPAD_DOWN;

	if(m_Gamepad[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		m_iKeyPressed = K_XBOX_DPAD_LEFT;

	if(m_Gamepad[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		m_iKeyPressed = K_XBOX_DPAD_RIGHT;

	if(g_Gamepads[0].bAnalogButtons[XINPUT_GAMEPAD_A] > ANALOG_SENSITIVITY)
		m_iKeyPressed = K_XBOX_A;

	if(g_Gamepads[0].bAnalogButtons[XINPUT_GAMEPAD_X] > ANALOG_SENSITIVITY)
		m_iKeyPressed = K_XBOX_X;

	if(g_Gamepads[0].bAnalogButtons[XINPUT_GAMEPAD_B] > ANALOG_SENSITIVITY)
		m_iKeyPressed = K_XBOX_B;

	if(g_Gamepads[0].bAnalogButtons[XINPUT_GAMEPAD_Y] > ANALOG_SENSITIVITY)
		m_iKeyPressed = K_XBOX_Y;

	if(m_iPreviousKeyState != m_iKeyPressed)
	{
		m_iPreviousKeyState = m_iKeyPressed;

		if(m_iKeyPressed!= K_XBOX_NONE)
		{
			*iKey = m_iKeyPressed;
			return true;
		}
	}

	return false;
}

DWORD cSysInputXbox::ReadButtons(int iPad)
{
	DWORD dwXBoxButtons = 0;

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_START )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_START];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_BACK )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_BACK];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_DPAD_UP )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_DPAD_UP];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_DPAD_DOWN];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_DPAD_LEFT )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_DPAD_LEFT];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_DPAD_RIGHT];

	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_LEFT_THUMB )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_LTHUMB];

	// mm should work but hold too long and it'll go back to main menu
	if(m_Gamepad[iPad].wButtons & XINPUT_GAMEPAD_RIGHT_THUMB )
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_RTHUMB];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_A] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_A];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_X] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_X];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_B] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_B];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_Y] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_Y];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_BLACK] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_BLACK];

	if(g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_WHITE] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_WHITE];

	if (g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_LTRIG];
	
	if (g_Gamepads[iPad].bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > ANALOG_SENSITIVITY)
		dwXBoxButtons |= m_GamepadConfig.buttons[iPad][K_XBOX_RTRIG];

	if(m_iPreviousKeyState != m_iKeyPressed)
		m_iPreviousKeyState = m_iKeyPressed;

	return dwXBoxButtons;
}

void cSysInputXbox::ReadThumbSticks(unsigned int iPort, unsigned char *lx, unsigned char *ly, unsigned char *rx, unsigned char *ry)
{
	if(g_Gamepads[iPort].hDevice)
	{
		if (fabs(g_Gamepads[iPort].fX1) > (float)(30/100.0f)) 
			*lx = (unsigned char)(127.5f + g_Gamepads[iPort].fX1 * 127.5f);
		else
			*lx = 128 ;

		if (fabs(g_Gamepads[iPort].fY1) > (float)(30/100.0f))  
			*ly = (unsigned char)(127.5f - g_Gamepads[iPort].fY1 * 127.5f);
		else
			*ly = 128 ;

		if (fabs(g_Gamepads[iPort].fX2) > (float)(30/100.0f))  
			*rx = (unsigned char)(127.5f + g_Gamepads[iPort].fX2 * 127.5f);
		else
			*rx = 128 ;

		if (fabs(g_Gamepads[iPort].fY2) > (float)(30/100.0f))  
			*ry = (unsigned char)(127.5f - g_Gamepads[iPort].fY2 * 127.5f);
		else
			*ry = 128 ;
	}
	else
	{
		*lx = *ly = *rx = *ry = 128 ;
	}	
}

void cSysInputXbox::SetRumble(unsigned char port, unsigned int val)
{
	if (g_Gamepads[port].hDevice)
	{
		if (port)
		{
			if (!m_bRumbleEnabled)
				return ;
			val += (/*m_psxfix_rumble2*/128 << 8); //TODO
		}
		else
		{
			if (!m_bRumbleEnabled)
				return ;
			val += (/*m_psxfix_rumble1*/128 << 8); //TODO
		}

		if (val > 65535)
			val = 65535;

		g_Gamepads[port].Feedback.Rumble.wLeftMotorSpeed  = val;
		g_Gamepads[port].Feedback.Rumble.wRightMotorSpeed = val;

		if(g_Gamepads[port].Feedback.Header.dwStatus != ERROR_IO_PENDING)
		{
			XInputSetState(g_Gamepads[port].hDevice, &(g_Gamepads[port].Feedback));
		}
	}
}

void cSysInputXbox::SetConfig(PinConfig config)
{
	m_GamepadConfig = config;
}

//======================================

int CheckQuit()
{
	return g_SysInputXbox.CheckQuit();
}

int GetGUIXBoxInput(int* iKey)
{
	g_SysInputXbox.UpdateGamepads();

	return g_SysInputXbox.GetGUIInput(iKey);
}

unsigned int ReadXBoxButtons(int iPad)
{
	unsigned int value;

	value = 0xFFFF;

	g_SysInputXbox.UpdateGamepads();

	value = (~(g_SysInputXbox.ReadButtons(iPad) )) & 0xFFFF;

	value = ((value>>8)&0xFF) | ((value<<8) & 0xFF00);

	return value;
}

void SetXBoxRumble(unsigned char port, unsigned int val)
{
	g_SysInputXbox.SetRumble(port, val);
}

void SetXBoxConfig(PinConfig config)
{
	g_SysInputXbox.SetConfig(config);
}

void ReadXBoxThumbSticks(unsigned int iPort, unsigned char *lx, unsigned char *ly, unsigned char *rx, unsigned char *ry)
{
	g_SysInputXbox.ReadThumbSticks(iPort, lx, ly, rx, ry);
}