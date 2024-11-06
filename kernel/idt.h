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

extern void InitializeIdt(void);

typedef void (*Isr_t)(void);

// gcc did not cook when they designed this inline asm syntax, they must have smoked crack beforehand

#define MAKE_ISR_ERROR(index)                                                                                                    \
	void __attribute__((naked)) Isr##index(void)                                                                                 \
	{                                                                                                                            \
		asm("pusha;"                                                                                                         \
				"push " #index ";"                                                                                               \
				"call %P0;"                                                                                                      \
				"popa;"                                                                                                          \
				"iret"                                                                                                           \
				:                                                                                                                \
				: "i"(IsrCommon));                                                                                               \
	}

#define MAKE_ISR_NO_ERROR(index)                                                                                                 \
	void __attribute__((naked)) Isr##index(void)                                                                                 \
	{                                                                                                                            \
		asm("pusha;"                                                                                                         \
				"push 0;" /* IsrCommon expects an error code */                                                                  \
				"push " #index ";"                                                                                               \
				"call %P0;"                                                                                                      \
				"popa;"                                                                                                          \
				"iret"                                                                                                           \
				:                                                                                                                \
				: "i"(IsrCommon));                                                                                               \
	}
