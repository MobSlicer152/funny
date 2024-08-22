nasm -f bin -o %~dp0start.bin %~dp0start.asm
nasm -f bin -o %~dp0main.bin %~dp0main.asm
copy %~dp0start.bin+%~dp0main.bin %~dp0funny.img
