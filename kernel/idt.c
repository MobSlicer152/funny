#include "idt.h"
#include "interrupt.h"

static IdtEntry_t s_idt[48];

static void RegisterGate(Isr_t handler, InterruptType_t type, IdtGateType_t gateType)
{
	s_idt[type].offsetLower = (uptr)handler & 0xFFFF;
	s_idt[type].offsetUpper = (uptr)handler >> 16;

	s_idt[type].gateType = gateType;

	// these fields are always the same
	s_idt[type].selector = KERNEL_CODE_SELECTOR << 3;
	s_idt[type].dpl = 0;
	s_idt[type].present = true;
}

void ATTRIBUTE(no_caller_saved_registers) IsrCommon(InterruptType_t type, u32 error)
{
	(void)error;

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
	case InterruptTypeTimer:
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
	// wipe the table (it should be clear already but idk)
	__builtin_memset_inline(s_idt, 0, sizeof(s_idt));

	// exceptions
	RegisterGate(Isr0x0, InterruptTypeDivisionError, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x1, InterruptTypeDebug, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x2, InterruptTypeNonMaskable, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x3, InterruptTypeBreakpoint, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x4, InterruptTypeOverflow, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x5, InterruptTypeBoundRangeExceeded, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x6, InterruptTypeInvalidOpcode, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x7, InterruptTypeDeviceNotAvailable, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x8, InterruptTypeDoubleFault, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x9, InterruptTypeCoprocessorSegmentOverrun, IdtGateTypeInterrupt32);
	RegisterGate(Isr0xa, InterruptTypeInvalidTss, IdtGateTypeInterrupt32);
	RegisterGate(Isr0xb, InterruptTypeSegmentNotPresent, IdtGateTypeInterrupt32);
	RegisterGate(Isr0xc, InterruptTypeStackSegmentFault, IdtGateTypeInterrupt32);
	RegisterGate(Isr0xd, InterruptTypeGeneralProtectionFault, IdtGateTypeInterrupt32);
	RegisterGate(Isr0xe, InterruptTypePageFault, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x10, InterruptTypeX87FloatException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x11, InterruptTypeAlignmentCheck, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x12, InterruptTypeMachineCheck, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x13, InterruptTypeSimdFloatException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x14, InterruptTypeVirtualizationException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x15, InterruptTypeControlProtectionException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x1c, InterruptTypeHypervisorInjectionException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x1d, InterruptTypeVmmCommunicationException, IdtGateTypeInterrupt32);
	RegisterGate(Isr0x1e, InterruptTypeSecurityException, IdtGateTypeInterrupt32);

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

	IdtDescriptor_t desc = {(uptr)s_idt, sizeof(s_idt)};

	// load the IDT
	asm volatile("lidt %0" : : "m"(desc));
}
