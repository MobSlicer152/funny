#include "ps2.h"
#include "serial.h"
#include "x86.h"

#define PS2_BASE_PORT 0x60
#define PS2_DATA      (PS2_BASE_PORT + 0)
#define PS2_STATUS    (PS2_BASE_PORT + 4)
#define PS2_COMMAND   (PS2_BASE_PORT + 4)

#define COMMAND_GET_BYTE 0x20
#define COMMAND_SET_BYTE 0x60

#define STATUS_OUTPUT_FULL (1 << 0)
#define STATUS_INPUT_FULL  (1 << 1)

#define CONFIG_TRANSLATION (1 << 6)

static bool s_initialized;

static void WaitForOutputFull()
{
    while (!(GetPs2Status() & STATUS_OUTPUT_FULL))
    {
    }
}

static void WaitForInputEmpty()
{
	while (GetPs2Status() & STATUS_INPUT_FULL)
	{
	}
}

static u8 SendCommand(u8 command)
{
    OutByte(PS2_COMMAND, command);
    return InByte(PS2_DATA);
}

static u8 SendCommandData(u8 command, u8 data)
{
    OutByte(PS2_COMMAND, command);
    WaitForInputEmpty();
    OutByte(PS2_DATA, data);
    return InByte(PS2_DATA);
}

void InitializePs2(void)
{
    DBG("Initializing PS/2 controller");
	u8 status = SendCommand(COMMAND_GET_BYTE);
    status &= ~CONFIG_TRANSLATION;
    SendCommandData(COMMAND_SET_BYTE, status);

    s_initialized = true;
}

bool Ps2Initialized(void)
{
    return s_initialized;
}

u8 GetPs2Status(void)
{
	return InByte(PS2_STATUS);
}

u8 ReadPs2Data(void)
{
    //WaitForOutputFull();
	return InByte(PS2_DATA);
}

void WritePs2Data(u8 data)
{
	WaitForInputEmpty();
	OutByte(PS2_DATA, data);
}
