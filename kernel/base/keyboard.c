#include "keyboard.h"
#include "libc.h"
#include "ps2.h"
#include "serial.h"
#include "x86.h"

static bool s_keyboardState[KeyCodeCount];
static bool s_lastKeyboardState[KeyCodeCount];

#define COMMAND_SET_LIGHTS       0xed
#define COMMAND_ECHO             0xee
#define COMMAND_SCANCODE_SET     0xf0
#define COMMAND_IDENTIFY         0xf2
#define COMMAND_SET_TYPEMATIC    0xf3
#define COMMAND_ENABLE_SCANNING  0xf4
#define COMMAND_DISABLE_SCANNING 0xf5
#define COMMAND_SET_DEFAULTS     0xf6
#define COMMAND_RESEND           0xfe
#define COMMAND_RESET_AND_TEST   0xff

#define COMMAND_NO_DATA (1 << 7)

#define KEY_RELEASE 0xf0

#define RESPONSE_ACKNOWLEDGE    0xfa
#define RESPONSE_ECHO           0xee
#define RESPONSE_SELF_TEST_GOOD 0xaa
#define RESPONSE_RESEND         0xfe

static void SendCommand(u8 command, u8 data)
{
	u8 response = 0;
	do
	{
		WritePs2Data(command);
		response = ReadPs2Data();
	} while (response == RESPONSE_RESEND);
    if (response != RESPONSE_ACKNOWLEDGE)
    {
        DBG("keyboard error 0x%02X", response);
    }
	if (!(data & COMMAND_NO_DATA))
	{
		do
		{
			WritePs2Data(data);
			response = ReadPs2Data();
		} while (response == RESPONSE_RESEND);
        if (response != RESPONSE_ACKNOWLEDGE)
        {
            DBG("keyboard error 0x%02X", response);
        }
	}
}

void InitializeKeyboard(void)
{
    DBG("Initializing keyboard");
    SendCommand(COMMAND_SET_DEFAULTS, COMMAND_NO_DATA);

    SendCommand(COMMAND_DISABLE_SCANNING, COMMAND_NO_DATA);
	SetKeyboardLights(false, false, false);
    SendCommand(COMMAND_SCANCODE_SET, 2);
	SendCommand(COMMAND_ENABLE_SCANNING, COMMAND_NO_DATA);
}

void SwapKeyboardState(void)
{
	memcpy(s_lastKeyboardState, s_keyboardState, sizeof(s_keyboardState));
}

void SetKeyboardLights(bool scroll, bool num, bool caps)
{
	SendCommand(COMMAND_SET_LIGHTS, (scroll << 0) | (num << 1) | (caps << 2));
}

bool GetKey(KeyCode_t code)
{
    //DBG("0x%02X last 0x%02X current 0x%02X", code, s_lastKeyboardState[code], s_keyboardState[code]);
	return s_lastKeyboardState[code];
}

void UpdateKeyboard(void)
{
    static bool released = false;
	u8 code = ReadPs2Data();
    
    if (code == KEY_RELEASE)
    {
        released = true;
        return;
    }

    if (code < KeyCodeCount)
    {
        DBG("0x%02X = %s", code, released ? "false" : "true");
        s_keyboardState[code] = !released;
        released = false;
    }
}
