#pragma once

#include "types.h"

typedef enum InterruptType
{
    // exceptions
    InterruptTypeDivisionError = 0x0,
    InterruptTypeDebug = 0x1,
    InterruptTypeNonMaskable = 0x2,
    InterruptTypeBreakpoint = 0x3,
    InterruptTypeOverflow = 0x4,
    InterruptTypeBoundRangeExceeded = 0x5,
    InterruptTypeInvalidOpcode = 0x6,
    InterruptTypeDeviceNotAvailable = 0x7,
    InterruptTypeDoubleFault = 0x8,
    InterruptTypeCoprocessorSegmentOverrun = 0x9, // legacy
    InterruptTypeInvalidTss = 0xa,
    InterruptTypeSegmentNotPresent = 0xb,
    InterruptTypeStackSegmentFault = 0xc,
    InterruptTypeGeneralProtectionFault = 0xd,
    InterruptTypePageFault = 0xe,
    InterruptTypeX87FloatException = 0x10,
    InterruptTypeAlignmentCheck = 0x11,
    InterruptTypeMachineCheck = 0x12,
    InterruptTypeSimdFloatException = 0x13,
    InterruptTypeVirtualizationException = 0x14,
    InterruptTypeControlProtectionException = 0x15,
    InterruptTypeHypervisorInjectionException = 0x1c,
    InterruptTypeVmmCommunicationException = 0x1d,
    InterruptTypeSecurityException = 0x1e,

    InterruptTypeMinException = InterruptTypeDivisionError,
    InterruptTypeMaxException = InterruptTypeSecurityException,

    // remapped IRQs
    InterruptTypeTimer = 0x20,
    InterruptTypeKeyboard = 0x21,
    InterruptTypeCascade = 0x22,
    InterruptTypeCom2 = 0x23,
    InterruptTypeCom1 = 0x24,
    InterruptTypeLpt2 = 0x25,
    InterruptTypeFloppy = 0x26,
    InterruptTypeLpt1 = 0x27,
    InterruptTypeSpurious = 0x27,
    InterruptTypeCmos = 0x28,
    InterruptTypePeripheral1 = 0x29,
    InterruptTypePeripheral2 = 0x2a,
    InterruptTypePeripheral3 = 0x2b,
    InterruptTypeMouse = 0x2c,
    InterruptTypeFpu = 0x2d,
    InterruptTypePrimaryHardDisk = 0x2e,
    InterruptTypeSecondaryHardDisk = 0x2f,
    
    InterruptTypeMinIrq = InterruptTypeTimer,
    InterruptTypeMaxIrq = InterruptTypeSecondaryHardDisk,
    InterruptTypeMinPrimaryIrq = InterruptTypeTimer,
    InterruptTypeMaxPrimaryIrq = InterruptTypeLpt1,
    InterruptTypeMinSecondaryIrq = InterruptTypeCmos,
    InterruptTypeMaxSecondaryIrq = InterruptTypeSecondaryHardDisk,
} InterruptType_t;

// initialize and remap pic, start interrupts
extern void InitializeIrq(void);

// send an eoi to pic
extern void EndIrq(InterruptType_t type);
