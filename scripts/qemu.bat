::@echo off

set config=%1
if a%config% == a (
    set config=Debug
)
set flags=%2
if a%flags% == a (
    set flags=""
)

start cmd /c qemu-system-i386 -hda %~dp0..\out\Clang\x86\%config%\funny.img -serial stdio -rtc base=localtime %flags:"=% ^> qemu.log
