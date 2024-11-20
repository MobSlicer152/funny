@echo off

set config=%1
if "%config%" == "" (
    set config=Debug
)

start cmd /c qemu-system-i386 -hda %~dp0..\out\Clang\x86\%config%\funny.img -serial stdio -rtc base=localtime ^> qemu.log
