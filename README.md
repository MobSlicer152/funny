# funny thing

x86 toy os, runs in ring 0 protected mode

requires an 80386 with an 80387 fpu, an 8259 pic, an 8254 pit, 1mb of ram, a hard drive (the image is like 65k, any drive should do), and vga

## build instructions

you need fastbuild, clang, nasm, and python

`tools/` has fastbuild and bochs with the gui debugger for windows

1. copy `fastbuild/Config.Default.bff` to `fastbuild/Config.bff`, change any paths/values as needed
2. run `fbuild` in the root of the repo
3. build will be in `out/Clang/x86/Debug`, gives a hard drive image named `funny.img`
4. you can do `qemu-system-i386 -hda out/Clang/x86/Debug/funny.img` to run it, or `scripts\qemu.bat`/`scripts/qemu.sh` if you want a log of the serial output
   (qemu messes up the console input mode on windows)
5. if you want a `compile_commands.json` for your editor, do `fbuild -compdb`

## program flow

1. `boot/boot.asm`:
   - reads kernel with `int 13h` to `0x7e00`
   - sets VGA to mode 13h
   - enables A20 line
   - loads the GDT (defined in `boot/gdt.inc`)
   - enables protected mode
   - jumps into kernel at `0x7e00`
2. `kernel/startup.asm`:
   - loads segment registers
   - adjusts stack pointer
3. `kernel/main.c`:
   - configures serial COM1 (`serial.c`)
   - fills and loads the IDT (`idt.c`)
   - remaps IRQs to avoid conflicts with CPU exceptions (`irq.c`)
   - enables interrupts (`x86.h`)
   - initializes the x87 FPU (`fpu.c`)
   - initializes the screen (`screen.c`)
   - initializes the timer (`timer.c`)
   - enters a 30 FPS loop that clears the screen and draws rings of sprites
   - if, somehow, the `while (true)` loop is broken, disables interrupts (and NMIs) and enters a `while (true)` that does the `hlt` instruction
