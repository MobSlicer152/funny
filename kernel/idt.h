#pragma once

#include "kernel.h"

typedef struct IdtDescriptor
{
	uptr address;
	u16 size;
} IdtDescriptor_t;

typedef enum IdtGateType
{
	IdtGateTypeTask = 0x5,
	IdtGateTypeInterrupt16 = 0x6,
	IdtGateTypeTrap16 = 0x7,
	IdtGateTypeInterrupt32 = 0xE,
	IdtGateTypeTrap32 = 0xF
} IdtGateType_t;

typedef struct IdtEntry
{
	u16 offsetLower;
	u16 selector;
	struct
	{
		u8 present : 1;
		u8 dpl : 2;
		u8 reserved : 1;
		u8 gateType : 4;
		u8 reserved2;
	};
	u16 offsetUpper;
} IdtEntry_t;

// initialize and load the idt
extern void InitializeIdt(void);
