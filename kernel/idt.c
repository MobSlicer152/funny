#include "idt.h"
#include "irq.h"
#include "libc.h"
#include "serial.h"
#include "timer.h"
#include "x86.h"

typedef void (*Isr_t)(void);

typedef struct IdtDescriptor
{
	u16 size;
	struct IdtEntry* address;
} ATTRIBUTE(packed) IdtDescriptor_t;

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
	union {
		u16 flags;
		struct
		{
			u8 reserved2;
			u8 gateType : 4;
			u8 reserved : 1;
			u8 dpl : 2;
			u8 present : 1;
		};
	};
	u16 offsetUpper;
} ATTRIBUTE(packed) IdtEntry_t;

#define INTERRUPT_STACK STRINGIZE_EXPAND(INTERRUPT_STACK_END)

#define MAKE_ISR_ERROR(index)                                                                                                    \
	void ATTRIBUTE(naked) Isr##index(void)                                                                                       \
	{                                                                                                                            \
		asm("pusha;"                                                                                                             \
			"movl 36(%%esp), %%eax;" /* get the error code above preserved registers */                                          \
			"mov %%esp, %%ebp;"      /* load interrupt stack */                                                                  \
			"mov $" INTERRUPT_STACK ", %%esp;"                                                                                               \
			"pushl %%eax;" /* pass it to IsrCommon */                                                                            \
			"pushl " #index ";"                                                                                                  \
			"call %P0;"                                                                                                          \
			"addl $8, %%esp;"                                                                                                    \
			"mov %%ebp, %%esp;"                                                                                                  \
			"popa;"                                                                                                              \
			"addl $4, %%esp;" /* remove the error code from the stack */                                                         \
			"iret"                                                                                                               \
			:                                                                                                                    \
			: "i"(IsrCommon));                                                                                                   \
	}

#define MAKE_ISR_NO_ERROR(index)                                                                                                 \
	void ATTRIBUTE(naked) Isr##index(void)                                                                                       \
	{                                                                                                                            \
		asm("pusha;"                                                                                                             \
			"mov %%esp, %%ebp;" /* load interrupt stack */                                                                       \
			"mov $" INTERRUPT_STACK ", %%esp;"                                                                                               \
			"pushl $0;" /* IsrCommon expects an error code */                                                                    \
			"pushl $" #index ";"                                                                                                 \
			"call %P0;"                                                                                                          \
			"addl $8, %%esp;"                                                                                                    \
			"mov %%ebp, %%esp;"                                                                                                  \
			"popa;"                                                                                                              \
			"iret"                                                                                                               \
			:                                                                                                                    \
			: "i"(IsrCommon));                                                                                                   \
	}

static IdtEntry_t s_idt[48];

static void RegisterGate(Isr_t handler, InterruptType_t type, IdtGateType_t gateType)
{
	DebugPrint("Registering type 0x%X ISR 0x%X for gate 0x%X\n", gateType, (uptr)handler, type);

	s_idt[type].offsetLower = (uptr)handler & 0xFFFF;
	s_idt[type].offsetUpper = (uptr)handler >> 16;

	s_idt[type].gateType = gateType;

	// these fields are always the same
	s_idt[type].selector = KERNEL_CODE_SELECTOR;
	s_idt[type].dpl = 0;
	s_idt[type].present = true;
}

InterruptType_t lastInterrupt;

static void IsrCommon(InterruptType_t type, u32 error)
{
	(void)error;

	DisableInterrupts();

	lastInterrupt = type;
	switch (type)
	{
	case InterruptTypeDivisionError:
	case InterruptTypeDebug:
	case InterruptTypeNonMaskable:
	case InterruptTypeBreakpoint:
	case InterruptTypeOverflow:
	case InterruptTypeBoundRangeExceeded:
	case InterruptTypeInvalidOpcode:
	case InterruptTypeDeviceNotAvailable:
	case InterruptTypeDoubleFault:
	case InterruptTypeCoprocessorSegmentOverrun:
	case InterruptTypeInvalidTss:
	case InterruptTypeSegmentNotPresent:
	case InterruptTypeStackSegmentFault:
	case InterruptTypeGeneralProtectionFault:
	case InterruptTypePageFault:
	case InterruptTypeX87FloatException:
	case InterruptTypeAlignmentCheck:
	case InterruptTypeMachineCheck:
	case InterruptTypeSimdFloatException:
	case InterruptTypeVirtualizationException:
	case InterruptTypeControlProtectionException:
	case InterruptTypeHypervisorInjectionException:
	case InterruptTypeVmmCommunicationException:
	case InterruptTypeSecurityException:
		DebugPrint("Got exception 0x%X with error 0x%X\n", type, error);
		break;

	case InterruptTypeTimer:
		UpdateTimer();
		break;
	case InterruptTypeKeyboard:
	case InterruptTypeCascade:
	case InterruptTypeCom2:
	case InterruptTypeCom1:
	case InterruptTypeLpt2:
	case InterruptTypeFloppy:
	case InterruptTypeLpt1:
	case InterruptTypeCmos:
	case InterruptTypePeripheral1:
	case InterruptTypePeripheral2:
	case InterruptTypePeripheral3:
	case InterruptTypeMouse:
	case InterruptTypeFpu:
	case InterruptTypePrimaryHardDisk:
	case InterruptTypeSecondaryHardDisk:
		break;
	}

	if (type >= InterruptTypeMinIrq && type != InterruptTypeTimer)
	{
		DebugPrint("Got interrupt 0x%x\n", type);
	}

	if (InterruptTypeMinIrq <= type && type <= InterruptTypeMaxIrq)
	{
		EndIrq(type);
	}
}

// exceptions/traps
MAKE_ISR_NO_ERROR(0x0)  // division err
MAKE_ISR_NO_ERROR(0x1)  // debug
MAKE_ISR_NO_ERROR(0x2)  // nmi
MAKE_ISR_NO_ERROR(0x3)  // breakpoint
MAKE_ISR_NO_ERROR(0x4)  // overflow
MAKE_ISR_NO_ERROR(0x5)  // bound exceeded
MAKE_ISR_NO_ERROR(0x6)  // invalid opcode
MAKE_ISR_NO_ERROR(0x7)  // device unavailable
MAKE_ISR_ERROR(0x8)     // double fault
MAKE_ISR_NO_ERROR(0x9)  // fpu segment overrun (legacy)
MAKE_ISR_ERROR(0xa)     // invalid tss
MAKE_ISR_ERROR(0xb)     // segment not present
MAKE_ISR_ERROR(0xc)     // stack segment fault
MAKE_ISR_ERROR(0xd)     // general protection fault
MAKE_ISR_NO_ERROR(0xe)  // page fault
MAKE_ISR_NO_ERROR(0x10) // x87 exception
MAKE_ISR_ERROR(0x11)    // alignment check
MAKE_ISR_NO_ERROR(0x12) // machine check
MAKE_ISR_NO_ERROR(0x13) // simd exception
MAKE_ISR_NO_ERROR(0x14) // virtualization exception
MAKE_ISR_ERROR(0x15)    // control protection exception
MAKE_ISR_NO_ERROR(0x1c) // hypervisor injection exception
MAKE_ISR_NO_ERROR(0x1d) // vmm communication exception
MAKE_ISR_NO_ERROR(0x1e) // security exception

// irqs
MAKE_ISR_NO_ERROR(0x20) // timer
MAKE_ISR_NO_ERROR(0x21) // keyboard
MAKE_ISR_NO_ERROR(0x22) // cascade
MAKE_ISR_NO_ERROR(0x23) // com2
MAKE_ISR_NO_ERROR(0x24) // com1
MAKE_ISR_NO_ERROR(0x25) // lpt2
MAKE_ISR_NO_ERROR(0x26) // floppy
MAKE_ISR_NO_ERROR(0x27) // lpt1/spurious
MAKE_ISR_NO_ERROR(0x28) // cmos
MAKE_ISR_NO_ERROR(0x29) // peripheral 1
MAKE_ISR_NO_ERROR(0x2a) // peripheral 2
MAKE_ISR_NO_ERROR(0x2b) // peripheral 3
MAKE_ISR_NO_ERROR(0x2c) // mouse
MAKE_ISR_NO_ERROR(0x2d) // fpu
MAKE_ISR_NO_ERROR(0x2e) // primary hard disk
MAKE_ISR_NO_ERROR(0x2f) // secondary hard disk

void InitializeIdt(void)
{
	// wipe the table (it should be clear already but idc)
	memset(s_idt, 0, sizeof(s_idt));

	DebugPrint("Initializing IDT\n");

	// exceptions
	RegisterGate(Isr0x0, InterruptTypeDivisionError, IdtGateTypeTrap32);
	RegisterGate(Isr0x1, InterruptTypeDebug, IdtGateTypeTrap32);
	RegisterGate(Isr0x2, InterruptTypeNonMaskable, IdtGateTypeTrap32);
	RegisterGate(Isr0x3, InterruptTypeBreakpoint, IdtGateTypeTrap32);
	RegisterGate(Isr0x4, InterruptTypeOverflow, IdtGateTypeTrap32);
	RegisterGate(Isr0x5, InterruptTypeBoundRangeExceeded, IdtGateTypeTrap32);
	RegisterGate(Isr0x6, InterruptTypeInvalidOpcode, IdtGateTypeTrap32);
	RegisterGate(Isr0x7, InterruptTypeDeviceNotAvailable, IdtGateTypeTrap32);
	RegisterGate(Isr0x8, InterruptTypeDoubleFault, IdtGateTypeTrap32);
	RegisterGate(Isr0x9, InterruptTypeCoprocessorSegmentOverrun, IdtGateTypeTrap32);
	RegisterGate(Isr0xa, InterruptTypeInvalidTss, IdtGateTypeTrap32);
	RegisterGate(Isr0xb, InterruptTypeSegmentNotPresent, IdtGateTypeTrap32);
	RegisterGate(Isr0xc, InterruptTypeStackSegmentFault, IdtGateTypeTrap32);
	RegisterGate(Isr0xd, InterruptTypeGeneralProtectionFault, IdtGateTypeTrap32);
	RegisterGate(Isr0xe, InterruptTypePageFault, IdtGateTypeTrap32);
	RegisterGate(Isr0x10, InterruptTypeX87FloatException, IdtGateTypeTrap32);
	RegisterGate(Isr0x11, InterruptTypeAlignmentCheck, IdtGateTypeTrap32);
	RegisterGate(Isr0x12, InterruptTypeMachineCheck, IdtGateTypeTrap32);
	RegisterGate(Isr0x13, InterruptTypeSimdFloatException, IdtGateTypeTrap32);
	RegisterGate(Isr0x14, InterruptTypeVirtualizationException, IdtGateTypeTrap32);
	RegisterGate(Isr0x15, InterruptTypeControlProtectionException, IdtGateTypeTrap32);
	RegisterGate(Isr0x1c, InterruptTypeHypervisorInjectionException, IdtGateTypeTrap32);
	RegisterGate(Isr0x1d, InterruptTypeVmmCommunicationException, IdtGateTypeTrap32);
	RegisterGate(Isr0x1e, InterruptTypeSecurityException, IdtGateTypeTrap32);

	// irqs
	RegisterGate(Isr0x20, InterruptTypeTimer, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x21, InterruptTypeKeyboard, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x22, InterruptTypeCascade, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x23, InterruptTypeCom2, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x24, InterruptTypeCom1, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x25, InterruptTypeLpt2, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x26, InterruptTypeFloppy, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x27, InterruptTypeLpt1, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x28, InterruptTypeCmos, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x29, InterruptTypePeripheral1, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2a, InterruptTypePeripheral2, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2b, InterruptTypePeripheral3, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2c, InterruptTypeMouse, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2d, InterruptTypeFpu, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2e, InterruptTypePrimaryHardDisk, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2f, InterruptTypeSecondaryHardDisk, IdtGateTypeInterrupt32);

	IdtDescriptor_t desc = {sizeof(s_idt) - 1, s_idt};

	DebugPrint("Setting IDT\n");

	// load the IDT
	asm volatile("lidt %0" : : "m"(desc));
}
