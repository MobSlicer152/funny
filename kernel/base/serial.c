#include "serial.h"
#include "libc.h"
#include "x86.h"


#define COM1_BASE             0x3f8
#define COM1_RECEIVE          (COM1_BASE + 0)
#define COM1_TRANSMIT         (COM1_BASE + 0)
#define COM1_INTERRUPT_ENABLE (COM1_BASE + 1)
#define COM1_DIVISOR_LSB      (COM1_BASE + 0)
#define COM1_DIVISOR_MSB      (COM1_BASE + 1)
#define COM1_INTERRUPT_ID     (COM1_BASE + 2)
#define COM1_FIFO_CTRL        (COM1_BASE + 2)
#define COM1_LINE_CTRL        (COM1_BASE + 3)
#define COM1_MODEM_CTRL       (COM1_BASE + 4)
#define COM1_LINE_STATUS      (COM1_BASE + 5)
#define COM1_MODEM_STATUS     (COM1_BASE + 6)
#define COM1_SCRATCH          (COM1_BASE + 7)

#define LINE_CTRL_DIVISOR_ACCESS_LATCH (1 << 7)
#define LINE_CTRL_BREAK_ENABLE         (1 << 6)
#define LINE_CTRL_PARITY_NONE          (0 << 3)
#define LINE_CTRL_PARITY_ODD           (1 << 3)
#define LINE_CTRL_PARITY_EVEN          (3 << 3)
#define LINE_CTRL_PARITY_MARK          (5 << 3)
#define LINE_CTRL_PARITY_SPACE         (7 << 3)
#define LINE_CTRL_STOP_1_BIT           (0 << 2)
#define LINE_CTRL_STOP_2_BIT           (1 << 2)
#define LINE_CTRL_DATA_5_BIT           (0 << 0)
#define LINE_CTRL_DATA_6_BIT           (1 << 0)
#define LINE_CTRL_DATA_7_BIT           (2 << 0)
#define LINE_CTRL_DATA_8_BIT           (3 << 0)

#define INTERRUPT_ENABLE_MODEM_STATUS    (1 << 3)
#define INTERRUPT_ENABLE_RECEIVER_STATUS (1 << 2)
#define INTERRUPT_ENABLE_TRANSMIT_EMPTY  (1 << 1)
#define INTERRUPT_ENABLE_RECEIVED_DATA   (1 << 0)

#define FIFO_CTRL_TRIGGER_1      (0 << 6)
#define FIFO_CTRL_TRIGGER_4      (1 << 6)
#define FIFO_CTRL_TRIGGER_8      (2 << 6)
#define FIFO_CTRL_TRIGGER_14     (3 << 6)
#define FIFO_CTRL_DMA_MODE       (1 << 3)
#define FIFO_CTRL_CLEAR_TRANSMIT (1 << 2)
#define FIFO_CTRL_CLEAR_RECEIVE  (1 << 1)
#define FIFO_CTRL_ENABLE         (1 << 0)

#define MODEM_CTRL_LOOPBACK     (1 << 4)
#define MODEM_CTRL_OUT_2        (1 << 3)
#define MODEM_CTRL_OUT_1        (1 << 2)
#define MODEM_CTRL_REQUEST_SEND (1 << 1)
#define MODEM_CTRL_DATA_READY   (1 << 0)

#define LINE_STATUS_DATA_READY     (1 << 0)
#define LINE_STATUS_TRANSMIT_EMPTY (1 << 5)

static bool TestSerial(void)
{
	u8 mode = InByte(COM1_MODEM_CTRL);
	OutByte(COM1_MODEM_CTRL, MODEM_CTRL_LOOPBACK | MODEM_CTRL_OUT_1 | MODEM_CTRL_OUT_2 | MODEM_CTRL_REQUEST_SEND);
	OutByte(COM1_TRANSMIT, 0x69);
	bool good = InByte(COM1_RECEIVE) == 0x69;
	OutByte(COM1_MODEM_CTRL, mode);
	return good;
}

void InitializeSerial(u16 baud)
{
	OutByte(COM1_INTERRUPT_ENABLE, 0); // disable interrupts
	SetBaud(baud);
	OutByte(COM1_LINE_CTRL, LINE_CTRL_DATA_8_BIT | LINE_CTRL_PARITY_NONE | LINE_CTRL_STOP_1_BIT);
	OutByte(COM1_FIFO_CTRL, FIFO_CTRL_TRIGGER_14 | FIFO_CTRL_CLEAR_RECEIVE | FIFO_CTRL_CLEAR_TRANSMIT | FIFO_CTRL_ENABLE);
	OutByte(
		COM1_INTERRUPT_ENABLE, INTERRUPT_ENABLE_MODEM_STATUS | INTERRUPT_ENABLE_RECEIVED_DATA | INTERRUPT_ENABLE_RECEIVER_STATUS);
	ASSERT(TestSerial() == true);

	OutByte(COM1_MODEM_CTRL, MODEM_CTRL_OUT_1 | MODEM_CTRL_OUT_2 | MODEM_CTRL_DATA_READY | MODEM_CTRL_REQUEST_SEND);
}

void SetBaud(u16 baud)
{
	u8 ctrl = InByte(COM1_LINE_CTRL);
	OutByte(COM1_LINE_CTRL, LINE_CTRL_DIVISOR_ACCESS_LATCH);
	OutByte(COM1_DIVISOR_LSB, baud & 0xFF);
	OutByte(COM1_DIVISOR_MSB, baud >> 8);
	OutByte(COM1_LINE_CTRL, ctrl);
}

static bool DataReady(void)
{
	return InByte(COM1_LINE_STATUS) & LINE_STATUS_DATA_READY;
}

u8 ReadSerial(void)
{
	while (!DataReady())
	{
		Halt();
	}
	return InByte(COM1_RECEIVE);
}

static bool TransmitEmpty(void)
{
	return InByte(COM1_LINE_STATUS) & LINE_STATUS_TRANSMIT_EMPTY;
}

static void WaitForTransmit(void)
{
	while (!TransmitEmpty())
	{
		Halt();
	}
}

void WriteSerial(const u8* data, usize size)
{
	for (usize i = 0; i < size; i++)
	{
		WaitForTransmit();
		if (data[i] == '\n')
		{
			OutByte(COM1_TRANSMIT, '\r');
			WaitForTransmit();
		}
		OutByte(COM1_TRANSMIT, data[i]);
	}
}

void VDebugPrint(cstr msg, va_list args)
{
	char buf[1024];

	s32 size = vsnprintf(buf, ARRAY_SIZE(buf), msg, args);
	WriteSerial((u8*)buf, size);
}

void DebugPrint(cstr msg, ...)
{
	va_list args;
	va_start(args, msg);
	VDebugPrint(msg, args);
	va_end(args);
}
