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
