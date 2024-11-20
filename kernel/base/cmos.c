#include "cmos.h"
#include "libc.h"
#include "macros.h"
#include "x86.h"

#define CMOS_BASE     0x70
#define CMOS_REGISTER (CMOS_BASE + 1)

#define CMOS_NMI (1 << 7)

#define CMOS_SECONDS  0x0
#define CMOS_MINUTES  0x2
#define CMOS_HOURS    0x4
#define CMOS_WEEKDAY  0x6
#define CMOS_DAY      0x7
#define CMOS_MONTH    0x8
#define CMOS_YEAR     0x9
#define CMOS_STATUS_A 0xa
#define CMOS_STATUS_B 0xb

#define CMOS_HOURS_PM             (1 << 7)
#define CMOS_A_UPDATE_IN_PROGRESS (1 << 7)
#define CMOS_B_24_HOUR            (1 << 1)
#define CMOS_B_BINARY             (1 << 2)

u8 ReadCmosRegister(u8 index)
{
	u8 nmi = InByte(CMOS_BASE) & CMOS_NMI;
	OutByte(CMOS_BASE, nmi | index);
	IoWait();
	return InByte(CMOS_REGISTER);
}

static bool RtcUpdating(void)
{
	return ReadCmosRegister(CMOS_STATUS_A) & CMOS_A_UPDATE_IN_PROGRESS;
}

static void RtcWaitForUpdate(void)
{
	while (RtcUpdating())
	{
	}
}

static u8 ReadRtcValue(u8 index)
{
	u8 statusB = ReadCmosRegister(CMOS_STATUS_B);
	u8 value = ReadCmosRegister(index);
	u8 final = value;

	if (!(statusB & CMOS_B_BINARY))
	{
		// why the fuck would anyone want BCD
		final = ((value & 0xf0) >> 4) * 10 + (value & 0x0f);
	}

	if (index == CMOS_HOURS && !(statusB & CMOS_B_24_HOUR))
	{
		// if pm add 12 and wrap around at 24 hours
		final += 12 * ((value & CMOS_HOURS_PM) >> 7) % 24;
	}

	return final;
}

void ReadRtc(u32* seconds, u32* minutes, u32* hours, u32* day, u32* month, u32* year)
{
	RtcWaitForUpdate();

	if (seconds)
	{
		*seconds = ReadRtcValue(CMOS_SECONDS);
	}
	if (minutes)
	{
		*minutes = ReadRtcValue(CMOS_MINUTES);
	}
	if (hours)
	{
		*hours = ReadRtcValue(CMOS_HOURS);
	}
	if (day)
	{
		*day = ReadRtcValue(CMOS_DAY);
	}
	if (month)
	{
		*month = ReadRtcValue(CMOS_MONTH);
	}
	if (year)
	{
		*year = ReadRtcValue(CMOS_YEAR) + 2000; // this software is from 2024, it'll be a minute till this is wrong
	}
}

cstr GetTimeString(void)
{
	static char buffer[128];

	u32 seconds = 0;
	u32 minutes = 0;
	u32 hours = 0;
	u32 day = 0;
	u32 month = 0;
	u32 year = 0;
	ReadRtc(&seconds, &minutes, &hours, &day, &month, &year);
    // HH:MM:SS dd-mm-YYYY
	snprintf(buffer, ARRAY_SIZE(buffer), "%02u:%02u:%02u %02u-%02u-%04u", hours, minutes, seconds, day, month, year);
	return buffer;
}
