#include "idt.h"
#include "interrupt.h"

static IdtEntry_t s_idt[48];

static void RegisterGate(Isr_t handler, InterruptType_t type)
{
    s_idt[type].offsetLower = (uptr)handler & 0xFFFF;
    s_idt[type].offsetUpper = (uptr)handler >> 16;

    s_idt[type].selector = KERNEL_CODE_SELECTOR << 3;
    s_idt[type].dpl = 0;
    
    s_idt[type].present = true;
}

void IsrCommon(InterruptType_t type)
{
    (void)type;
}

// exceptions/traps
MAKE_ISR(0x0) // division err
MAKE_ISR(0x1) // debug
MAKE_ISR(0x2) // nmi
MAKE_ISR(0x3) // breakpoint
MAKE_ISR(0x4) // overflow
MAKE_ISR(0x5) // bound exceeded
MAKE_ISR(0x6) // invalid opcode
MAKE_ISR(0x7) // device unavailable
MAKE_ISR(0x8) // double fault
MAKE_ISR(0x9) // fpu segment overrun (legacy)
MAKE_ISR(0xa) // invalid tss
MAKE_ISR(0xb) // segment not present
MAKE_ISR(0xc) // stack segment fault
MAKE_ISR(0xd) // general protection fault
MAKE_ISR(0xe) // page fault
MAKE_ISR(0x10) // x87 exception
MAKE_ISR(0x11) // alignment check
MAKE_ISR(0x12) // machine check
MAKE_ISR(0x13) // simd exception
MAKE_ISR(0x14) // virtualization exception
MAKE_ISR(0x15) // control protection exception
MAKE_ISR(0x1c) // hypervisor injection exception
MAKE_ISR(0x1d) // vmm communication exception
MAKE_ISR(0x1e) // security exception

// irqs
MAKE_ISR(0x20) // timer
MAKE_ISR(0x21) // keyboard
MAKE_ISR(0x22) // cascade
MAKE_ISR(0x23) // com2
MAKE_ISR(0x24) // com1
MAKE_ISR(0x25) // lpt2
MAKE_ISR(0x26) // floppy
MAKE_ISR(0x27) // lpt1/spurious
MAKE_ISR(0x28) // cmos
MAKE_ISR(0x29) // peripheral 1
MAKE_ISR(0x2a) // peripheral 2
MAKE_ISR(0x2b) // peripheral 3
MAKE_ISR(0x2c) // mouse
MAKE_ISR(0x2d) // fpu
MAKE_ISR(0x2e) // primary hard disk
MAKE_ISR(0x2f) // secondary hard disk

void InitializeIdt(void)
{
    __builtin_memset(s_idt, 0, sizeof(s_idt));

    // exceptions
    RegisterGate(Isr0x0, InterruptTypeDivisionError);
    RegisterGate(Isr0x1, InterruptTypeDebug);
    RegisterGate(Isr0x2, InterruptTypeNonMaskable);
    RegisterGate(Isr0x3, InterruptTypeBreakpoint);
    RegisterGate(Isr0x4, InterruptTypeOverflow);
    RegisterGate(Isr0x5, InterruptTypeBoundRangeExceeded);
    RegisterGate(Isr0x6, InterruptTypeInvalidOpcode);
    RegisterGate(Isr0x7, InterruptTypeDeviceNotAvailable);
    RegisterGate(Isr0x8, InterruptTypeDoubleFault);
    RegisterGate(Isr0x9, InterruptTypeCoprocessorSegmentOverrun);
    RegisterGate(Isr0xa, InterruptTypeInvalidTss);
    RegisterGate(Isr0xb, InterruptTypeSegmentNotPresent);
    RegisterGate(Isr0xc, InterruptTypeStackSegmentFault);
    RegisterGate(Isr0xd, InterruptTypeGeneralProtectionFault);
    RegisterGate(Isr0xe, InterruptTypePageFault);
    RegisterGate(Isr0x10, InterruptTypeX87FloatException);
    RegisterGate(Isr0x11, InterruptTypeAlignmentCheck);
    RegisterGate(Isr0x12, InterruptTypeMachineCheck);
    RegisterGate(Isr0x13, InterruptTypeSimdFloatException);
    RegisterGate(Isr0x14, InterruptTypeVirtualizationException);
    RegisterGate(Isr0x15, InterruptTypeControlProtectionException);
    RegisterGate(Isr0x1c, InterruptTypeHypervisorInjectionException);
    RegisterGate(Isr0x1d, InterruptTypeVmmCommunicationException);
    RegisterGate(Isr0x1e, InterruptTypeSecurityException);

    // irqs
    RegisterGate(Isr0x20, InterruptTypeTimer);
    RegisterGate(Isr0x21, InterruptTypeKeyboard);
}