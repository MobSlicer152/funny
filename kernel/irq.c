#include "irq.h"
#include "kernel.h"
#include "serial.h"
#include "x86.h"

#define PIC1_BASE 0x20
#define PIC1_COMMAND (PIC1_BASE + 0x0)
#define PIC1_DATA (PIC1_BASE + 0x1)
#define PIC2_BASE 0xa0
#define PIC2_COMMAND (PIC2_BASE + 0x0)
#define PIC2_DATA (PIC2_BASE + 0x1)

#define PIC_END_OF_INTERRUPT 0x20

#define ICW1_ICW4 (1 << 0) // ICW4 will be in the sequence
#define ICW1_SINGLE (1 << 1) // cascade mode
#define ICW1_INTERVAL4 (1 << 2) // call address interval
#define ICW1_LEVEL (1 << 3) // level triggered mode
#define ICW1_INIT (1 << 4) // initialization

#define ICW4_8086 (1 << 0) // 8086 mode, instead of 8080 mode
#define ICW4_AUTO (1 << 1) // normal end of interrupt
#define ICW4_BUF_SECONDARY (1 << 3) // buffered mode, secondary
#define ICW4_BUF_PRIMARY (ICW4_BUF_SECONDARY + 4) // buffered mode, primary
#define ICW4_SFNM (1 << 4) // special fully nested mode

void InitializeIrq(void)
{
    DebugPrint("Initializing IRQs\n");

    // save masks
    u8 primaryMask = InByte(PIC1_DATA);
    u8 secondaryMask = InByte(PIC2_DATA);

    // start initialization sequence (ICW1)
    OutByte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IoWait();
    OutByte(PIC2_COMMAND, ICW1_INIT | ICW1_INIT);
    IoWait();

    // offset vectors (ICW2)
    OutByte(PIC1_DATA, InterruptTypeMinPrimaryIrq);
    IoWait();
    OutByte(PIC2_DATA, InterruptTypeMinSecondaryIrq);
    IoWait();

    // tell primary and secondary where secondary is (ICW3)
    OutByte(PIC1_DATA, 0b0100);
    IoWait();
    OutByte(PIC2_DATA, 0b0010);
    IoWait();

    // use 8086 mode (ICW4)
    OutByte(PIC1_DATA, ICW4_8086);
    IoWait();
    OutByte(PIC2_DATA, ICW4_8086);
    IoWait();

    // restore saved masks
    OutByte(PIC1_DATA, primaryMask);
    OutByte(PIC2_DATA, secondaryMask);
}

void EndIrq(InterruptType_t type)
{
    if (type >= InterruptTypeMinSecondaryIrq)
    {
        OutByte(PIC2_COMMAND, PIC_END_OF_INTERRUPT);
    }

    OutByte(PIC1_COMMAND, PIC_END_OF_INTERRUPT);
}
