#pragma once

#include "kernel.h"

typedef struct VgaMode
{
    u8 modeControl;
    u8 overscan;
    u8 colorPlaneEnable;
    u8 horizontalPanning;
    u8 colorSelect;
    u8 misc;
    u8 clockMode;
    
} VgaMode_t;
